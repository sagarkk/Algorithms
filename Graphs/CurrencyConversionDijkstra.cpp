#pragma GCC optimize("O3")
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <cmath>
#include <limits>

using namespace std;

/*
*
*
Works only if no arbitrage oppurtunity available
*
*
*/

struct Edge{
    int to;
    double logWeight;
    double originalRate;
};

struct NodeState{
    double cumulativeLogWeight;
    int currencyId;
    double actualRate;

    bool operator>(const NodeState& other) const{
        return cumulativeLogWeight > other.cumulativeLogWeight;
    }
};

double shortestPathCurrencyConversion(const vector<pair<pair<string, string>, double>>& rates, const string& source, const string& target){
    
    unordered_map<string, int> currencyMap;
    int totalCurrencies = 0;

    for(const auto& entry : rates){
        string from = entry.first.first;
        string to = entry.first.second;
        if(currencyMap.find(from) == currencyMap.end()) currencyMap[from] = totalCurrencies++;
        if(currencyMap.find(to) == currencyMap.end()) currencyMap[to] = totalCurrencies++;
    }

    if(currencyMap.find(source)==currencyMap.end() || currencyMap.find(target)==currencyMap.end()) return -1.0;

    int sourceId = currencyMap[source];
    int targetId = currencyMap[target];

    vector<vector<Edge>> graph(totalCurrencies);
    for(const auto& entry : rates){
        int u = currencyMap[entry.first.first];
        int v = currencyMap[entry.first.second];
        double rates = entry.second;

        double logWeight = -log(rate);
        //Add forward rate transformation
        graph[u].push_back({v, logWeight, rate});
        //Add reciprocal reverse path implicitly
        graph[v].push_back({u, -logWeight, 1.0 / rate});
    }
    
    priority_queue<NodeState, vector<NodeState>, greater<NodeState>> minHeap;
    vector<double> bestLogWeights(totalCurrencies, std::numeric_limits<double>::infinity());

    minHeap.push({0.0, sourceId, 1.0});
    bestLogWeights[sourceId] = 0.0;

    while(!minHeap.empty()){
        NodeState current = minHeap.top();
        minHeap.pop();

        int currId = current.currencyId;
        double currLogWeight = current.cumulativeLogWeight;
        double currRate = current.actualRate;

        if(currId == targetId) return currRate;

        // Skip stale states if a shorter path to this node was already processed
        if(currLogWeight > bestLogWeights[currId]) continue;

        // Explore adjacent currency pairing
        for(const auto& edge: graph[currId]){
            double nextLogWeight = currLogWeight + edge.logWeight;
            
            if(nextLogWeight < bestLogWeights[edge.to]){
                bestLogWeights[edge.to] = nextLogWeight;
                minHeap.push({nextLogWeight, edge.to, currRate * edge.originalRate});
            }
        }
    }
    return -1.0;
}

int main() {
    // Format: { {From, To}, Rate }
    vector<pair<pair<string, string>, double>> exchangeRates = {
        {{"USD", "EUR"}, 0.92},
        {{"EUR", "GBP"}, 0.85},
        {{"USD", "GBP"}, 0.75}
    };

    string fromCurrency = "USD";
    string toCurrency = "GBP";

    double result = shortestPathCurrencyConversion(exchangeRates, fromCurrency, toCurrency);
    
    cout << "Maximum conversion path rate: " << result << endl;
    return 0;
}