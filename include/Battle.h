#pragma once
#include <bicycle/Personality.h>
#include <memory>
#include <map>
#include "Character.h"
#include <vector>

class Battle : public std::enable_shared_from_this<Battle> {
  public:
    Battle();
    void update() override;
    void react( const int i ) override;
    void run();
  private:
    // private member functions
    void aggregateActions();
    void sortActions();
    void executeActions();
    void simulateBadGuyChoices();
    void resetSequence();
    // void clean();
    void drawHealthBars() const;
};
