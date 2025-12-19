#include "Battle.h"
#include "GameState.h"
#include <bicycle/bicycle.h>
#include <bicycle/Menu.h>
#include <algorithm>
#include "Dice.h"

struct Action;

// GOALS
//  1) fix turn order
//  2) show health bars
//  3) colorize (2)

// This itself is pushed to the stack in my current rendition of main().
Battle::Battle() : Constellation<Character> ( COLS / 2, LINES / 2 ) {
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

  resetSequence();
}

// Can I insert the character by having them fight inside the lambda?
void Battle::resetSequence() {
  constexpr int MENU_HEIGHT{ 5 };
  // This is a bit awkward as constellation and actual character data live separately.
  auto& points = getPoints();
  for ( auto p : points ) {
    // Only give this hero a turn if he's still alive. 
    if ( p.second.stats.hp > 0 ) {
      // Make a battle menu beneath this character's symbol.
      std::vector<MenuItem> battleMenuItems{};
      battleMenuItems.emplace_back( "Fight", [&]() { p.second.fight( shared_from_this() ); }  );
      battleMenuItems.emplace_back( "Spells", [&]() { p.second.spell( shared_from_this() ); } );
      battleMenuItems.emplace_back( "Items", [&]() { p.second.item( shared_from_this() ); } );
      _seq.push<Menu>( std::move(battleMenuItems), getX() + p.second.x - p.second.name.length() / 2, getY() + p.second.y + 2, MENU_HEIGHT );
    }
  }
}

void Battle::clean() {
  std::vector<std::string> keysToRemove{};
  // Remove dead characters from constellation
  auto points = getPoints();
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

void Battle::drawHealthBars() const {
  int rownum = 2;
  constexpr int MAX_BAR_LENGTH{10};  // green threshold
  constexpr int ONE_THIRD_MAX_BAR_LENGTH{ MAX_BAR_LENGTH / 3};  // red threshold
  constexpr int TWO_THIRDS_BAR_LENGTH{ static_cast<int>(2 * ONE_THIRD_MAX_BAR_LENGTH) }; // yellow threshold
  constexpr int GREEN{1};
  constexpr int YELLOW{2};
  constexpr int RED{3};
  init_pair( GREEN, COLOR_WHITE, COLOR_GREEN );
  init_pair( YELLOW, COLOR_WHITE, COLOR_YELLOW );
  init_pair( RED, COLOR_WHITE, COLOR_RED );
  for ( const auto& c : getPoints() ) {
    // Build HP string.
    long unsigned barLength{ static_cast<long unsigned>( MAX_BAR_LENGTH * c.second.stats.hp / c.second.maxStats.hp ) };
    mvprintw( getY() + c.second.y + 1, getX() + c.second.x, "[" );
    if ( has_colors() && can_change_color() ) {
      int colorNum{};
      if ( barLength > TWO_THIRDS_BAR_LENGTH ) {
        colorNum = GREEN;
      }
      else if ( barLength > ONE_THIRD_MAX_BAR_LENGTH ) {
        colorNum = YELLOW;
      }
      else {
        colorNum = RED;
      }
      attron( COLOR_PAIR(colorNum) );
      printw( "%.*s", barLength, "          " );
      attroff( COLOR_PAIR(colorNum) );
      printw( "%.*s", MAX_BAR_LENGTH - barLength, "          " );
    }
    else {
      printw( "%.*s", barLength, "@@@@@@@@@@" );
      printw( "%.*s", MAX_BAR_LENGTH - barLength, "          " );
    }
    printw( "]" );
    ++rownum;
  }
}


void Battle::update() {
  if ( _heroes.size() == 0 || _enemies.size() == 0) {
    bicycle::pop();  // Pop the battle menu.
    bicycle::pop();  // Pop the battle window.  (TODO if you automate someone, this window won't exist. Careful!) 
  }
  else {
    if ( _seq.isComplete() ) {
      // All the actions have been chosen. Sort them by order of speed and execute them.
      simulateBadGuyChoices();
      aggregateActions();
      sortActions();
      executeActions();
      _actionSequence.clear();
      clean();  
      // Don't revive heroes' battle menus if the battle's over.
      if (  _heroes.size() > 0 && _enemies.size() > 0 ) {
        resetSequence();
      }
    }
    // Don't act on menus if they don't exist.
    if ( _heroes.size() > 0 && _enemies.size() > 0 ) {
      if ( ! isSelecting() ) {
        _seq.tick();
      }
    }
    Constellation::update();
    drawHealthBars();
  }
}

void Battle::react( const int i ) {
}
