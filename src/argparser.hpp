#include <string>
#include <map>
#include <set>
#include <vector>
#include <tuple>
using namespace std;
struct ArgParser{
  map<string,string> opts;
  vector<string> arguments;
  bool success;
  string err_msg;
  ArgParser(int argc, char * argv[], set<string> options_set){
    set<string> used_opt;
    vector<string> argv_str;
    for(int i = 1; i < argc; i++ ){
      argv_str.emplace_back( argv[i] );
    }
    for(unsigned int i = 0; i < argv_str.size(); i++ ){
      string str = argv_str[i];
      if( str[0] == '-' ){
        if( options_set.find( str.substr(1) ) == options_set.end() ){
          success = false;
          err_msg = "Unknown option : " + str;
          return;
        }
        if( used_opt.find( str.substr(1) ) != used_opt.end() ){
          success = false;
          err_msg = "The same option used more than once : " + str;
          return;
        }
        string oa = "";
        if( i+1 < argv_str.size() && argv_str[i+1][0] != '-' ){
          i++;
          oa = argv_str[i];
        }
        opts[ str.substr(1) ] = oa;
        used_opt.insert( str.substr(1) );
      }
    }
    success = true;
  }
  tuple<bool,string> get_opt( string o ){
    if( opts.find( o ) == opts.end() )
      return tuple<bool,string>(false,"");
    return tuple<bool,string>(true,opts[o]);
  }
};
