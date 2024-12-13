// #include <iostream>
// #include <vector>
// #include <cmath>
// #include <algorithm>
// #include <unordered_set>
// #include <string>
// #include "load_data.h"
// #include <unordered_map>

// using namespace std;

// // Helper function to compute the dot product of two vectors
// double dot_product(const vector<double> &a, const vector<double> &b)
// {
//     double result = 0.0;
//     for (size_t i = 0; i < a.size(); ++i)
//     {
//         result += a[i] * b[i];
//     }
//     return result;
// }

// // Normalize a vector
// void normalize(vector<double> &vec)
// {
//     double norm = sqrt(dot_product(vec, vec));
//     for (double &val : vec)
//     {
//         val /= norm;
//     }
// }

// // Function to perform SVD on a simple matrix (2D vector)
// void svd(const vector<vector<double>> &matrix,
//          vector<vector<double>> &U,
//          vector<double> &S,
//          vector<vector<double>> &V)
// {
//     int rows = matrix.size();
//     // int cols = matrix[0].size();
//     int cols = 6;

//     U.resize(rows, vector<double>(cols, 0.0)); // Initialize U
//     V.resize(cols, vector<double>(cols, 0.0)); // Initialize V
//     S.resize(cols, 0.0);                       // Singular values

//     for (int k = 0; k < cols; ++k)
//     {
//         vector<double> u_k(rows, 0.0), v_k(cols, 0.0);
//         // Power iteration for singular vector
//         for (int i = 0; i < cols; ++i)
//         {
//             v_k[i] = (i == k) ? 1.0 : 0.0;
//         }

//         for (int iter = 0; iter < 10; ++iter) // Converge the singular vectors
//         {
//             // u_k = M * v_k
//             for (int i = 0; i < rows; ++i)
//             {
//                 u_k[i] = dot_product(matrix[i], v_k);
//             }

//             normalize(u_k);

//             // v_k = M^T * u_k
//             for (int j = 0; j < cols; ++j)
//             {
//                 double sum = 0.0;
//                 for (int i = 0; i < rows; ++i)
//                 {
//                     sum += matrix[i][j] * u_k[i];
//                 }
//                 v_k[j] = sum;
//             }
//             normalize(v_k);
//         }

//         // Compute singular value S[k]
//         double singular_value = 0.0;
//         for (int i = 0; i < rows; ++i)
//         {
//             singular_value += u_k[i] * dot_product(matrix[i], v_k);
//         }

//         S[k] = singular_value;
//         U[k] = u_k;
//         V[k] = v_k;
//     }

//     cout << "SVD Completed" << endl;
//     return;
// }

// // Recommend movies for a target user
// vector<pair<int, double>> recommend_movies(const vector<vector<double>> &user_movie_matrix,
//                                            const vector<vector<double>> &U,
//                                            int target_user)
// {
//     int num_users = U.size();
//     int num_movies = user_movie_matrix[0].size();

//     // Step 1: Compute similarity between the target user and all other users
//     vector<pair<int, double>> similarities;
//     for (int other_user = 0; other_user < num_users; ++other_user)
//     {
//         if (other_user == target_user)
//             continue;
//         double similarity = dot_product(U[target_user], U[other_user]);
//         similarities.emplace_back(other_user, similarity);
//     }

//     // Sort by similarity in descending order
//     sort(similarities.begin(), similarities.end(), [](const pair<int, double> &a, const pair<int, double> &b)
//          { return a.second > b.second; });

//     // Step 2: Recommend movies based on highly similar users
//     vector<pair<int, double>> recommendations;
//     unordered_set<int> rated_movies;
//     for (int movie_id = 0; movie_id < num_movies; ++movie_id)
//     {
//         if (user_movie_matrix[target_user][movie_id] != 0)
//         {
//             rated_movies.insert(movie_id); // Movies already rated by the target user
//         }
//     }

//     for (auto it = similarities.begin(); it != similarities.end(); ++it)
//     {
//         int other_user = it->first;
//         double similarity = it->second;
//         if (similarity <= 0)
//             break; // Skip users with zero or negative similarity

//         for (int movie_id = 0; movie_id < num_movies; ++movie_id)
//         {
//             if (rated_movies.count(movie_id) == 0 && user_movie_matrix[other_user][movie_id] > 0)
//             {
//                 recommendations.emplace_back(movie_id, user_movie_matrix[other_user][movie_id] * similarity);
//             }
//         }
//     }

//     // Sort recommendations by predicted score in descending order
//     sort(recommendations.begin(), recommendations.end(), [](const pair<int, double> &a, const pair<int, double> &b)
//          { return a.second > b.second; });

//     // Return up to 5 recommendations
//     return vector<pair<int, double>>(recommendations.begin(), recommendations.begin() + min(5, (int)recommendations.size()));
// }

// int main()
// {
//     // Example movie data (movieId, title, tags)

//     vector<vector<string>> movies = read_movies();

//     unordered_map<int, string> mp_movie;

//     for (int i = 1; i < movies.size(); i++)
//     {
//         int x = stoi(movies[i][0]);
//         if (x > 5000)
//             continue;
//         mp_movie[x] = movies[i][1];
//         }

