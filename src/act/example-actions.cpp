#include "act/example-actions.h"
#include <bicycle/Entity.h>
#include <bicycle/Grid.h>

using enum ActionState;
using namespace Example;

// We need a common function for testing a neighboring tile for collision types.

// Solid collisions can simply block our way.
enum BodyType {
  NOTHING = 0,
  SOLID,
  DOOR,
  FIRE,
  NUM_BODY_TYPES
};

/* 
 * if i can map onCollision, onTimer, etc. in yaml, then it'd be nice to be able to inherit from and append toother trees. With the option to override either an individual key or a whole branch (collision/timer/input) of the parent tree.
 */

F( onInput ) {
  auto& pos = arg.get<std::shared_ptr<Position>>( "pos" );
  auto& input = arg.get<int>( "input" );
  auto& env = arg.get<Environment>( "env" );
  Position delta;
  switch( input ) {
    case 'h':
    case 'H':
      delta.x = -1;
      break;
    case 'j':
    case 'J':
      delta.y = 1;
      break;
    case 'k':
    case 'K':
      delta.y = -1;
      break;
    case 'l':
    case 'L':
      delta.x = 1;
      break;
  }
  auto bgColl = checkBgCollision( pos, delta, env );
  if ( bgColl == NOTHING ) {
    *pos += delta;
  }
  else {
    onCollision( arg );
  }

  return READY;
}

F( onCollision ) {
  auto& collisionType = arg.get<int>( "collisionType" );
  switch( collisionType ) {
    default:
      break;
  }
  return READY;
}

F( act2 ) {
  auto& i = arg.get<int>( "key3" );
  return COMPLETE;
}

