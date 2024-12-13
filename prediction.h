#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <cmath>   
#include <sstream> 
#include <unordered_map>
#include "user_similarity.h"
using namespace std;

// Recommend movies for a target user [Maximum top 5]
vector<pair<int, double>> recommend_movies(const vector<vector<double>> &user_movie_matrix, int target_user)
{
    int num_users = user_movie_matrix.size();
    int num_movies = user_movie_matrix[0].size();

    vector<pair<int, double>> similarities;
    for (int other_user = 0; other_user < num_users; ++other_user)
    {
        if (other_user == target_user)
            continue;
        double similarity = cosine_similarity(user_movie_matrix[target_user], user_movie_matrix[other_user]);
        similarities.emplace_back(other_user, similarity);
    }

    sort(similarities.begin(), similarities.end(), [](const pair<int, double> &a, const pair<int, double> &b)
         { return a.second > b.second; });

    vector<pair<int, double>> recommendations;
    unordered_set<int> rated_movies;

    for (int movie_id = 0; movie_id < num_movies; ++movie_id)
    {
        if (user_movie_matrix[target_user][movie_id] != 0)
        {
            rated_movies.insert(movie_id);
        }
    }

    for (const auto &similarity : similarities)
    {
        int other_user = similarity.first;
        double similarity_score = similarity.second;
        if (similarity_score <= 0)
            break;

        for (int movie_id = 0; movie_id < num_movies; ++movie_id)
        {
            if (rated_movies.count(movie_id) == 0 && user_movie_matrix[other_user][movie_id] > 0)
            {
                recommendations.emplace_back(movie_id, user_movie_matrix[other_user][movie_id] * similarity_score);
            }
        }
    }

    sort(recommendations.begin(), recommendations.end(), [](const pair<int, double> &a, const pair<int, double> &b)
         { return a.second > b.second; });

    return vector<pair<int, double>>(recommendations.begin(), recommendations.begin() + min(5, (int)recommendations.size()));
}