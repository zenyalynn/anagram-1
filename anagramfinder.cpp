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

using namespace std;

vector<string> dict;
const int prime[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101};

int get_value(string current){
    int value = 1; 
    // for(unsigned int i = 0; i < current.length(); i++){
    //     char temp = current[i];
    //     value = value * prime[current];
    // }
    return value; 

}

string sorter(string in ){
    sort(in.begin(), in.end()); 
    return in; 
}

vector<vector<string>> find_anagram(){
    vector<vector<string>> words;
    unordered_map< string, vector<string>> mapping; 
    for (unsigned int i = 0; i < dict.size(); i++){
        string word = dict[i];
        //cout << i << " " << word << endl;
        string sorted = sorter(word);
        cout << sorted << endl;
        mapping[sorted].push_back(word);
    }  
    for (auto it : mapping){
        words.push_back(it.second);
    }    
    cout << words[0][0] << endl;
    return words; 
}

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


}