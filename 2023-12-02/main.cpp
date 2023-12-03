#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

#include <algorithm> 
#include <cctype>
#include <locale>

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

    vector<string> game_vector;
    string temp_str;

    vector<string> game_sets;
    vector<string> sub_sets;
    vector<string> num_color;

    uint16_t length;
    uint16_t game_id;

    uint16_t reds;
    uint16_t greens;
    uint16_t blues;

    bool is_impossible;

    uint16_t max_reds;
    uint16_t max_greens;
    uint16_t max_blues;

    max_reds = 12;
    max_greens = 13;
    max_blues = 14;

    uint16_t possible_game_sum;
    possible_game_sum = 0;

    input_file.open("input.txt", ios::in);
    if (input_file.is_open()){
        // Go line by line
        while(getline(input_file, line)){

            // Get game ID
            game_vector = split(line, ":");
            temp_str = game_vector[0];
            temp_str = split(temp_str, " ")[1];
            game_id = stoi(temp_str);

            cout << "\nGame: " << game_id << "\n";

            // Loop through game sets
            temp_str = game_vector[1];
            game_sets = split(temp_str, ";");
            
            is_impossible = false;
            
            for (size_t i = 0; i < game_sets.size(); i++)
            {
                blues = 0;
                reds = 0;
                greens = 0;
                sub_sets = split(game_sets[i], ",");
                for (size_t j = 0; j < sub_sets.size(); j++)
                {
                    temp_str = sub_sets[j];
                    num_color = split(temp_str, " ");
                    temp_str = num_color[1];
                    if (temp_str == "blue"){
                        blues = stoi(num_color[0]);
                        if (blues > max_blues)
                        {
                            is_impossible = true;
                        }
                        
                    } else if (temp_str == "red"){
                        reds = stoi(num_color[0]);
                        if (reds > max_reds)
                        {
                            is_impossible = true;
                        }
                        
                    } else if (temp_str == "green"){
                        greens = stoi(num_color[0]);
                        if (greens > max_greens)
                        {
                            is_impossible = true;
                        }
                        
                    }
                }
                
                cout << game_sets[i] << "\n\tReds: " << reds 
                    << "\n\tGreens: " << greens 
                    << "\n\tBlues: " << blues 
                    << "\n\t\tImpossible: " << is_impossible << "\n";
                if (is_impossible)
                {
                    // Break fast
                    break;
                }
                
            }
            if (!is_impossible) {
                possible_game_sum = possible_game_sum + game_id;
            }
           
        }
        input_file.close();
    }

    cout << "\n\nSum of Possible Game IDs: " << possible_game_sum  << "\n";

    return 0;
}