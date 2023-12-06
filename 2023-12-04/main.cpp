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
    uint32_t total_win_value = 0;
    uint32_t current_wins;

    size_t scratch_index = 0;
    size_t target_index = 0;
    uint32_t scratch_cards[219] = {0};
    uint32_t total_num_scratch_cards = 0;

    // input_file.open("input.txt", ios::in);
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

            cout << "Game: " << game_id << "\n";

            // Parse through the rest of the string and save winning numbers.
            str_temp = string_list[1];
            for (size_t i = 1; i < line_length_0; i = i + 3)
            {
                double_char_p0 = str_temp.at(i);
                double_char_p1 = str_temp.at(i + 1);
                double_char = double_char_p0 + double_char_p1;
                // cout << "\tWN " << double_char << "\n";
                winning_numbers.push_back(stoi(double_char));
            }
            // Now our numbers
            for (size_t i = 33; i < line_length_1; i = i + 3)
            {
                double_char_p0 = str_temp.at(i);
                double_char_p1 = str_temp.at(i + 1);
                double_char = double_char_p0 + double_char_p1;
                // cout << "\tMN " << double_char << "\n";
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
            for (size_t i = 0; i < current_wins; i++)
            {
                if (i == 0)
                {
                    current_win_value = 1;
                } else {
                    current_win_value *= 2;
                }
                
                // See how many of the following scratch cards
                target_index = (scratch_index + 1) + i;

                cout << "\t\t YY " << target_index << "\n";
                if (target_index < 219)
                {
                    // Each card won gets 1 copy.
                    scratch_cards[target_index] += 1;

                    // And then they get a number of copies for each of _this_ card's copies.
                    scratch_cards[target_index] += scratch_cards[scratch_index];
                }
            }
            // Part 1 win value
            // total_win_value = total_win_value + current_win_value;
            total_num_scratch_cards += 1 + scratch_cards[scratch_index];


            cout << "\tWins = " << current_wins << ", Score: " << current_win_value << ", Scratch Copies: " << scratch_cards[scratch_index] << "+1, Total Score: " << total_num_scratch_cards << "\n";

            // Update scratch card index
            scratch_index++;
        }

        cout << "Total Score: " << total_num_scratch_cards;
        input_file.close();
    }

    return 0;
}

