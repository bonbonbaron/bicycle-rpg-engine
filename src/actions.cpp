#include <bicycle/Personality.h>
#include "example-actions.h"

// TODO try putting the keys all in one string, spaced by commas and/or whitespace to seem natural to the user

extern "C" {
  ActPkg actions[] {
    ACT( act1, "key1", "key2" ),
    ACT( act2, "key3" ),
  };

  int numActions = sizeof( actions ) / sizeof( actions[0] );
}

