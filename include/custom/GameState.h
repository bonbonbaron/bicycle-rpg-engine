#include <string>
#include <bitset>
#include <memory>
#include "Character.h"

enum class Area { GRASSLANDS, WETLANDS };

class GameState {
  using Party = std::vector<std::shared_ptr<Character>>;
  public:
    static auto getInstance() -> GameState& {
      static GameState state;
      return state;
    }

    auto getArea() -> Area;
    auto getParty() -> Party&;
    auto getMonsters() -> std::vector<Character>;
    void configure();

  private:
    Area _area{};
    std::vector<std::shared_ptr<Character>> _party{};
    std::map<Area, std::vector<Character>> _areaToMonstersMap;
    GameState();
    GameState( const GameState& ) = delete;
    GameState& operator=( const GameState& ) = delete;
};
