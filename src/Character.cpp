#include "Character.h"
#include <vector>
#include <bicycle/bicycle.h>
#include <bicycle/Menu.h>

Character::Character( std::string name,
        const BattleStats& stats) :
      name(name), stats(stats) {}

void Character::fight( CharMap& enemies ) {
  std::vector<MenuItem> availableEnemies{};

  // Add all enemies and the effects of fighting them to the menu.
  for ( auto& e : enemies ) {
    availableEnemies.emplace_back ( 
      e.first,  // Menu item name
      [&]() {   // Menu item effect
        if ( stats.hp > 0 ) {
          auto effect = std::make_shared<Effect>( "fight", -stats.strength, TargetAttribute::HP, stats.type );
          action = std::make_shared<Action>( shared_from_this(), e.second, effect );  // Mark this character's chosen action.
        }
        bicycle::pop();  // pop the target menu
        bicycle::pop();  // pop the battle menu
      }
    );
  } 

  // Create the menu.
  bicycle::push<Menu>( availableEnemies, x + 1, y + 2, 5 );
}

void Character::spell( CharMap& allies, CharMap& enemies ) {

}

void Character::item( CharMap& allies, CharMap& enemies ) {

}


