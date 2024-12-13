#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <unordered_set>
#include <string>
#include "load_data.h"
#include <unordered_map>
#include "prediction.h"

using namespace std;


int main()
{
    vector<vector<string>> movies = read_movies();
    unordered_map<int, string> mp_movie;

    for (int i = 1; i < movies.size(); i++)
    {
        int x = stoi(movies[i][0]);
        if (x > 5000)
            continue;
        mp_movie[x] = movies[i][1];
    }

    vector<vector<string>> ratings = read_ratings();

    int num_users = 611; 
    int num_movies = 5001;
    vector<vector<double>> user_movie_matrix(num_users, vector<double>(num_movies, 0));

    for (int row = 1; row < ratings.size(); row++)
    {
        int user_id = stoi(ratings[row][0]);  
        int movie_id = stoi(ratings[row][1]); 
        if (movie_id > 5000)
            continue;
        double user_rating = stod(ratings[row][2]);
        user_movie_matrix[user_id][movie_id] = user_rating;
    }

    int target_user = 5;
    vector<pair<int, double>> recommendations = recommend_movies(user_movie_matrix,target_user);

    cout << "Top Recommendations for User " << target_user << ":\n";
    for (const auto &rec : recommendations)
    {
        string title = mp_movie[rec.first];
        cout << "Movie ID: " << rec.first << ", Title: " << title << ", Predicted Score: " << rec.second << endl;
    }

    return 0;
}
