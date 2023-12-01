#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void replaceAll(string& str, const string& from, const string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

int main(){

    // String parameters
    uint16_t line_num;
    uint16_t line_len;
    uint32_t letter_as_int;
    uint32_t number;
    uint32_t sum;

    int last_found;
    
    string line;
    string letter;

    // Possible digit words
    string look_for;
    string convert_to;
    string digit_names[19] = {
        "zerone", "oneight", "twone", "threeight", "fiveight", "sevenine", "eightwo", "eighthree", "nineight",
        "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
        };
    string digit_names_conv[19] = {
        "01", "18", "21", "38", "58", "79", "82", "83", "98",
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"
        };

    // initialize summation
    sum = 0;
    line_num = 1;

    // Load input file 
    fstream input_file;

    input_file.open("calibration_input.txt", ios::in);
    if (input_file.is_open()){
        // Go line by line
        while(getline(input_file, line)){
            // Start by converting any digit words into digits.
            cout << "Line: " << line_num << "\n" << line << "\n";
            line_num = line_num + 1;
            for (size_t i = 0; i < 19; i++)
            {
                look_for = digit_names[i];
                convert_to = digit_names_conv[i];
                replaceAll(line, look_for, convert_to);
            }


            cout << line << "\n";
            number = 0;
            last_found = -1;
            line_len = line.length();
            for (size_t i = 0; i < line_len; i++)
            {
                letter = line[i];
                try {
                    letter_as_int = stoi(letter);
                    if (number == 0){
                        // First digit
                        number = letter_as_int * 10;
                    } else {
                        // Looking for another digit.
                        last_found = letter_as_int;
                    }
                } catch(exception &err) {
                    // Not a digit: Continue..
                }
            }
            // End of line. Add last digit to number and then add number to summation.
            if (last_found == -1) {
                // Only one digit was found. Add the digit back onto itself
                number = number + (number / 10);
            } else {
                number = number + last_found;
            }
            cout << number << "\n" << "\n";
            sum = sum + number;
        }
        input_file.close();
    }

    // Print Result
    cout << "\n" << "\n" << sum << "\n";
}