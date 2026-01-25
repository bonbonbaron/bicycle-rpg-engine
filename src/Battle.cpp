#include "Battle.h"
#include "GameState.h"
#include <algorithm>
#include <bicycle/Dice.h>

// This itself is pushed to the stack in my current rendition of main().
Battle::Battle() {
  auto& gs = GameState::getInstance();
  auto& party = gs.getParty();
  auto  monsters = gs.getMonsters();
  int i{};

  // Add party to battlefield
  constexpr int PARTY_ROW{3};
  for ( auto& p : party ) {
    auto x = static_cast<int>( i++ * getWidth() / (party.size() + 1) - p->name.length()/2 );
    auto y = static_cast<int>( PARTY_ROW*getHeight()/4 );
  }

  // Add monsters to battlefield
  constexpr int MONSTER_ROW{3};
  for ( auto& m : monsters ) {
    auto x = static_cast<int>( i++ * getWidth() / (party.size() + 1) - m.name.length()/2 );
    auto y = static_cast<int>( MONSTER_ROW*getHeight()/4 );
  }

  // Parse the map for good guys and bad guys to make selections easier.
  _heroes = filter( []( CharPoint& c ) { return c.good; } );
  _enemies = filter( []( CharPoint& c ) { return !c.good; } );
}

void Battle::clean() {
  std::vector<std::string> keysToRemove{};
  // Remove dead characters from constellation
  auto points = getPoints();
  // TODO might be cleaner to use filter() here.
  for ( auto it = points.begin(); it != points.end(); ++it ) {
    if ( it->second.stats.hp <= 0 ) {
      if ( it->second.good ) {
        auto it2 = _heroes.find( it->first );
        if ( it2 != _heroes.end() ) {
          _heroes.erase( it2 );
        }
      }
      else {
        auto it2 = _enemies.find( it->first );
        if ( it2 != _enemies.end() ) {
          _enemies.erase( it2 );
        }
      }
      it = points.erase( it );
    }
  }
}



void Battle::simulateBadGuyChoices() {
  Dice dice{ 0, static_cast<int>(_heroes.size()) - 1 };
  for (auto& e : _enemies ) {
    auto effect = std::make_shared<Effect>( 
        "fight", 
        -e.second.stats.strength, 
        TargetAttribute::HP, 
        e.second.stats.type );

    auto h = _heroes.begin();
    std::advance( h, dice.roll() );

    e.second.action = std::make_shared<Action>( 
        e.second.shared_from_this(),
        std::static_pointer_cast<Character>( h->second.shared_from_this() ),
        //it.shared_from_this(),
        effect );  // Mark this character's chosen action.
  }
}

void Battle::aggregateActions() {
  for ( const auto& p : getPoints() ) {
    if ( p.second.action != nullptr ) {
      _actionSequence.emplace_back( *p.second.action );
    }
  }
}

void Battle::sortActions() {
  std::sort( _actionSequence.begin(), _actionSequence.end() );
}

void Battle::executeActions() {
  int i = 0;
  for ( auto& a : _actionSequence ) {
    if ( a.src->stats.hp > 0 && a.dst->stats.hp > 0 ) {
      a.execute();
    }
  }
}

// This funcdtion needs to be pointed at to repeat for an event.
// A battle is a sequence of:
// 1. a song
// 2. a battle
// 3. a victory/defeat fork:
//   3a1. victory song
//   3a2. what you gain from the battle
//   3b1. defeat song
//   3b2. dialogue saying you're a loser
//   3b3. menu asking whether to continue or quit
void Battle::run() {
  simulateBadGuyChoices();
  aggregateActions();
  sortActions();
  executeActions();
  clean();  
}

