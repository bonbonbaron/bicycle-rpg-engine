#include <bicycle/Grid.h>
#include <bicycle/Dialogue.h>
#include <bicycle/Menu.h>

#include <bicycle/bicycle.h>

#include "Config.h"

int main() {
  config();
  bicycle::init();
  bicycle::push<Grid>( "g1" );
  bicycle::push<Dialogue>( "hello there. My name is Michael. What's your naem?", 25, 15, 10, 10 );
  bicycle::push<Menu>( "m1" );
  return bicycle::run();
}
