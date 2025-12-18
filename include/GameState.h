#include <string>
#include <bitset>

class GameState {
  public:
    static auto getInstance() -> GameState& {
      static GameState state;
      return state;
    }

  private:
    GameState();
    GameState( const GameState& ) = delete;
    GameState& operator=( const GameState& ) = delete;
};
