#pragma GCC optimize("O3")
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cmath>
#include <limits>

using namespace std;

/*
*
*
Check arbitrage before getting best conversion rate
*
*
*/



// Structure to hold individual directed conversion pairs
struct ConversionEdge {
    int from;
    int to;
    double logWeight;
    double originalRate;
};

void detectArbitrageAndMaximize(
    const vector<pair<pair<string, string>, double>>& rates, 
    const string& source, 
    const string& target
) {
    unordered_map<string, int> currencyMap;
    vector<string> currencyName; // Inverse map to print names later
    int totalCurrencies = 0;

    // Step 1: Map unique currency strings to IDs
    for (const auto& entry : rates) {
        string from = entry.first.first;
        string to = entry.first.second;
        if (currencyMap.find(from) == currencyMap.end()) {
            currencyMap[from] = totalCurrencies++;
            currencyName.push_back(from);
        }
        if (currencyMap.find(to) == currencyMap.end()) {
            currencyMap[to] = totalCurrencies++;
            currencyName.push_back(to);
        }
    }

    if (currencyMap.find(source) == currencyMap.end()) {
        cout << "Source currency not found in trading pairs." << endl;
        return;
    }

    int sourceId = currencyMap[source];
    
    // Step 2: Build flattened edge list
    vector<ConversionEdge> edges;
    for (const auto& entry : rates) {
        int u = currencyMap[entry.first.first];
        int v = currencyMap[entry.first.second];
        double rate = entry.second;

        // Weight is -log(rate)
        edges.push_back({u, v, -log(rate), rate});
        edges.push_back({v, u, -log(1.0 / rate), 1.0 / rate});
    }

    // Step 3: Bellman-Ford execution arrays
    vector<double> dist(totalCurrencies, numeric_limits<double>::infinity());
    vector<double> actualRateToNode(totalCurrencies, 0.0);
    
    dist[sourceId] = 0.0;
    actualRateToNode[sourceId] = 1.0;

    // Relax all edges V - 1 times
    for (int i = 0; i < totalCurrencies - 1; ++i) {
        for (const auto& edge : edges) {
            if (dist[edge.from] != numeric_limits<double>::infinity()) {
                if (dist[edge.from] + edge.logWeight < dist[edge.to]) {
                    dist[edge.to] = dist[edge.from] + edge.logWeight;
                    actualRateToNode[edge.to] = actualRateToNode[edge.from] * edge.originalRate;
                }
            }
        }
    }

    // Step 4: Run the V-th relaxation pass to check for Arbitrage Loops
    bool arbitrageDetected = false;
    for (const auto& edge : edges) {
        if (dist[edge.from] != numeric_limits<double>::infinity()) {
            if (dist[edge.from] + edge.logWeight < dist[edge.to]) {
                arbitrageDetected = true;
                cout << "⚠️ ARBITRAGE DETECTED! Infinite profit cycle found involving: " 
                     << currencyName[edge.from] << " -> " << currencyName[edge.to] << endl;
                break; 
            }
        }
    }

    // Step 5: Process output decisions
    if (arbitrageDetected) {
        cout << "Profit is infinitely scalable. Standard max path calculation aborted due to infinite loop loop risk." << endl;
    } else {
        if (currencyMap.find(target) == currencyMap.end() || dist[currencyMap[target]] == numeric_limits<double>::infinity()) {
            cout << "Target currency " << target << " is completely unreachable." << endl;
        } else {
            int targetId = currencyMap[target];
            cout << "✅ No arbitrage risks found." << endl;
            cout << "Maximum achievable conversion rate from " << source << " to " << target 
                 << " is: " << actualRateToNode[targetId] << endl;
        }
    }
}

int main() {
    // Normal Graph Test (No Arbitrage)
    vector<pair<pair<string, string>, double>> safeRates = {
        {{"USD", "EUR"}, 0.90},
        {{"EUR", "GBP"}, 0.85},
        {{"USD", "GBP"}, 0.75}
    };
    cout << "--- Running Safe Dataset ---" << endl;
    detectArbitrageAndMaximize(safeRates, "USD", "GBP");

    // Arbitrage Loop Graph Test (USD -> EUR -> JPY -> USD returns 1.04x profit multiplier)
    vector<pair<pair<string, string>, double>> arbitrageRates = {
        {{"USD", "EUR"}, 0.90},
        {{"EUR", "JPY"}, 130.0},
        {{"JPY", "USD"}, 0.0089} // 0.90 * 130.0 * 0.0089 = 1.0413 (> 1.0)
    };
    cout << "\n--- Running Arbitrage Dataset ---" << endl;
    detectArbitrageAndMaximize(arbitrageRates, "USD", "JPY");

    return 0;
}
