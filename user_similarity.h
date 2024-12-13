#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <unordered_set>
#include <string>
#include <unordered_map>

double dot_product(const vector<double> &a, const vector<double> &b)
{
    double result = 0.0;
    for (size_t i = 0; i < a.size(); ++i)
    {
        if (std::isnan(a[i]) || std::isnan(b[i]))
            continue;
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