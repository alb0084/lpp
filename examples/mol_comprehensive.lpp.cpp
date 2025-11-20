#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <tuple>
#include <array>
#include <optional>
#include <functional>
#include <variant>
#include <map>
#include <any>
#include <future>
#include <random>
#include <chrono>

// ============ QUANTUM VARIABLE SYSTEM ============
template<typename T>
class QuantumVar {
private:
    std::vector<T> states;
    std::vector<double> probabilities;
    std::optional<T> collapsed;
    bool hasWeights;
    std::mt19937 rng;

public:
    // Constructor for uniform distribution
    QuantumVar(const std::vector<T>& s) 
        : states(s), hasWeights(false), rng(std::chrono::system_clock::now().time_since_epoch().count()) {
        // Equal probability for all states
        if (states.empty()) throw std::runtime_error("QuantumVar: cannot create with empty states");
        double prob = 1.0 / states.size();
        probabilities = std::vector<double>(states.size(), prob);
    }
    
    // Constructor for weighted distribution
    QuantumVar(const std::vector<T>& s, const std::vector<double>& probs)
        : states(s), hasWeights(true), rng(std::chrono::system_clock::now().time_since_epoch().count()) {
            // Normalize probabilities to sum to 1.0
            double sum = 0.0;
            for (double p : probs) sum += p;
            if (sum > 0.0) {
                for (double p : probs) probabilities.push_back(p / sum);
            } else {
                // Fallback to uniform if all probabilities are 0
                if (states.empty()) throw std::runtime_error("QuantumVar: cannot create with empty states");
                double uniformProb = 1.0 / states.size();
                probabilities = std::vector<double>(states.size(), uniformProb);
            }
        }
    
    // observe(): Collapse superposition to single state
    T observe() {
        if (!collapsed) {
            // Weighted random selection
            std::discrete_distribution<> dist(probabilities.begin(), probabilities.end());
            int idx = dist(rng);
            if (idx >= 0 && idx < static_cast<int>(states.size())) collapsed = states[idx];
        }
        return *collapsed;
    }
    
    // map(): Transform all states (lazy)
    template<typename F>
    auto map(F func) -> QuantumVar<decltype(func(std::declval<T>()))> {
        using U = decltype(func(std::declval<T>()));
        std::vector<U> newStates;
        for (const auto& state : states) {
            newStates.push_back(func(state));
        }
        return QuantumVar<U>(newStates, probabilities);
    }
    
    // reset(): Return to superposition
    void reset() {
        collapsed.reset();
    }
    
    // entangle(): Create correlated quantum variable
    template<typename F>
    auto entangle(F transform) -> QuantumVar<decltype(transform(std::declval<T>()))> {
        using U = decltype(transform(std::declval<T>()));
        // If already collapsed, entangled var uses same index
        std::vector<U> entangledStates;
        for (const auto& state : states) {
            entangledStates.push_back(transform(state));
        }
        auto result = QuantumVar<U>(entangledStates, probabilities);
        if (collapsed) {
            // Force same index selection
            int collapsedIdx = 0;
            for (size_t i = 0; i < states.size(); ++i) {
                if (states[i] == *collapsed) { collapsedIdx = i; break; }
            }
            if (collapsedIdx >= 0 && collapsedIdx < static_cast<int>(entangledStates.size())) result.collapsed = entangledStates[collapsedIdx];
        }
        return result;
    }
};

// Global entangle function
template<typename T, typename F>
auto entangle(QuantumVar<T>& qvar, F transform) {
    return qvar.entangle(transform);
}
// ================================================

#include "../stdlib/lpp_stdlib.hpp"
using namespace lpp::stdlib;

void print(const std::string& s) {
    std::cout << s << std::endl;
}

void print(int n) {
    std::cout << n << std::endl;
}

void print(double n) {
    std::cout << n << std::endl;
}

// Higher-order function: map
template<typename T, typename F>
auto map(const std::vector<T>& vec, F func) {
    std::vector<decltype(func(vec[0]))> result;
    for (const auto& item : vec) {
        result.push_back(func(item));
    }
    return result;
}

