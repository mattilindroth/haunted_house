#pragma once
#include <string>
#include <vector>

struct ScoreEntry {
    std::string name;
    float score;
};

class Scoreboard {
public:
    static const int MAX_ENTRIES = 10;
    static const int MAX_NAME_LENGTH = 12;
    
    // Current state for name entry
    bool enteringName = false;
    std::string currentName;
    int newScoreRank = -1;  // -1 if not on scoreboard
    
    // Lifecycle
    void Init();
    void Save();
    
    // Score management
    bool IsHighScore(float score) const;
    int GetRank(float score) const;  // Returns rank (0-9) or -1 if not on board
    void AddScore(const std::string& name, float score);
    void StartNameEntry(float score);
    
    // Name entry
    void UpdateNameEntry();
    bool IsEnteringName() const { return enteringName; }
    void ConfirmName(float score);
    
    // Drawing
    void Draw(int screenWidth, int screenHeight, float currentScore);
    
    // Get scores
    const std::vector<ScoreEntry>& GetScores() const { return scores; }
    
private:
    std::vector<ScoreEntry> scores;
    std::string filePath = "../scores.dat";
    float pendingScore = 0.0f;
};

// Global scoreboard instance
extern Scoreboard scoreboard;
