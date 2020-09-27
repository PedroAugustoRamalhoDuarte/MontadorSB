#include <string>
#include <regex>
using namespace std;

bool isInteger(const std::string & s){
    return std::regex_match(s, std::regex("-?[0-9]{0,10}"));
}

string toUpperCase(string str) {
    std::transform(str.begin(), str.end(),str.begin(), ::toupper);
    return str;
}