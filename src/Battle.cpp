#include "Battle.h"
#include "GameState.h"
#include <bicycle/bicycle.h>
#include <bicycle/Menu.h>
#include <memory>
#include <algorithm>
#include "Dice.h"

// GOALS
//  1) fix turn order
//  2) show health bars
//  3) colorize (2)

using namespace BG;
BattleStats gs { 10, 10, 15, 10, 10, EffectType::NORMAL, 1 };

// This itself is pushed to the stack in my current rendition of main().
Battle::Battle() : Constellation ( COLS / 2, LINES / 2 ) {

  // auto& gs = GameState::getInstance();  // TODO not necessary (yet)
  // TODO replace below; somehow derive the list of enemies based on area or some other game state
  // error: no matching function for call to ‘construct_at(Character*&, const char [15], int, int, int, BattleStats&, BattleStats&, BattleStats&)’
  auto fred = std::make_shared<Character>( "Fred", gs );
  auto ted  = std::make_shared<Character>( "Ted",  gs );
  auto ed   = std::make_shared<Character>( "Ed",   gs );
  auto monster1 = std::make_shared<Character>( "monster bean 1", gs );
  auto monster2 = std::make_shared<Character>( "monster bean 2", gs );
  auto monster3 = std::make_shared<Character>( "monster bean 3", gs );

  // make monsters super weak so they can't kill good guys (yet)
  monster1->stats.strength = 100;
  monster1->stats.speed = 7;
  monster2->stats.strength = 100;
  monster2->stats.speed = 9;
  monster3->stats.strength = 1;
  monster3->stats.speed = 2;

  fred->stats.speed = 3;
  ted->stats.speed = 8;
  ed->stats.speed = 10;

  _heroes[ fred->name ] = fred;
  _heroes[ ted->name ] = ted;
  _heroes[ ed->name ] = ed;
  _enemies[ monster1->name ] = monster1;
  _enemies[ monster2->name ] = monster2;
  _enemies[ monster3->name ] = monster3;

  positionCharacters( _enemies, 1, RED );
  positionCharacters( _heroes, 3, GREEN );
  // stay dumb for now Michael... stay dumb for the proof of concept
  resetSequence();
}

void Battle::positionCharacters( CharMap& chars, const int heightRatio, const Color color ) {
  int i = 1;
  for ( const auto& c: chars ) {
    // Centers hero if size is 1.
    auto name = c.second->name;
    auto x = static_cast<int>( i++ * getWidth() / (chars.size() + 1) - name.length()/2 );
    auto y = static_cast<int>( heightRatio*getHeight()/4 );
    setPoint( name, { color, x, y, name } );
    c.second->x = x;
    c.second->y = y;
  }
}


// Can I insert the character by having them fight inside the lambda?
void Battle::resetSequence() {
  constexpr int MENU_HEIGHT{ 5 };
  // This is a bit awkward as constellation and actual character data live separately.
  const auto& points = getPoints();
  for ( auto& [ k, hero ] : _heroes ) {
    // Only give this hero a turn if he's still alive. 
    if ( hero->stats.hp > 0 ) {
      // Get *living* hero's position in the constellation.
      auto p = points.find( k );
      if ( p != points.end() ) {
        // Make a battle menu beneath this character's symbol.
        std::vector<MenuItem> battleMenuItems{};
        battleMenuItems.emplace_back( "Fight", [&, this]() { hero->fight( _enemies ); }  );
        battleMenuItems.emplace_back( "Spells", [&, this]() { hero->spell( _heroes, _enemies ); } );
        battleMenuItems.emplace_back( "Items", [&, this]() { hero->item( _heroes, _enemies ); } );
        _seq.push<Menu>( std::move(battleMenuItems), getX() + hero->x - hero->name.length() / 2, getY() + hero->y + 1, MENU_HEIGHT );
      }
    }
  }
}

void Battle::clean( CharMap& characters ) {
  std::vector<std::string> keysToRemove{};
  // Remove dead characters from constellation
  for ( const auto& c : characters ) {
    if ( c.second->stats.hp <= 0 ) {
      removePoint( c.first );
      // Defer removal from the map we're iterating over.
      keysToRemove.push_back( c.first );
    }
  }

  // Remove character from character map.
  for ( const auto& key : keysToRemove ) {
    auto it = characters.find( key );
    if ( it != characters.end() ) {
      characters.erase( it );
    }
    if ( characters.size() ) {
      int i = 0;
    }
  }
}

void Battle::simulateBadGuyChoices() {
  Dice dice{ 0, static_cast<int>( _heroes.size() ) - 1 };
  const auto firstHeroIt = _heroes.begin();

  for ( auto& e : _enemies ) {
    auto tgtHeroIt = firstHeroIt;
    auto effect = std::make_shared<Effect>( 
        "fight", 
        -e.second->stats.strength, 
        TargetAttribute::HP, 
        e.second->stats.type );

    // Maybe there's a better way to do this, but map iterators don't let you add an integer to an iterator.
    for ( int idx = 0, tgtIdx = dice.roll(); idx < tgtIdx; ++idx, ++tgtHeroIt );

    e.second->action = std::make_shared<Action>( 
        e.second,
        tgtHeroIt->second,
        effect );  // Mark this character's chosen action.
  }
}

void Battle::aggregateActions() {
  for ( auto& h : _heroes ) {
    if ( h.second->action != nullptr ) {
      _actionSequence.emplace_back( *h.second->action );
    }
  }

  for ( auto& e : _enemies ) {
    if ( e.second->action != nullptr ) {
      _actionSequence.emplace_back( *e.second->action );
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
      mvprintw( LINES - 8 + i++, 1, "%s hits %s for %d damage with %d speed. %s's HP = %d",
          a.src->name.c_str(),
          a.dst->name.c_str(), 
          a.effect->amount,
          a.src->stats.speed,
          a.dst->name.c_str(),
          a.dst->stats.hp );
    }
  }
}

//void Battle::drawHealthBars() {
//for ( const auto& e : _enemies ) {


void Battle::update() {
  if ( _enemies.size() == 0 || _heroes.size() == 0 ) {
    bicycle::pop();  // Pop the battle menu.
    bicycle::pop();  // Pop the battle window.
  }
  else {
    if ( _seq.isComplete() ) {
      // All the actions have been chosen. Sort them by order of speed and execute them.
      simulateBadGuyChoices();
      aggregateActions();
      sortActions();
      executeActions();
      _actionSequence.clear();
      clean( _enemies );  // blows away enemies from constellation AND map
      clean( _heroes );   // blows away heroes  from constellation AND map
                          // Don't revive heroes' battle menus if the battle's over.
      if ( _enemies.size() > 0 && _heroes.size() > 0 ) {
        resetSequence();
      }
    }
    // Don't act on menus if they don't exist.
    if ( _enemies.size() > 0 && _heroes.size() > 0 ) {
      _seq.tick();
    }
    Constellation::update();
  }
}

void Battle::react( const int i ) {
}
