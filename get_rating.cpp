#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <unordered_set>
#include <string>
#include <unordered_map>

using namespace std;

// Helper function to compute the dot product of two vectors
double dot_product(const vector<double> &a, const vector<double> &b)
{
    double result = 0.0;
    for (size_t i = 0; i < a.size(); ++i)
    {
        if (std::isnan(a[i]) || std::isnan(b[i]))
        {
            continue; // Skip if either value is NaN
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
        return 0.0; // Return 0 similarity if one of the vectors is zero
    }
    return dot / (norm_a * norm_b);
}

// Function to predict the rating of a movie for a target user
double predict_rating(const vector<vector<double>> &user_movie_matrix,
                      int target_user, int movie_id)
{
    int num_users = user_movie_matrix.size();

    double weighted_sum = 0.0;
    double similarity_sum = 0.0;

    // Iterate over all users to compute weighted average
    for (int other_user = 0; other_user < num_users; ++other_user)
    {
        // Skip if the user is the target user or if the movie is not rated by the other user
        if (other_user == target_user || user_movie_matrix[other_user][movie_id] == 0)
            continue;

        // Compute similarity between target user and other user
        double similarity = cosine_similarity(user_movie_matrix[target_user], user_movie_matrix[other_user]);

        // If similarity is zero or negative, we skip the user
        if (similarity <= 0)
            continue;

        // Add weighted rating to weighted_sum and similarity to similarity_sum
        weighted_sum += user_movie_matrix[other_user][movie_id] * similarity;
        similarity_sum += similarity;
    }

    // Avoid division by zero
    if (similarity_sum == 0)
    {
        return 0.0; // Return a default value if no similar users exist
    }

    // Return the predicted rating as a weighted average
    return weighted_sum / similarity_sum;
}

int main()
{
    // Example movie data (movieId, title, tags)
    unordered_map<int, string> mp_movie;
    mp_movie[1374] = "Movie A";
    mp_movie[2492] = "Movie B";
    mp_movie[5678] = "Movie C";

    unordered_map<string, int> title_of_movie;
    title_of_movie["Movie A"] = 2497;

    // Example user ratings (userId, movieId, rating, timestamp)
    vector<vector<string>> ratings = {
        {"0", "1374", "4.5", "1234567890"},
        {"0", "2492", "3.0", "1234567890"},
        {"1", "1374", "5.0", "1234567890"},
        {"1", "2492", "2.5", "1234567890"},
        {"2", "1374", "3.0", "1234567890"},
        {"2", "2492", "4.0", "1234567890"},
        {"3", "2492", "4.5", "1234567890"},
    };

    // Step 1: Create user-movie rating matrix
    int num_users = 4;     // Example: 4 users
    int num_movies = 5001; // Example: 5001 movies
    vector<vector<double>> user_movie_matrix(num_users, vector<double>(num_movies, 0));

    for (const auto &row : ratings)
    {
        int user_id = stoi(row[0]);
        int movie_id = stoi(row[1]);
        double user_rating = stod(row[2]);
        user_movie_matrix[user_id][movie_id] = user_rating;
    }

    // Predict rating for Movie 2492 (index 2492) for User 0 (index 0)
    int target_user = 0;
    int movie_id = 2492;
    double predicted_rating = predict_rating(user_movie_matrix, target_user, movie_id);

    // Print predicted rating
    cout << "Predicted Rating for User " << target_user + 1 << " on Movie ID " << movie_id << ", Title: " << mp_movie[movie_id] << " is: " << predicted_rating << endl;

    return 0;
}
