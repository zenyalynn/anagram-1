/*******************************************************************************
 * Name        : anagramfinder.cpp
 * Author      : Zenya Koprowski 
 * Version     : 1.0
 * Date        : 5/11/2020
 * Description : Find the largest word that has one or more anagrams present.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <algorithm>
#include <bits/stdc++.h>
#include <unordered_map>
#include <stdio.h>
#include <ctype.h>

using namespace std;

vector<string> dict;
vector<vector<string>> words;
vector<vector<string>> finaloutput; 

string sorter( string in ){ 
    //sorter takes in a string and returns it alphabetically with no capitals, hyphens, or apostrophe's
    for( unsigned int i = 0; i < in.length(); i++ ){ 
        //puts everything to lower case
        int c = in[i];
        in[i] = tolower(c);
    }
    string chars = "-'";
    for (char c: chars) { 
        // gets rid of hyphens and apostrophe's
		in.erase(remove(in.begin(), in.end(), c), in.end());
	}
    sort(in.begin(), in.end()); 
    //sorts the words alphabetically
    return in; 
}

void find_anagram(){
    unordered_map< string, vector<string>> mapping; 
    for (unsigned int i = 0; i < dict.size(); i++){
        string word = dict[i];
        //cout << i << " " << word << endl;
        string sorted = sorter(word);
        //cout << sorted << endl;
        mapping[sorted].push_back(word);
    } 
    for (auto pair: mapping) {
        std::cout << "{ " << pair.first << ", "<< pair.second << " }" << endl;
    } 
    for (auto it : mapping){
        words.push_back(it.second);
    }    
    
}
void find_max() {
    // finds the max length of anagrams by looping through 'words' and pushing them onto finaloutput
    unsigned int max = 0; 
    for( unsigned int i = 0; i < words.size(); i++){
        //loops through the whole vector until it reaches its size
        vector<string> temp = words.at(i);
        // cout << i << " " << temp[0] << endl;
        if(temp.size() == max){
            // here you found another words with your max length and push it onto the final output
            finaloutput.push_back(temp);
        } 
        if(temp.size() > max){
            // here you found a new long word so you clear the old list and start adding the new words
            max = temp.size(); 
            finaloutput.clear(); 
            finaloutput.push_back(temp);
        }
    }
    cout << "Max is: " << max << endl; 
}

// void alphabetize() {

// }

bool load_values_from_file(const string &filename) {
    ifstream input_file(filename.c_str());
    if (!input_file) {
        cerr << "Error: File '" << filename << "' not found." << endl;
        return false;
    }
    input_file.exceptions(ifstream::badbit);
    string line;
    try {
        while (getline(input_file, line)) {
            dict.push_back(line);
        }
        input_file.close();
    } catch (const ifstream::failure &f) {
        cerr << "Error: An I/O error occurred reading '" << filename << "'.";
        return false;
    }
    
    if (dict.size() == 0){
        cerr << "No anagrams found." << endl;
        return false;
    }
    return true;
}

int main(int argc, char * const argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <dictionary file>" << endl;
        return 1;
    }

    string filename(argv[1]);
    if (!load_values_from_file(filename)) {
        return 1;
    }

    find_anagram();
    find_max();


}