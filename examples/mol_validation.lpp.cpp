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

// Molecule: EmptyMol
Molecule<std::string> EmptyMol;


// Molecule: SingleAtom
Molecule<std::string> SingleAtom;
SingleAtom.addAtom("Alone");
SingleAtom.addBond("Alone", "Alone", BondType::SINGLE);


// Molecule: SelfLoop
Molecule<std::string> SelfLoop;
SelfLoop.addAtom("A");
SelfLoop.addBond("A", "A", BondType::SINGLE);


// Molecule: MultiBond
Molecule<std::string> MultiBond;
MultiBond.addAtom("A");
MultiBond.addAtom("B");
MultiBond.addBond("A", "B", BondType::SINGLE);
MultiBond.addBond("A", "B", BondType::DOUBLE);
MultiBond.addBond("A", "B", BondType::ARROW);


// Molecule: LongNames
Molecule<std::string> LongNames;
LongNames.addAtom("VeryLongNodeName1");
LongNames.addAtom("VeryLongNodeName2");
LongNames.addAtom("VeryLongNodeName3");
LongNames.addBond("VeryLongNodeName1", "VeryLongNodeName2", BondType::SINGLE);
LongNames.addBond("VeryLongNodeName2", "VeryLongNodeName3", BondType::ARROW);


// Molecule: WithNumbers
Molecule<std::string> WithNumbers;
WithNumbers.addAtom("Node1");
WithNumbers.addAtom("Node2");
WithNumbers.addAtom("Node3");
WithNumbers.addBond("Node1", "Node2", BondType::SINGLE);
WithNumbers.addBond("Node2", "Node3", BondType::ARROW);


// Molecule: DAG
Molecule<std::string> DAG;
DAG.addAtom("Input1");
DAG.addAtom("Process1");
DAG.addAtom("Input2");
DAG.addAtom("Process2");
DAG.addAtom("Output");
DAG.addBond("Input1", "Process1", BondType::ARROW);
DAG.addBond("Input2", "Process1", BondType::ARROW);
DAG.addBond("Process1", "Process2", BondType::ARROW);
DAG.addBond("Process2", "Output", BondType::ARROW);


// Molecule: Bidirectional
Molecule<std::string> Bidirectional;
Bidirectional.addAtom("Client");
Bidirectional.addAtom("Server");
Bidirectional.addAtom("Database");
Bidirectional.addBond("Client", "Server", BondType::SINGLE);
Bidirectional.addBond("Server", "Database", BondType::SINGLE);


// Molecule: HubSpoke
Molecule<std::string> HubSpoke;
HubSpoke.addAtom("Center");
HubSpoke.addAtom("Spoke1");
HubSpoke.addAtom("Spoke2");
HubSpoke.addAtom("Spoke3");
HubSpoke.addAtom("Leaf1");
HubSpoke.addAtom("Leaf2");
HubSpoke.addAtom("Leaf3");
HubSpoke.addBond("Center", "Spoke1", BondType::SINGLE);
HubSpoke.addBond("Center", "Spoke2", BondType::SINGLE);
HubSpoke.addBond("Center", "Spoke3", BondType::SINGLE);
HubSpoke.addBond("Spoke1", "Leaf1", BondType::ARROW);
HubSpoke.addBond("Spoke2", "Leaf2", BondType::ARROW);
HubSpoke.addBond("Spoke3", "Leaf3", BondType::ARROW);


// Molecule: DenseGraph
Molecule<std::string> DenseGraph;
DenseGraph.addAtom("A");
DenseGraph.addAtom("B");
DenseGraph.addAtom("C");
DenseGraph.addAtom("D");
DenseGraph.addBond("A", "B", BondType::SINGLE);
DenseGraph.addBond("A", "C", BondType::SINGLE);
DenseGraph.addBond("A", "D", BondType::SINGLE);
DenseGraph.addBond("B", "C", BondType::SINGLE);
DenseGraph.addBond("B", "D", BondType::SINGLE);
DenseGraph.addBond("C", "D", BondType::SINGLE);


