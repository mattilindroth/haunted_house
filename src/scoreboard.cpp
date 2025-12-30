#include "scoreboard.h"
#include "raylib.h"
#include <fstream>
#include <algorithm>

// Global instance
Scoreboard scoreboard;

void Scoreboard::Init() {
    scores.clear();
    
    // Try to load existing scores
    std::ifstream file(filePath);
    if (file.is_open()) {
        std::string name;
        float score;
        while (file >> name >> score) {
            scores.push_back({ name, score });
        }
        file.close();
    }
    
    // Sort by score descending
    std::sort(scores.begin(), scores.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
        return a.score > b.score;
    });
    
    // Trim to max entries
    if (scores.size() > MAX_ENTRIES) {
        scores.resize(MAX_ENTRIES);
    }
}

void Scoreboard::Save() {
    std::ofstream file(filePath);
    if (file.is_open()) {
        for (const auto& entry : scores) {
            file << entry.name << " " << entry.score << "\n";
        }
        file.close();
    }
}

bool Scoreboard::IsHighScore(float score) const {
    if (scores.size() < MAX_ENTRIES) return true;
    return score > scores.back().score;
}

int Scoreboard::GetRank(float score) const {
    for (int i = 0; i < (int)scores.size(); i++) {
        if (score > scores[i].score) {
            return i;
        }
    }
    if (scores.size() < MAX_ENTRIES) {
        return (int)scores.size();
    }
    return -1;
}

void Scoreboard::AddScore(const std::string& name, float score) {
    scores.push_back({ name, score });
    
    // Sort by score descending
    std::sort(scores.begin(), scores.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
        return a.score > b.score;
    });
    
    // Trim to max entries
    if (scores.size() > MAX_ENTRIES) {
        scores.resize(MAX_ENTRIES);
    }
    
    Save();
}

void Scoreboard::StartNameEntry(float score) {
    if (IsHighScore(score)) {
        enteringName = true;
        currentName = "";
        pendingScore = score;
        newScoreRank = GetRank(score);
    } else {
        newScoreRank = -1;
    }
}

void Scoreboard::UpdateNameEntry() {
    if (!enteringName) return;
    
    // Get typed characters
    int key = GetCharPressed();
    while (key > 0) {
        // Only allow alphanumeric and some special characters
        if ((key >= 32) && (key <= 125) && (currentName.length() < MAX_NAME_LENGTH)) {
            currentName += (char)key;
        }
        key = GetCharPressed();
    }
    
    // Handle backspace
    if (IsKeyPressed(KEY_BACKSPACE) && currentName.length() > 0) {
        currentName.pop_back();
    }
    
    // Confirm with Enter
    if (IsKeyPressed(KEY_ENTER) && currentName.length() > 0) {
        ConfirmName(pendingScore);
    }
}

void Scoreboard::ConfirmName(float score) {
    // Replace spaces with underscores for file storage
    std::string safeName = currentName;
    for (char& c : safeName) {
        if (c == ' ') c = '_';
    }
    
    AddScore(safeName, score);
    enteringName = false;
    currentName = "";
    newScoreRank = -1;
}

void Scoreboard::Draw(int screenWidth, int screenHeight, float currentScore) {
    int startY = 120;
    int centerX = screenWidth / 2;
    
    // Draw scoreboard title
    DrawText("HIGH SCORES", centerX - 80, startY, 24, GOLD);
    startY += 40;
    
    // Draw header
    DrawText("RANK", centerX - 180, startY, 16, LIGHTGRAY);
    DrawText("NAME", centerX - 80, startY, 16, LIGHTGRAY);
    DrawText("TIME", centerX + 80, startY, 16, LIGHTGRAY);
    startY += 25;
    
    // Draw divider
    DrawLine(centerX - 180, startY, centerX + 150, startY, GRAY);
    startY += 10;
    
    // Draw scores
    for (int i = 0; i < MAX_ENTRIES; i++) {
        Color rowColor = WHITE;
        
        // Highlight if this is where the new score will go
        if (enteringName && i == newScoreRank) {
            // Draw input field for new score
            DrawText(TextFormat("%2d.", i + 1), centerX - 180, startY, 18, YELLOW);
            
            // Blinking cursor
            std::string displayName = currentName;
            if ((int)(GetTime() * 2) % 2 == 0) {
                displayName += "_";
            }
            DrawText(displayName.c_str(), centerX - 80, startY, 18, YELLOW);
            DrawText(TextFormat("%.1f s", pendingScore), centerX + 80, startY, 18, YELLOW);
            startY += 22;
            continue;
        }
        
        // Adjust index for display when entering name
        int scoreIndex = i;
        if (enteringName && newScoreRank >= 0 && i > newScoreRank) {
            scoreIndex = i - 1;
        }
        
        if (scoreIndex < (int)scores.size() && (!enteringName || i != newScoreRank)) {
            // Highlight current player's score position
            if (!enteringName && i == GetRank(currentScore) && currentScore > 0) {
                rowColor = GREEN;
            }
            
            DrawText(TextFormat("%2d.", i + 1), centerX - 180, startY, 18, rowColor);
            DrawText(scores[scoreIndex].name.c_str(), centerX - 80, startY, 18, rowColor);
            DrawText(TextFormat("%.1f s", scores[scoreIndex].score), centerX + 80, startY, 18, rowColor);
        } else if (!enteringName) {
            // Empty slot
            DrawText(TextFormat("%2d.", i + 1), centerX - 180, startY, 18, DARKGRAY);
            DrawText("---", centerX - 80, startY, 18, DARKGRAY);
            DrawText("---", centerX + 80, startY, 18, DARKGRAY);
        }
        
        startY += 22;
    }
    
    // Draw instructions
    startY += 20;
    if (enteringName) {
        DrawText("Enter your name and press ENTER", centerX - 140, startY, 16, YELLOW);
    } else {
        DrawText("Press SPACE to play again", centerX - 110, startY, 16, WHITE);
    }
}