// Higher-order function: filter
template<typename T, typename F>
std::vector<T> filter(const std::vector<T>& vec, F predicate) {
    std::vector<T> result;
    for (const auto& item : vec) {
        if (predicate(item)) {
            result.push_back(item);
        }
    }
    return result;
}

// Higher-order function: reduce/fold
template<typename T, typename F>
T reduce(const std::vector<T>& vec, T init, F func) {
    T result = init;
    for (const auto& item : vec) {
        result = func(result, item);
    }
    return result;
}

// Molecule: UndirectedGraph
Molecule<std::string> UndirectedGraph;
UndirectedGraph.addAtom("A");
UndirectedGraph.addAtom("B");
UndirectedGraph.addAtom("C");
UndirectedGraph.addAtom("D");
UndirectedGraph.addBond("A", "B", BondType::SINGLE);
UndirectedGraph.addBond("B", "C", BondType::SINGLE);
UndirectedGraph.addBond("C", "D", BondType::SINGLE);
UndirectedGraph.addBond("D", "A", BondType::SINGLE);


// Molecule: DirectedGraph
Molecule<std::string> DirectedGraph;
DirectedGraph.addAtom("Start");
DirectedGraph.addAtom("Node1");
DirectedGraph.addAtom("Node2");
DirectedGraph.addAtom("End");
DirectedGraph.addBond("Start", "Node1", BondType::ARROW);
DirectedGraph.addBond("Node1", "Node2", BondType::ARROW);
DirectedGraph.addBond("Node2", "End", BondType::ARROW);


// Molecule: MixedGraph
Molecule<std::string> MixedGraph;
MixedGraph.addAtom("A");
MixedGraph.addAtom("B");
MixedGraph.addAtom("C");
MixedGraph.addAtom("D");
MixedGraph.addBond("A", "B", BondType::SINGLE);
MixedGraph.addBond("B", "C", BondType::DOUBLE);
MixedGraph.addBond("C", "D", BondType::ARROW);


// Molecule: Network
Molecule<std::string> Network;
Network.addAtom("Server1");
Network.addAtom("Server2");
Network.addAtom("Server3");
Network.addAtom("Database");
Network.addAtom("Cache");
Network.addBond("Server1", "Server2", BondType::SINGLE);
Network.addBond("Server2", "Server3", BondType::SINGLE);
Network.addBond("Server1", "Database", BondType::ARROW);
Network.addBond("Server2", "Database", BondType::ARROW);
Network.addBond("Server3", "Database", BondType::ARROW);
Network.addBond("Database", "Cache", BondType::DOUBLE);


// Molecule: StarTopology
Molecule<std::string> StarTopology;
StarTopology.addAtom("Hub");
StarTopology.addAtom("Node1");
StarTopology.addAtom("Node2");
StarTopology.addAtom("Node3");
StarTopology.addAtom("Node4");
StarTopology.addBond("Hub", "Node1", BondType::SINGLE);
StarTopology.addBond("Hub", "Node2", BondType::SINGLE);
StarTopology.addBond("Hub", "Node3", BondType::SINGLE);
StarTopology.addBond("Hub", "Node4", BondType::SINGLE);


// Molecule: Triangle
Molecule<std::string> Triangle;
Triangle.addAtom("A");
Triangle.addAtom("B");
Triangle.addAtom("C");
Triangle.addBond("A", "B", BondType::SINGLE);
Triangle.addBond("B", "C", BondType::SINGLE);
Triangle.addBond("C", "A", BondType::SINGLE);


// Molecule: Chain
Molecule<std::string> Chain;
Chain.addAtom("First");
Chain.addAtom("Second");
Chain.addAtom("Third");
Chain.addAtom("Fourth");
Chain.addAtom("Fifth");
Chain.addBond("First", "Second", BondType::ARROW);
Chain.addBond("Second", "Third", BondType::ARROW);
Chain.addBond("Third", "Fourth", BondType::ARROW);
Chain.addBond("Fourth", "Fifth", BondType::ARROW);


{ main() {
    auto neighbors = UndirectedGraph.neighbors;
    "A";
    return println("Test passed!");
}
