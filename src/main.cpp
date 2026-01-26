#include <iostream>
#include <bicycle/Graph.h>
#include <bicycle/ConditionRegistry.h>
#include <bicycle/Blackboard.h>
#include <memory>
#include <map>
#include  <string>
#include <any>

using namespace std;
using enum ActionState;

struct Stats {
  int age;
  int num;
  std::string buddy;
};

struct A {
  int age{38};
  int hp{42};
  int mp{42};
  string favoriteChild{ "Michael" };
};

F_( sayHi, std::cout << "tree1: Hi there!\n"; return SUCCESS; );
F_( sayHo, 
    std::cout << "tree2: Ho there!\n"; 
    auto& c = arg.get<A>("c");
    cout << "my age is " << c.age << "\n";
    return FAILED; );
//F_( sayHe, std::cout << "He there!\n"; arg.get<int>("hello") = 45; return SUCCESS; );
F_( sayHe, std::cout << "tree2: He there!\n"; return SUCCESS; );
F_( chooseTgt, 
    std::cout << "both trees: Thinking...\n"; 
    std::cout << " both trees: Choosing my target\n"; 
    return FAILED; );
F_( eatTgt, std::cout << "both trees: Eating my target\n"; return SUCCESS; );

static void registerPortTypes() {
  PORT( age, int );
  PORT( hi, Stats );
  PORT( hello, int );
  PORT( there, int );
  PORT( time, int );
  PORT( a, int );
  PORT( b, string );
  PORT( c, A );
}

// TODO automate this, perhaps replacing F_ macro with template
static void registerActions() {
  ACT( sayHi, "hello", "there" );
  ACT( sayHo, "c" );
  ACT( sayHe );
  ACT( chooseTgt );
  ACT( eatTgt );
}

static auto imnotgay ()-> bool {
  cout << "I SWEAR TO GAWD\n"; 
  return true; 
}

// TODO automate this, perhaps replacing F_ macro with template
static void registerConditions() {
  COND( imnotgay );
}

static void registerBbs() {
  BB( bb1, 
    { "a", 5 },
    { "b", "someString" },
    { "hello", 45 },
    { "there", 45 },
    { "c", A { 38, 40, 42, string("Charlie") } } );
}

int main() {
  registerPortTypes();
  registerActions();
  registerConditions();
  registerBbs();

  std::string_view fp{ "./config/node/n1.yml" };
  auto root = YAML::LoadFile( fp.data() );
  auto n = root.as<bicycle::Node>();
  n.run();
  auto edges = n.getEdges();
  edges.at( "Closet door" ).loadEndpoint();
  return 0;
}
