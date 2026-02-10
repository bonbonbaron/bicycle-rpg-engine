#pragma once
#include <bicycle/Blackboard.h>

// You have to make a shared pointer to the blackboard first, then its header, and then you have to include that header in the file that actually maps all the blackboards to names.

struct A {
  int i;
  char j;
  double k;
};

Blackboard bb1 { 
  { "aKey", A( 45, 'j', 1234.5678 ) },
  { "bKey", A( 46, 'n', 1234.5678 ) },
  { "cKey", A( 48, 'k', 1234.5678 ) },
  { "dKey", A( 50, 'l', 1234.5678 ) },
  { "eKey", A( 54, 'm', 1234.5678 ) },
};

Blackboard bb2 {
  { "aKey", A( 45, 'j', 1234.5678 ) },
  { "eKey", A( 54, 'm', 1234.5678 ) },
};

Blackboard bb3 {
  { "aKey", A( 45, 'j', 1234.5678 ) },
};
