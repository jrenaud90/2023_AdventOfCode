#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(){

    // Load input file 
    fstream input_file;
    string line;

    input_file.open("input.txt", ios::in);
    if (input_file.is_open()){
        // Go line by line
        while(getline(input_file, line)){

        }

        input_file.close();
    }

    return 0;
}