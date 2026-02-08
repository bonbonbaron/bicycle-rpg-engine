#include <bicycle/Personality.h>

extern "C" {
  // old school ports needed for easier 
  PortTypeRegistry::value_type ports[] {
      PORT( age, int ),
      PORT( key1, int ),
      PORT( key2, int ),
      PORT( key3, int ),
  };

  int numPorts{ sizeof( ports ) / sizeof( ports[0] ) };
}
