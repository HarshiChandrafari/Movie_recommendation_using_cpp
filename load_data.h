#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

vector<vector<string>> read_movies()
{
    fstream fin;
    fin.open("movies.csv", ios::in);
    if (!fin.is_open())
    {
        cerr << "Error: Could not open file" << endl;
        return {};
    }
    cout << "File movies.csv opened successfully" << endl;
    vector<string> row;
    vector<vector<string>> movies;
    string line, word;


    while (getline(fin, line))
    { 
        row.clear();
        stringstream s(line); 
        while (getline(s, word, ','))
        { 
            row.push_back(word);
        }

        movies.push_back(row); // Add the row to the movies vector
    }
    fin.close();
    return movies;
}

vector<vector<string>> read_ratings()
{
    // File pointer
    fstream fin;

    // Open an existing file
    fin.open("ratings.csv", ios::in);
    if (!fin.is_open())
    {
        cerr << "Error: Could not open file" << endl;
        return {};
    }
    cout << "File ratings.csv opened successfully" << endl;
    vector<string> row;
    vector<vector<string>> ratings;
    string line, word;

    while (getline(fin, line))
    { // Read each row
        row.clear();
        stringstream s(line);

        while (getline(s, word, ','))
        { 
            row.push_back(word);
        }

        ratings.push_back(row); 
    }
    fin.close();
    return ratings;
}

