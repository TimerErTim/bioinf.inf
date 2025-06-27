#include "pattern_search.h"
#include <vector>
#include <algorithm>

// Brute-force search implementation
long long brute_search::search(
    const std::string& text,
    const std::string& pattern,
    data_collector& collector,
    size_t start_pos) {
    
    if (pattern.empty() || text.length() < pattern.length()) {
        return -1;
    }

    size_t n = text.length();
    size_t m = pattern.length();

    for (size_t i = start_pos; i <= n - m; ++i) {
        size_t j = 0;
        while (j < m) {
            collector.record_comparison(i + j, text[i + j] == pattern[j]);
            if (text[i + j] != pattern[j]) {
                break;
            }
            j++;
        }
        if (j == m) {
            return i; // Pattern found
        }
    }

    return -1; // Pattern not found
}


// Boyer-Moore search implementation
namespace { // Anonymous namespace for helper functions
    void build_bad_char_table(const std::string& pattern, std::vector<int>& table) {
        table.assign(256, -1);
        for (size_t i = 0; i < pattern.length(); ++i) {
            table[static_cast<unsigned char>(pattern[i])] = i;
        }
    }
}

long long bm_search::search(
    const std::string& text,
    const std::string& pattern,
    data_collector& collector,
    size_t start_pos) {
    
    size_t m = pattern.length();
    size_t n = text.length();

    if (m == 0 || n < m) {
        return -1;
    }

    std::vector<int> bad_char_table;
    build_bad_char_table(pattern, bad_char_table);

    size_t s = start_pos; // s is the shift of the pattern with respect to text
    while (s <= (n - m)) {
        int j = m - 1;

        while (j >= 0) {
            collector.record_comparison(s + j, text[s + j] == pattern[j]);
            if (pattern[j] != text[s + j]) {
                break;
            }
            j--;
        }

        if (j < 0) {
            return s; // Pattern found
        } else {
            // Shift the pattern so that the bad character in text aligns with the last
            // occurrence of it in pattern. The max function is used to make sure
            // that we get a positive shift. We may get a negative shift if the last
            // occurrence of bad character in pattern is on the right side of the
            // current character.
            s += std::max(1, j - bad_char_table[static_cast<unsigned char>(text[s + j])]);
        }
    }

    return -1; // Pattern not found
}


// Knuth-Morris-Pratt search implementation
namespace { // Anonymous namespace for helper functions
    void compute_lps_array(const std::string& pattern, std::vector<int>& lps) {
        int length = 0; // length of the previous longest prefix suffix
        lps[0] = 0; // lps[0] is always 0

        int i = 1;
        while (i < pattern.length()) {
            if (pattern[i] == pattern[length]) {
                length++;
                lps[i] = length;
                i++;
            } else {
                if (length != 0) {
                    length = lps[length - 1];
                } else {
                    lps[i] = 0;
                    i++;
                }
            }
        }
    }
}

long long kmp_search::search(
    const std::string& text,
    const std::string& pattern,
    data_collector& collector,
    size_t start_pos) {

    size_t m = pattern.length();
    size_t n = text.length();

    if (m == 0 || n < m) {
        return -1;
    }

    std::vector<int> lps(m);
    compute_lps_array(pattern, lps);

    size_t i = start_pos; // index for text
    int j = 0; // index for pattern

    while (i < n) {
        collector.record_comparison(i, text[i] == pattern[j]);
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }

        if (j == m) {
            return i - j; // Found pattern at index i - j
        } else if (i < n && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i = i + 1;
            }
        }
    }

    return -1;
} 