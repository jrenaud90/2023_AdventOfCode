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

    string str_temp;
    string double_char_p0;
    string double_char_p1;
    string double_char;
    vector<string> string_list;

    uint32_t game_id;
    uint32_t line_length_0 = 30;
    uint32_t line_length_1 = 107;

    vector<uint32_t> winning_numbers;
    vector<uint32_t> my_numbers;

    uint32_t check_my_num;
    uint32_t check_win_num;

    uint32_t current_win_value;
    uint32_t total_win_value;
    uint32_t current_wins;


    total_win_value = 0;

    input_file.open("input.txt", ios::in);
    if (input_file.is_open()){
        // Go line by line
        while(getline(input_file, line)){
            // Reset containers
            winning_numbers.clear();
            my_numbers.clear();

            // Get game number
            string_list = split(line, ":", false);
            str_temp = split(string_list[0], "d", true)[1];
            game_id = stoi(str_temp);

            cout << "\nGame: " << game_id << "\n";

            // Parse through the rest of the string and save winning numbers.
            str_temp = string_list[1];
            for (size_t i = 1; i < line_length_0; i = i + 3)
            {
                double_char_p0 = str_temp.at(i);
                double_char_p1 = str_temp.at(i + 1);
                double_char = double_char_p0 + double_char_p1;
                cout << "\tWN " << double_char << "\n";
                winning_numbers.push_back(stoi(double_char));
            }
            // Now our numbers
            for (size_t i = 33; i < line_length_1; i = i + 3)
            {
                double_char_p0 = str_temp.at(i);
                double_char_p1 = str_temp.at(i + 1);
                double_char = double_char_p0 + double_char_p1;
                cout << "\tMN " << double_char << "\n";
                my_numbers.push_back(stoi(double_char));
            }

            // Check if our numbers are inside the winning numbers
            current_wins = 0;
            for (size_t i = 0; i < my_numbers.size(); i++)
            {
                check_my_num = my_numbers[i];
                for (size_t j = 0; j < winning_numbers.size(); j++)
                {
                    check_win_num = winning_numbers[j];
                    if (check_my_num == check_win_num)
                    {
                        current_wins = current_wins + 1;
                        break;
                    }
                }
            }

            // Convert current wins into a score.
            current_win_value = 0;
            if (current_wins > 0) {
                current_win_value = 1;
            }
            for (size_t i = 1; i < current_wins; i++)
            {
                current_win_value = 2 * current_win_value;
            }
            total_win_value = total_win_value + current_win_value;

            cout << "Wins = " << current_wins << ", Score: " << current_win_value << ", Total Score: " << total_win_value << "\n";
        }

        cout << "Total Score: " << total_win_value;
        input_file.close();
    }

    return 0;
}

