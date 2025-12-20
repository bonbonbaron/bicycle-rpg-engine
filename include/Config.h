#include <yaml-cpp/yaml.h>
#include <string_view>
#include <iostream>
#include <cursesw.h>
#include <memory>

class Config {

  static constexpr std::string_view CONFIG_FILEPATH{ "./config/test.yml" };

  public:
  Config();
  private:

  class Node : public YAML::Node {
    public:
      Node( const std::string&& cfgFilepath ) : _filepath( std::make_shared<std::string>( cfgFilepath ) ), YAML::Node( cfgFilepath ){}
      Node() = default;

      void stealFilepath( std::shared_ptr<std::string> filepath ) {
        _filepath = filepath;
      }

      template<typename T>
        auto readRequired( const std::string&& key ) -> T {
          if ( Node node = (*this)[key] ) {
            try {
              node.stealFilepath( _filepath );  // prevents us from having to make an annoying getRoot() function
              return node.as<T>();
            }
            catch ( YAML::Exception& e ) {
              std::cerr << *_filepath << ":" << node.Mark().line() << " couldn't convert to desired type. Details:\n";
              std::cerr << e.what();
              std::cerr << "Exiting...\n";
            }
          }
          else {
            std::cerr << *_filepath << ":" << Mark().line() << " " << this->Tag() << " didn't have expected member " << key << ". Exiting...\n";
            endwin();
            exit(1);
          }
        }  // readRequired()

      template<typename T>
        auto readOptional( const std::string&& key ) -> T {
          if ( Node node = (*this)[key] ) {
            try {
              return node.as<T>();
            }
            catch ( YAML::Exception& e ) {
              std::cerr << *_filepath << ":" << node.Mark().line() << " couldn't convert to desired type. Details:\n";
              std::cerr << e.what();
              std::cerr << "Exiting...\n";
            }
          }
        }  // readOptional()
    private:
      std::shared_ptr<std::string> _filepath;  // If this is empty, this isn't the root node for this file.
  };  // private class Node

  Node _root;
};
