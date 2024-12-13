#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <unordered_set>
#include <string>
#include <unordered_map>
#include "load_data.h"

using namespace std;

double calculate_rmse(const vector<double> &predicted, const vector<double> &actual)
{
    double sum_squared_error = 0.0;
    int count = 0;

    for (size_t i = 0; i < predicted.size(); ++i)
    {
        if (actual[i] > 0)
        {
            sum_squared_error += pow(predicted[i] - actual[i], 2);
            ++count;
        }
    }

    return count > 0 ? sqrt(sum_squared_error / count) : 0.0;
}

// Helper function to compute the dot product of two vectors
double dot_product(const vector<double> &a, const vector<double> &b)
{
    double result = 0.0;
    for (size_t i = 0; i < a.size(); ++i)
    {
        if (std::isnan(a[i]) || std::isnan(b[i]))
        {
            continue;
        }
        result += a[i] * b[i];
    }
    return result;
}

// Function to compute the cosine similarity between two vectors
double cosine_similarity(const vector<double> &a, const vector<double> &b)
{
    double dot = dot_product(a, b);
    double norm_a = sqrt(dot_product(a, a));
    double norm_b = sqrt(dot_product(b, b));
    if (norm_a == 0.0 || norm_b == 0.0)
    {
        return 0.0;
    }
    return dot / (norm_a * norm_b);
}

// Function to predict the rating of a movie for a target user
double predict_rating(const vector<vector<double>> &user_movie_matrix, int target_user, int movie_id)
{
    int num_users = user_movie_matrix.size();

    double weighted_sum = 0.0;
    double similarity_sum = 0.0;

    for (int other_user = 0; other_user < num_users; ++other_user)
    {
        if (other_user == target_user || user_movie_matrix[other_user][movie_id] == 0) continue;
        double similarity = cosine_similarity(user_movie_matrix[target_user], user_movie_matrix[other_user]);
        if (similarity <= 0) continue;
        weighted_sum += user_movie_matrix[other_user][movie_id] * similarity;
        similarity_sum += similarity;
    }

    if (similarity_sum == 0)
    {
        return 0.0;
    }

    return weighted_sum / similarity_sum;
}

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

    int target_user = 101;
    int movie_id = 4378;
    double predicted_rating = predict_rating(user_movie_matrix, target_user, movie_id);
    cout << "Predicted Rating for User " << target_user + 1 << " on Movie ID " << movie_id << ", Title: " << mp_movie[movie_id] << " is: " << round(predicted_rating) << endl;

    vector<double> predicted_ratings;
    vector<double> actual_ratings;
    int count = 0;

    for (int user_id = 1; user_id <= 100; ++user_id)
    {
        for (int movie_id = 1; movie_id <= 1000; ++movie_id)
        {
            if (user_movie_matrix[user_id][movie_id] > 0)
            {
                double predicted = predict_rating(user_movie_matrix, user_id, movie_id);
                predicted_ratings.push_back(predicted);
                actual_ratings.push_back(user_movie_matrix[user_id][movie_id]);
                ++count;
            }
        }
    }

    double rmse = calculate_rmse(predicted_ratings, actual_ratings);
    cout << "Root Mean Square Error (RMSE): " << rmse << endl;

    return 0;
}
