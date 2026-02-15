#include <bicycle/Blackboard.h>
#include "bb/example-blackboards.h"

using BbMap = std::map<std::string, Blackboard>;
using BbMapPtr = std::shared_ptr<BbMap>;

extern "C" {
  inline auto blackboards = std::make_shared<BbMap> (
    std::initializer_list<BbMap::value_type> {
      BB( bb1 ),
      BB( bb2 ),
      BB( bb3 ),
    }
  );
}
