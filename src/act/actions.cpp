#include <bicycle/Personality.h>
#include "act/example-actions.h"

// TODO try putting the keys all in one string, spaced by commas and/or whitespace to seem natural to the user

extern "C" {
  ActPkg actions[] {
    ACT( Example::act1, "key1", "key2" ),
    ACT( Example::act2, "key3" ),
  };

  int numActions = sizeof( actions ) / sizeof( actions[0] );
}

