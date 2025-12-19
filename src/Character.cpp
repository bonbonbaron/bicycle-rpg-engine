#include "Character.h"
#include <vector>
#include <bicycle/bicycle.h>
#include <bicycle/Menu.h>
#include "Battle.h"

Character::Character() {}

Character::Character( std::string name,
        const Stats& stats,
        const Stats& maxStats) :
      name(name), stats(stats), maxStats(maxStats) {}

// TODO pass in the maps of heroes and villains too-- SHIT!-- so you can act on them w/ the lambda's Point result.
//      You can re-aggregate the remaining characters only when somebody dies.
void Character::fight( std::shared_ptr<Battle>&& battle) {
  auto selectionFunc = [&, this]() {
    if ( stats.hp > 0 ) {
      auto effect = std::make_shared<Effect>( "fight", -stats.strength, TargetAttribute::HP, stats.type );
      //action = std::make_shared<Action>( shared_from_this(), c->second, effect );  // Mark this character's chosen action.
      // TODO have constellation override control
      auto& wm = WindowManager::getInstance();
        // std::static_pointer_cast<Character>( h->second.shared_from_this() ),
      wm.contextOverride( std::static_pointer_cast<Window>( battle ) );
    }
  };
  // You'll select whom to act on from the callback.
  battle->setCallback( selectionFunc );  // calls when selecting item from battle's constellation
  bicycle::pop();  // pop the battle menu to give constellation control of selecting
}

void Character::spell(  std::shared_ptr<Battle>&& battle ) {

}

void Character::item( std::shared_ptr<Battle>&& battle) {

}