//             // Example user ratings (userId, movieId, rating, timestamp)
//             vector<vector<string>> ratings = read_ratings();

//         // Step 1: Create user-movie rating matrix
//         int num_users = 610; // Example: 3 users
//         int num_movies = 5001;
//         vector<vector<double>> user_movie_matrix(num_users, vector<double>(num_movies, 0));

//         for (int row = 100;row<200;row++)
//         {
//             int user_id = stoi(ratings[row][0]); // Convert to 0-based index
//             int movie_id = stoi(ratings[row][1]);    // Use movieId directly
//             if (movie_id>5000) continue;
//             double user_rating = stod(ratings[row][2]);
//             user_movie_matrix[user_id][movie_id] = user_rating;
//         }

//         vector<vector<double>> U, V;
//         vector<double> S;

//         // Perform SVD
//         svd(user_movie_matrix, U, S, V);

//         // Recommend movies for User 1 (index 0)
//         int target_user = 3;
//         vector<pair<int, double>> recommendations = recommend_movies(user_movie_matrix, U, target_user);

//         // Print recommendations
//         cout << "Top Recommendations for User " << target_user + 1 << ":\n";
//         for (const auto &rec : recommendations)
//         {
//             string title = mp_movie[rec.first];
//             cout << "Movie ID: " << rec.first << ", Title: "<< title << ", Predicted Score: " << rec.second << endl;
//     }

//     return 0;
// }

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <unordered_set>
#include <string>
#include "load_data.h"
#include <unordered_map>

using namespace std;
//  Helper function to compute the dot product of two vectors 
double dot_product(const vector<double> &a, const vector<double> &b)
{
    double result = 0.0;
    for (size_t i = 0; i < a.size(); ++i)
    {
        // Handle NaN values
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

// Recommend movies for a target user
vector<pair<int, double>> recommend_movies(const vector<vector<double>> &user_movie_matrix,
                                           int target_user)
{
    int num_users = user_movie_matrix.size();
    int num_movies = user_movie_matrix[0].size();

    // Step 1: Compute similarity between the target user and all other users
    vector<pair<int, double>> similarities;
    for (int other_user = 0; other_user < num_users; ++other_user)
    {
        if (other_user == target_user)
            continue;
        double similarity = cosine_similarity(user_movie_matrix[target_user], user_movie_matrix[other_user]);
        similarities.emplace_back(other_user, similarity);
    }

    // Sort by similarity in descending order
    sort(similarities.begin(), similarities.end(), [](const pair<int, double> &a, const pair<int, double> &b)
         { return a.second > b.second; });

    // Step 2: Recommend movies based on highly similar users
    vector<pair<int, double>> recommendations;
    unordered_set<int> rated_movies;

    // Track movies already rated by the target user
    for (int movie_id = 0; movie_id < num_movies; ++movie_id)
    {
        if (user_movie_matrix[target_user][movie_id] != 0)
        {
            rated_movies.insert(movie_id); // Movies already rated by the target user
        }
    }

    // Go through the most similar users and recommend unwatched movies
    for (const auto &similarity : similarities)
    {
        int other_user = similarity.first;
        double similarity_score = similarity.second;
        if (similarity_score <= 0)
            break; // Skip users with zero or negative similarity

        for (int movie_id = 0; movie_id < num_movies; ++movie_id)
        {
            // Only recommend movies that the target user has not rated
            if (rated_movies.count(movie_id) == 0 && user_movie_matrix[other_user][movie_id] > 0)
            {
                recommendations.emplace_back(movie_id, user_movie_matrix[other_user][movie_id] * similarity_score);
            }
        }
    }

    // Sort recommendations by predicted score in descending order
    sort(recommendations.begin(), recommendations.end(), [](const pair<int, double> &a, const pair<int, double> &b)
         { return a.second > b.second; });

    // Return top 5 recommendations
    return vector<pair<int, double>>(recommendations.begin(), recommendations.begin() + min(5, (int)recommendations.size()));
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

    // Step 1: Create user-movie rating matrix
    int num_users = 611; // Example: 3 users
    int num_movies = 5001;
    vector<vector<double>> user_movie_matrix(num_users, vector<double>(num_movies, 0));

    for (int row = 1; row < ratings.size(); row++)
    {
        int user_id = stoi(ratings[row][0]);  // Convert to 0-based index
        int movie_id = stoi(ratings[row][1]); // Use movieId directly
        if (movie_id > 5000)
            continue;
        double user_rating = stod(ratings[row][2]);
        user_movie_matrix[user_id][movie_id] = user_rating;
    }

    // Recommend movies for User 1 (index 0)
    int target_user = 5;
    vector<pair<int, double>> recommendations = recommend_movies(user_movie_matrix,target_user);

    // Print recommendations
    cout << "Top Recommendations for User " << target_user << ":\n";
    cout << recommendations.size();
    for (const auto &rec : recommendations)
    {
        string title = mp_movie[rec.first];
        cout << "Movie ID: " << rec.first << ", Title: " << title << ", Predicted Score: " << rec.second << endl;
    }

    return 0;
}
