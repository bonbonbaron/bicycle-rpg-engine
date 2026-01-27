#include <bicycle/Grid.h>
#include <bicycle/bicycle.h>

int main() {
  bicycle::init();
  bicycle::push<Grid>( "g1" );
  return bicycle::run();
}
