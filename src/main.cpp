#include <bicycle/Grid.h>
#include <bicycle/bicycle.h>
#include "Config.h"

int main() {
  config();
  bicycle::init();
  bicycle::push<Grid>( "g1" );
  return bicycle::run();
}
