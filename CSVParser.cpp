#include "CSVParser.h"



bool ignorecase(const string& a, const string& b){
      char ca; 
      char cb;
    if (a.length() != b.length()){
        return false;
    }


    for (size_t i = 0; i < a.length(); i++){
         ca = a[i];
        cb = b[i];

        if (ca >= 'A'&&ca <= 'Z'){
            ca += 32;
         }

        if (cb >= 'A' && cb <='Z'){
            cb += 32;
        }
        
        if (ca!= cb){

            return false;
        }


    }

    return true;


}


bool is_space(char c){

    return (c == ' '|| c == '\t'|| c == '\n'|| c == '\r'|| c == '\f'|| c == '\v');

}

//removes leading and trailing whitespaces
string CSVParser::trim(string str){
    int start ;
    int end;
    string result;

    start= 0;
    end = str.length() - 1;
    
    while (start <= end &&(is_space(str[start]) || str[start] == '\xC2' || str[start] == '\xA0')){
       start++;
    }
    
    while (end >= start && (is_space(str[end]) || str[end] == '\xC2' || str[end] == '\xA0')){
         end--;
    }
    
    if (start > end){
        return "";

    }

    
    for (int i = start; i <= end; i++){
        result += str[i];
    }

    return result;

}


//extracts the next CSV field
string CSVParser::parse_field(string& line, int& position){

    string field;
    bool inquotes;

    inquotes = false;
    field = "";
    
    while (position <line.length()){
        char c = line[position];
        
        if (c== '"'){

            inquotes = !inquotes;
            position++;
            continue;
        }
        
        if (c == ',' &&!inquotes){
            position++; 
            return trim(field);
        }
        
        field += c;
        position++;

    }
    

    return trim(field);

}

// splits actor string using '|' comma

void CSVParser::split_bycomma(string str, char comma, string*& result, int& count){
    int index;
    string current;
    count = 1;

    for (int i = 0; i < str.length(); i++){
        if (str[i] ==comma){  //count the num of ocurrences 
            count++;}
   
    }

    result = new string[count];  //to store the substrings
    

    // extracting substrings
     current = "";
    index = 0;
    
    for (int i = 0; i < str.length(); i++){

        if (str[i] == comma){

            result[index++] =trim(current);//if a comma found put the current substring in the array
            current = "";

        }

        else{
            current += str[i];//otherwise append the character to the current temp var
        
        }

    }


    result[index] = trim(current);  //to store the last substring 


}





