#pragma once
#include <string>
#include <fstream>
using namespace std;

bool ignorecase(const string& a, const string& b);
bool is_space(char c);


class CSVParser{

private:

    void parseCSVLine(string line, string* fields, int& fieldCount);
    string trim(string str);
    
    
public:

    void split_bycomma(string str, char comma, string*& result, int& count);
    string parse_field(string& line, int& pos);
    
    

};
