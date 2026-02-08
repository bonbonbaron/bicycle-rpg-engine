#include "example-actions.h"

using enum ActionState;

F( act1 ) {
  auto& i = arg.get<int>( "key1" );
  auto& j = arg.get<int>( "key2" );
  return COMPLETE;
}


F( act2 ) {
  auto& i = arg.get<int>( "key3" );
  return COMPLETE;
}

