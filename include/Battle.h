#pragma once
#include <bicycle/Constellation.h>
#include <bicycle/Sequence.h>
#include "Character.h"

namespace BG {
  // Let's just see how a battle would look before we worry about the design overly much.

  class Battle : public Constellation {
    public:
      Battle();
      void update() override;
      void react( const int i ) override;
    private:
      // private member variables
      CharMap _heroes{};
      CharMap _enemies{};
      int _turnNum{};
      Sequence _seq{};
      std::vector<Action> _actionSequence{};
      // private member functions
      void positionCharacters( CharMap& chars, const int heightRatio, const Color color );
      void aggregateActions();
      void sortActions();
      void executeActions();
      void simulateBadGuyChoices();
      void resetSequence();
      void clean( CharMap& characters );
  };
}  // namespace BG
