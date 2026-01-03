#include "Config.h"
#include "GameState.h"

Config::Config () : _root( CONFIG_FILEPATH.data() ) {
  auto& gs = GameState::getInstance();
  auto i = _root.readRequired<int>( "there" );
  auto h = _root.readOptional<std::string>( "there" );
  if ( h.has_value() ) {
    std::cout << "h has value " << *h << "\n";
  }
  else {
    std::cout << "h has no value\n";
  }
  auto j = _root.readRequired<std::string>( "there" );
  auto k = _root.readOptional<int>( "hi" );
  if ( k.has_value() ) {
    std::cout << "k has value " << *k << "\n";
  }
  else {
    std::cout << "k has no value\n";
  }
  auto child = _root.getChild("a");
  auto l = child.readRequired<int>("b");
  auto m = child.readRequired<std::vector<int>>("c");
  for (const auto& _m : m ) {
    std::cout << _m << ", ";
  }
  std::cout << "\n";
  std::cout << "I read " << i << "\n";
}
