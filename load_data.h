#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

vector<vector<string>> read_movies()
{
    // File pointer
    fstream fin;

    // Open an existing file
    fin.open("movies.csv", ios::in);
    if (!fin.is_open())
    {
        cerr << "Error: Could not open file" << endl;
        return {};
    }
    cout << "File opened successfully" << endl;

    // Read the Data from the file as String Vector
    vector<string> row;
    vector<vector<string>> movies;
    string line, word;


    while (getline(fin, line))
    { // Read each row
        row.clear();
        stringstream s(line); // Parse the line into words

        while (getline(s, word, ','))
        { // Extract words separated by commas
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
    cout << "File opened successfully" << endl;

    // Read the Data from the file as String Vector
    vector<string> row;
    vector<vector<string>> ratings;
    string line, word;

    while (getline(fin, line))
    { // Read each row
        row.clear();
        stringstream s(line); // Parse the line into words

        while (getline(s, word, ','))
        { // Extract words separated by commas
            row.push_back(word);
        }

        ratings.push_back(row); // Add the row to the movies vector
    }
    fin.close();
    return ratings;
}

// Function that returns users x movies matrix
vector<vector<string>>get_data(){
    cout << "Creating data matrix" << endl;
    vector<vector<string>>  ratings = read_ratings();
    vector<vector<string>> movies = read_movies();
    int users = 611;          // seen in data;
    int no_of_movies = 50000; // we are choosing movies only upto this id due to memory issues

    vector<vector<string>> data(users, vector<string>(no_of_movies+1, "0")); // users vs movieID;

    for (int i = 1; i<ratings.size() ;i++){
        // data[userID][movieID] = rating given
        try
        {
            int userID = stoi(ratings[i][0]);  // Convert userID
            int movieID = stoi(ratings[i][1]); // Convert movieID
            string rating = ratings[i][2];     // Get rating as string

            if (userID < 1 || userID > users || movieID < 1 || movieID > no_of_movies)
            {
                continue;
            }

            data[userID][movieID] = rating; // Assign the rating
        }
        catch (const std::exception &e)
        {
            cerr << "Error processing row " << i << ": " << e.what() << endl;
        }
    }

    return data;
}

vector<int> get_user_vector(vector<vector<string>> &data, int movieID)
{

}