#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <cmath>   
#include <sstream> 
#include <unordered_map>
using namespace std;


// Structure to represent a recommendation
struct Recommendation
{
    string title;
    double distance;
};

// Function to search for a movie by name
string search_movie(const vector<vector<string>> &movies, const string &movie_name)
{
    for (int i = 1; i < movies.size(); i++)
    {
        if (movies[i][1].find(movie_name) != string::npos)
        {
            return movies[i][0];
        }
    }
    return "-1"; // Movie not found
}


// Function to compute cosine similarity
double cosine_similarity(const vector<double> &vec1, const vector<double> &vec2)
{
    double dot_product = 0.0, norm1 = 0.0, norm2 = 0.0;
    for (size_t i = 0; i < vec1.size(); ++i)
    {
        dot_product += vec1[i] * vec2[i];
        norm1 += vec1[i] * vec1[i];
        norm2 += vec2[i] * vec2[i];
    }
    return dot_product / (sqrt(norm1) * sqrt(norm2));
}

// Function to split a string by a delimiter
vector<string> split_string(const string &str, char delimiter)
{
    vector<string> tokens;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

// Function to create CSR-like data
vector<vector<int>> convert_to_csr(const vector<vector<string>> &movie_data)
{
    unordered_map<string, int> tag_to_index; // Map from tag to column index
    int tag_counter = 0;

    // Step 1: Extract unique tags and assign indices
    for (const auto &movie : movie_data)
    {
        if (movie.size() < 3)
            continue;                                      // Ensure the movie row has all three columns
        vector<string> tags = split_string(movie[2], '|'); // Tags are separated by '|'
        for (const string &tag : tags)
        {
            if (tag_to_index.find(tag) == tag_to_index.end())
            {
                tag_to_index[tag] = tag_counter++;
            }
        }
    }

    // Step 2: Create CSR-like matrix
    vector<vector<int>> csr_data(movie_data.size(), vector<int>(tag_counter, 0));
    for (size_t i = 0; i < movie_data.size(); ++i)
    {
        if (movie_data[i].size() < 3)
            continue;                                              // Ensure the movie row has all three columns
        vector<string> tags = split_string(movie_data[i][2], '|'); // Tags are separated by '|'
        for (const string &tag : tags)
        {
            if (tag_to_index.find(tag) != tag_to_index.end())
            {
                int col_index = tag_to_index[tag];
                csr_data[i][col_index] = 1; // Set 1 for this tag
            }
        }
    }

    return csr_data;
}


// Function to get recommendations based on KNN
vector<Recommendation> get_recommendation(const string &movie_name,
                                          const vector<vector<string>> &movies,
                                          const vector<vector<double>> &csr_data)
{
    vector<Recommendation> recommendations;

    // Search for the movie
    string x = search_movie(movies, movie_name);
    int movie_idx = stoi(x);
    if (movie_idx == -1)
    {
        cout << "Movie not found..." << endl;
        return recommendations;
    }

    // Get the vector for the movie
    const vector<double> &movie_vector = csr_data[movie_idx];

    // Compute distances to all other movies
    vector<pair<int, double>> distances;
    for (size_t i = 0; i < csr_data.size(); ++i)
    {
        if (i == movie_idx)
            continue; // Skip the input movie
        double dist = cosine_similarity(movie_vector, csr_data[i]);
        distances.emplace_back(i, dist);
    }

    // Sort by distance in descending order
    sort(distances.begin(), distances.end(), [](const pair<int, double> &a, const pair<int, double> &b)
         {
             return a.second > b.second; // Descending order
         });

    // Get top 10 recommendations
    for (size_t i = 0; i < 10 && i < distances.size(); ++i)
    {
        int idx = distances[i].first;
        recommendations.push_back({movies[idx][1], distances[i].second});
    }

    return recommendations;
}