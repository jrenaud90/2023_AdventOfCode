#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// trim from start (in place)
static inline void ltrim(string &s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void strip(string &s) {
    rtrim(s);
    ltrim(s);
}

vector<string> split(string line, string delim, bool run_strip = true){
    
    size_t location;
    string temp_str;
    string line_copy;
    vector<string> output;
    // Make copy of line so that edits can be made without losing original data.
    line_copy = line;

    // Step through string and store results of split.
    while (true){
        location = line_copy.find(delim);
        if (location == string::npos){
            // No more delims found. Need to store the rest of string as the final value.
            if (run_strip) {
                strip(line_copy);
            }
            output.push_back(line_copy);
            break;
        } else {
            // Add line to output
            temp_str = line_copy.substr(0, location);
            if (run_strip) {
                strip(temp_str);
            }
            output.push_back(temp_str);
            // Erase the string up to this point.
            // The plus one ensures that the delim is removed as well otherwise the while loop would never break.
            line_copy.replace(0, location + 1, "");
        }
    }
    return output;
}

int main(){

    // Load input file 
    fstream input_file;
    string line;

    // input_file.open("input.txt", ios::in);
    input_file.open("input.txt", ios::in);
    if (input_file.is_open()){
        // Go line by line
        while(getline(input_file, line)){
        
        }

        input_file.close();
    }

    return 0;
}

