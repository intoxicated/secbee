#include <iostream>
#include <sstream>

using namespace std;

int main()
{
    unsigned short test = 10;
    char raw[2];
    std::stringstream sstream;
    sstream << std::hex << test;
    std::string result = sstream.str();

    cout<< result << endl;

    memcpy(raw, result.c_str(), 2);

    printf("%s\n", raw);
    return 0;
}
