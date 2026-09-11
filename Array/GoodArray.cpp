/*
Count number of good subarrays.
A good subarray is a subarray, which when removed from array makes it continously
increasing array.

# Example Walkthrough
# Input: [1, 3, 4, 2, 5]
# Output: 10
*/
#include <iostream>
#include <vector>

long long countGoodSubarrays(const std::vector<int>& arr) {
    int n = arr.size();
    if (n <= 1) return n;

    // 1. Find the longest strictly increasing prefix length
    int pref_len = 1;
    while (pref_len < n && arr[pref_len] > arr[pref_len - 1]) {
        pref_len++;
    }

    // Edge case: If the entire array is already strictly increasing
    if (pref_len == n) {
        return (1LL * n * (n + 1)) / 2;
    }

    // 2. Find the longest strictly increasing suffix length
    int suff_len = 1;
    while (suff_len < n && arr[n - 1 - suff_len] < arr[n - suff_len]) {
        suff_len++;
    }

    long long ans = 0;
    int j = suff_len;

    // 3. Two-pointer traversal to count valid removals
    for (int i = 0; i <= pref_len; ++i) {
        // Shrink suffix pointer j while conditions are invalid
        // (i.e., when they overlap or prefix element >= suffix element)
        while (j > 0 && (i + j > n - 1 || (i > 0 && arr[i - 1] >= arr[n - j]))) {
            j--;
        }
        // If prefix i is valid, any suffix size from 0 to j works
        ans += (j + 1);
    }

    return ans;
}

int main() {
    // Example test case
    std::vector<int> arr = {1, 2, 3, 5, 4, 7};
    
    // Expected output: 10
    std::cout << "Number of good subarrays: " << countGoodSubarrays(arr) << std::endl;
    
    return 0;
}
