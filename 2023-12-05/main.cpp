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

struct farmMapper {
    uint32_t source;
    uint32_t dest;
    uint32_t delta;

    farmMapper(uint32_t _dest, uint32_t _source, uint32_t _delta) : 
            dest(_dest),
            source(_source),
            delta(_delta)
        {
        }
};

uint32_t get_dest(vector<farmMapper*>* map_ptr, uint32_t source_val){

    uint32_t diff;
    bool found = false;

    for (size_t i = 0; i < map_ptr->size(); i++)
    {
        // Get the source difference.
        diff = source_val - map_ptr->at(i)->source;

        if ((diff >= 0) && (diff <= map_ptr->at(i)->delta))
        {
            // Source value is within the mapping's source range; proceed with mapping.
            found = true;
            return map_ptr->at(i)->dest + diff;
        }
    }

    if (!found)
    {
        // Source value is not within the mapping's source range; return same value.
        return source_val;
    }
}

int main(){

    // Load input file 
    fstream input_file;
    string line;
    uint32_t current_line_no;

    vector<string> line_split;

    vector<uint32_t> seeds;
    vector<uint32_t> new_dests;
    vector<uint32_t> last_dests;
    vector<uint32_t>* source_to_use;

    uint32_t current_section = 0;
    uint32_t seed_to_soil_lno = 2;     // Section 1
    uint32_t soil_to_fert_lno = 24;    // Section 2
    uint32_t fert_to_water_lno = 54;   // Section 3
    uint32_t water_to_light_lno = 88;  // Section 4
    uint32_t light_to_temp_lno = 122;  // Section 5
    uint32_t temp_to_humid_lno = 167;  // Section 6
    uint32_t humid_to_loc_lno = 203;   // Section 7
    uint32_t num_sections = 7;         // Section 8
    uint32_t sections[7] = {
        seed_to_soil_lno, soil_to_fert_lno, fert_to_water_lno, water_to_light_lno, light_to_temp_lno, temp_to_humid_lno,
        humid_to_loc_lno
    };

    vector<farmMapper*> seed_to_soil_mappings;
    vector<farmMapper*> soil_to_fert_mappings;
    vector<farmMapper*> fert_to_water_mappings;
    vector<farmMapper*> water_to_light_mappings;
    vector<farmMapper*> light_to_temp_mappings;
    vector<farmMapper*> temp_to_humid_mappings;
    vector<farmMapper*> humid_to_loc_mappings;
    size_t mapping_size;
    size_t reversed_index;
    vector<farmMapper*>* all_mappings[7] = {
        &seed_to_soil_mappings, &soil_to_fert_mappings, &fert_to_water_mappings, &water_to_light_mappings,
        &light_to_temp_mappings, &temp_to_humid_mappings, &humid_to_loc_mappings
    };
    vector<farmMapper*>* current_mapping;
    farmMapper* current_map;

    uint32_t lowest_loc;
    lowest_loc = numeric_limits<uint32_t>::max();

    // input_file.open("input.txt", ios::in);
    current_line_no = 0;
    input_file.open("input.txt", ios::in);
    if (input_file.is_open()){
        // Go line by line
        while(getline(input_file, line)){
            
            // Read data from file.
            // Get Number of seeds
            if (current_line_no == 0)
            {
                line_split = split(line, ":", true);
                line_split = split(line_split[1], " ", true);
                for (size_t i = 0; i < line_split.size(); i++)
                {
                    seeds.push_back(stoul(line_split[i]));
                }
                
            } else if (line == "")
            {
                // empty line. Set mode to 0.
                current_section = 0;
            } else {
                if (current_section == 0)
                {
                    // Starting a new section.
                    for (size_t i = 0; i < num_sections; i++)
                    {
                        if (current_line_no == sections[i]) {
                            current_section = i + 1;
                        }
                    }
                    
                } else {
                    // Loading in data.
                    // Get current storage mapping
                    current_mapping = all_mappings[current_section - 1];

                    // Load in this line's data into a farmMapper struct.
                    line_split = split(line, " ", true);
                    current_map = new farmMapper(
                        stoul(line_split[0]),
                        stoul(line_split[1]),
                        stoul(line_split[2])
                    );

                    // Store reference to the struct in our mapping vectors.
                    current_mapping->push_back(current_map);
                }
            }
            current_line_no += 1;
        }
        input_file.close();
    }

    // Data is loaded. Begin linking mappings.
    for (size_t i = 0; i < num_sections; i++)
    {
        // Get the source vector.
        if (i == 0)
        {
            // first source is the seeds themselves.
            source_to_use = &seeds;
        } else {
            // Otherwise it was the vector last built by this loop.
            source_to_use = &last_dests; 
        }
        
        // Get the current mapping struct.
        current_mapping = all_mappings[i];

        // Loop through the source to find the correct destinations.
        for (size_t j = 0; j < source_to_use->size(); j++)
        {
            new_dests.push_back(
                get_dest(current_mapping, source_to_use->at(j))
            );
            cout << "SECTION: " << i << ", SRC: " << source_to_use->at(j) << ", DEST: " << new_dests.at(j) << "\n";
        }
        
        // Now that we are done: reset the old destination vector and set its values equal to this loops dest.
        last_dests = new_dests;
        // Clear the new destinations to prepare for next loop.
        new_dests.clear();
    }

    // Loop through the final destination to look for the lowest value.
    cout << "\n\n Beginning Final Location Lookup...\n\n";
    for (size_t i = 0; i < seeds.size(); i++)
    {
        if (last_dests[i] < lowest_loc)
        {
            lowest_loc = last_dests[i];
        }
        cout << "\tSEED: " << seeds[i] << ", LOC: " << last_dests[i] << "\n";
    }
    cout << "LOWEST LOC: " << lowest_loc << "\n";

    // Delete all of our mappings
    for (size_t i = 0; i < num_sections; i++)
    {
        current_mapping = all_mappings[current_section];
        mapping_size = current_mapping->size();
        // Loop through vector in reverse.
        for (size_t j = 0; j < mapping_size; j++)
        {
            reversed_index = (mapping_size - 1) - j;
            current_map = current_mapping->at(reversed_index);
            current_mapping->pop_back();
            free (current_map);
        }
    }
    
    return 0;
}
