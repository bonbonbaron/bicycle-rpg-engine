#include <bicycle/bicycle.h>
#include "Config.h"

int main() {
  config();
  bicycle::init();
  bicycle::push<Grid>( "g1" );
  bicycle::push<Dialogue>( "Hello there. My name is Michael. What's your name?", 25, 15, 10, 10 );
  bicycle::push<Menu>( "m1" );
  return bicycle::run();
}
