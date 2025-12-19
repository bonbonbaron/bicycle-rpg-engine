#pragma once
#include <bicycle/Constellation.h>
#include <bicycle/Sequence.h>
#include <memory>
#include <map>
#include "Character.h"
#include <vector>

using CharPoint = Point<Character>;

class Battle : public std::enable_shared_from_this<Battle>, public Constellation<Character> {
  public:
    Battle();
    void update() override;
    void react( const int i ) override;
  private:
    // private member variables
    Sequence _seq{};
    std::vector<Action> _actionSequence{};
    // private member functions
    void aggregateActions();
    void sortActions();
    void executeActions();
    void simulateBadGuyChoices();
    void resetSequence();
    void clean();
    void drawHealthBars() const;
    PointMap<Character> _heroes{};
    PointMap<Character> _enemies{};
};
