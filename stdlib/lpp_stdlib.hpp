// LPP Standard Library - Data Structures
// This file provides modern data structures that transpile to C++ STL equivalents

#ifndef LPP_STDLIB_H
#define LPP_STDLIB_H

#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <deque>
#include <list>
#include <vector>
#include <algorithm>
#include <optional>
#include <stdexcept>
#include <cctype>

namespace lpp
{
    namespace stdlib
    {

        // ===== SET =====
        template <typename T>
        class Set
        {
        private:
            std::set<T> data;

        public:
            void add(const T &item) { data.insert(item); }
            void remove(const T &item) { data.erase(item); }
            bool has(const T &item) const { return data.find(item) != data.end(); }
            int size() const { return data.size(); }
            void clear() { data.clear(); }

            auto begin() { return data.begin(); }
            auto end() { return data.end(); }
        };

        // ===== MAP =====
        template <typename K, typename V>
        class Map
        {
        private:
            std::map<K, V> data;

        public:
            void set(const K &key, const V &value) { data[key] = value; }
            // BUG #206 fix: Safer get() - check existence first
            V get(const K &key) const
            {
                auto it = data.find(key);
                if (it == data.end())
                    throw std::out_of_range("Map key not found");
                return it->second;
            }
            // Alternative: get with default value
            V getOrDefault(const K &key, const V &defaultValue) const
            {
                auto it = data.find(key);
                return (it != data.end()) ? it->second : defaultValue;
            }
            bool has(const K &key) const { return data.find(key) != data.end(); }
            void remove(const K &key) { data.erase(key); }
            int size() const { return data.size(); }
            void clear() { data.clear(); }

            auto begin() { return data.begin(); }
            auto end() { return data.end(); }
        };

        // ===== HASHSET =====
        template <typename T>
        class HashSet
        {
        private:
            std::unordered_set<T> data;

        public:
            void add(const T &item) { data.insert(item); }
            void remove(const T &item) { data.erase(item); }
            bool has(const T &item) const { return data.find(item) != data.end(); }
            int size() const { return data.size(); }
            void clear() { data.clear(); }

            auto begin() { return data.begin(); }
            auto end() { return data.end(); }
        };

        // ===== HASHMAP =====
        template <typename K, typename V>
        class HashMap
        {
        private:
            std::unordered_map<K, V> data;

        public:
            void set(const K &key, const V &value) { data[key] = value; }
            // BUG #207 fix: Safer get() - check existence first
            V get(const K &key) const
            {
                auto it = data.find(key);
                if (it == data.end())
                    throw std::out_of_range("HashMap key not found");
                return it->second;
            }
            // Alternative: get with default value
            V getOrDefault(const K &key, const V &defaultValue) const
            {
                auto it = data.find(key);
                return (it != data.end()) ? it->second : defaultValue;
            }
            bool has(const K &key) const { return data.find(key) != data.end(); }
            void remove(const K &key) { data.erase(key); }
            int size() const { return data.size(); }
            void clear() { data.clear(); }

            auto begin() { return data.begin(); }
            auto end() { return data.end(); }
        };

        // ===== QUEUE (FIFO) =====
        template <typename T>
        class Queue
        {
        private:
            std::queue<T> data;

        public:
            void enqueue(const T &item) { data.push(item); }
            void enqueue(T &&item) { data.push(std::move(item)); } // BUG #226 fix: move overload
            T dequeue()
            {
                if (data.empty())
                    throw std::runtime_error("dequeue from empty queue");
                // BUG #208 fix: Avoid const_cast by copying front, then move
                T item = data.front();
                data.pop();
                return item;
            }
            T peek() const
            {
                // FIX BUG #201a: peek() crashes on empty queue
                if (data.empty())
                    throw std::runtime_error("peek on empty queue");
                return data.front();
            }
            bool isEmpty() const { return data.empty(); }
            int size() const { return data.size(); }
        };

        // ===== STACK (LIFO) =====
        template <typename T>
        class Stack
        {
        private:
            std::stack<T> data;

        public:
            void push(const T &item) { data.push(item); }
            void push(T &&item) { data.push(std::move(item)); } // BUG #227 fix: move overload
            T pop()
            {
                // BUG #202 fix: Add empty check
                if (data.empty())
                    throw std::runtime_error("pop from empty stack");
                T item = std::move(const_cast<T &>(data.top()));
                data.pop();
                return item;
            }
            T peek() const
            {
                // BUG #203 fix: Add empty check
                if (data.empty())
                    throw std::runtime_error("peek on empty stack");
                return data.top();
            }
            bool isEmpty() const { return data.empty(); }
            int size() const { return data.size(); }
        };

        // ===== DEQUE (Double-ended queue) =====
        template <typename T>
        class Deque
        {
        private:
            std::deque<T> data;

        public:
            void pushFront(const T &item) { data.push_front(item); }
            void pushFront(T &&item) { data.push_front(std::move(item)); } // BUG #228 fix: move overload
            void pushBack(const T &item) { data.push_back(item); }
            void pushBack(T &&item) { data.push_back(std::move(item)); } // BUG #229 fix: move overload
            T popFront()
            {
                // FIX BUG #201b: No empty check causes undefined behavior
                if (data.empty())
                    throw std::runtime_error("popFront from empty deque");
                T item = std::move(data.front()); // Move semantics
                data.pop_front();
                return item;
            }
            T popBack()
            {
                // FIX BUG #201b: No empty check causes undefined behavior
                if (data.empty())
                    throw std::runtime_error("popBack from empty deque");
                T item = std::move(data.back()); // Move semantics
                data.pop_back();
                return item;
            }
            T peekFront() const
            {
                // FIX BUG #201b: peek crashes on empty deque
                if (data.empty())
                    throw std::runtime_error("peekFront on empty deque");
                return data.front();
            }
            T peekBack() const
            {
                // FIX BUG #201b: peek crashes on empty deque
                if (data.empty())
                    throw std::runtime_error("peekBack on empty deque");
                return data.back();
            }
            int size() const { return data.size(); }
            bool isEmpty() const { return data.empty(); }
        };

        // ===== PRIORITY QUEUE =====
        template <typename T>
        class PriorityQueue
        {
        private:
            std::priority_queue<T> data;

        public:
            void push(const T &item) { data.push(item); }
            void push(T &&item) { data.push(std::move(item)); } // BUG #230 fix: move overload
            T pop()
            {
                // BUG #204 fix: Add empty check
                if (data.empty())
                    throw std::runtime_error("pop from empty priority queue");
                T item = std::move(const_cast<T &>(data.top()));
                data.pop();
                return item;
            }
            T peek() const
            {
                // BUG #205 fix: Add empty check
                if (data.empty())
                    throw std::runtime_error("peek on empty priority queue");
                return data.top();
            }
            int size() const { return data.size(); }
            bool isEmpty() const { return data.empty(); }
        };

        // ===== LINKED LIST =====
        template <typename T>
        class LinkedList
        {
        private:
            std::list<T> data;

        public:
            void pushFront(const T &item) { data.push_front(item); }
            void pushFront(T &&item) { data.push_front(std::move(item)); } // BUG #231 fix: move overload
            void pushBack(const T &item) { data.push_back(item); }
            void pushBack(T &&item) { data.push_back(std::move(item)); } // BUG #232 fix: move overload
            T popFront()
            {
                // FIX BUG #201c: No empty check causes undefined behavior
                if (data.empty())
                    throw std::runtime_error("popFront from empty list");
                T item = std::move(data.front()); // Move semantics
                data.pop_front();
                return item;
            }
            T popBack()
            {
                // FIX BUG #201c: No empty check causes undefined behavior
                if (data.empty())
                    throw std::runtime_error("popBack from empty list");
                T item = std::move(data.back()); // Move semantics
                data.pop_back();
                return item;
            }
            int size() const { return data.size(); }
            bool isEmpty() const { return data.empty(); }

            auto begin() { return data.begin(); }
            auto end() { return data.end(); }
        };

        // ===== STRING UTILITIES =====

        // Get length of string or vector
        template <typename T>
        int len(const std::vector<T> &vec)
        {
            return vec.size();
        }

        int len(const std::string &str)
        {
            return str.length();
        }

        // Push element to vector
        template <typename T>
        void push(std::vector<T> &vec, const T &item)
        {
            vec.push_back(item);
        }

        // BUG #233 fix: Move overload for push
        template <typename T>
        void push(std::vector<T> &vec, T &&item)
        {
            vec.push_back(std::move(item));
        }

        // Pop element from vector
        template <typename T>
        T pop(std::vector<T> &vec)
        {
            if (vec.empty())
                throw std::runtime_error("pop from empty vector");
            // BUG #209 fix: Use move semantics instead of copy
            T item = std::move(vec.back());
            vec.pop_back();
            return item;
        }

        // Split string by delimiter
        std::vector<std::string> split(const std::string &str, const std::string &delimiter)
        {
            // BUG #214 fix: Prevent infinite loop with empty delimiter
            if (delimiter.empty())
                throw std::invalid_argument("split delimiter cannot be empty");

            std::vector<std::string> result;
            size_t start = 0;
            size_t end = str.find(delimiter);

            while (end != std::string::npos)
            {
                result.push_back(str.substr(start, end - start));
                start = end + delimiter.length();
                end = str.find(delimiter, start);
            }
            result.push_back(str.substr(start));
            return result;
        }

        // Join vector of strings with delimiter
        std::string join(const std::vector<std::string> &vec, const std::string &delimiter)
        {
            std::string result;
            for (size_t i = 0; i < vec.size(); i++)
            {
                result += vec[i];
                // BUG #216 fix: Prevent underflow when vec.size() == 0
                if (i + 1 < vec.size())
                {
                    result += delimiter;
                }
            }
            return result;
        }

        // Slice vector
        template <typename T>
        std::vector<T> slice(const std::vector<T> &vec, int start, int end = -1)
        {
            if (end == -1)
                end = vec.size();
            if (start < 0)
                start = vec.size() + start;
            if (end < 0)
                end = vec.size() + end;

            // BUG #212 & #217 fix: Clamp to valid range
            if (start < 0)
                start = 0;
            if (end < 0)
                end = 0;
            if (start > (int)vec.size())
                start = vec.size();
            if (end > (int)vec.size())
                end = vec.size();
            if (end < start)
                end = start;

            std::vector<T> result;
            for (int i = start; i < end; i++)
            {
                result.push_back(vec[i]);
            }
            return result;
        }

        // Slice string
        std::string slice(const std::string &str, int start, int end = -1)
        {
            if (end == -1)
                end = str.length();
            if (start < 0)
                start = str.length() + start;
            if (end < 0)
                end = str.length() + end;

            // BUG #211 & #218 fix: Clamp to valid range and validate end >= start
            if (start < 0)
                start = 0;
            if (end < 0)
                end = 0;
            if (start > (int)str.length())
                start = str.length();
            if (end > (int)str.length())
                end = str.length();
            if (end < start)
                end = start;

            return str.substr(start, end - start);
        }

        // Get character at index
        char charAt(const std::string &str, int index)
        {
            if (index < 0 || index >= (int)str.length())
            {
                throw std::out_of_range("charAt index out of range");
            }
            return str[index];
        }

        // Substring
        std::string substring(const std::string &str, int start, int length)
        {
            // BUG #210 fix: Validate bounds before substr()
            if (start < 0 || start > (int)str.length())
                throw std::out_of_range("substring start out of range");
            if (length < 0)
                throw std::invalid_argument("substring length cannot be negative");
            // Clamp length to available characters
            int availableLength = str.length() - start;
            if (length > availableLength)
                length = availableLength;
            return str.substr(start, length);
        }

        // To uppercase
        std::string toUpper(const std::string &str)
        {
            std::string result = str;
            std::transform(result.begin(), result.end(), result.begin(), ::toupper);
            return result;
        }

        // To lowercase
        std::string toLower(const std::string &str)
        {
            std::string result = str;
            std::transform(result.begin(), result.end(), result.begin(), ::tolower);
            return result;
        }

        // Trim whitespace
        std::string trim(const std::string &str)
        {
            size_t start = str.find_first_not_of(" \t\n\r");
            if (start == std::string::npos)
                return "";
            size_t end = str.find_last_not_of(" \t\n\r");
            return str.substr(start, end - start + 1);
        }

        // String contains
        bool contains(const std::string &str, const std::string &substr)
        {
            return str.find(substr) != std::string::npos;
        }

        // String starts with
        bool startsWith(const std::string &str, const std::string &prefix)
        {
            // BUG #213 fix: Check length before substr()
            if (prefix.length() > str.length())
                return false;
            return str.substr(0, prefix.length()) == prefix;
        }

        // String ends with
        bool endsWith(const std::string &str, const std::string &suffix)
        {
            if (suffix.length() > str.length())
                return false;
            return str.substr(str.length() - suffix.length()) == suffix;
        }

        // Replace all occurrences
        std::string replace(const std::string &str, const std::string &from, const std::string &to)
        {
            // BUG #219 fix: Prevent infinite loop with empty 'from'
            if (from.empty())
                return str;

            std::string result = str;
            size_t pos = 0;
            while ((pos = result.find(from, pos)) != std::string::npos)
            {
                result.replace(pos, from.length(), to);
                pos += to.length();
            }
            return result;
        }

        // Repeat string n times
        std::string repeat(const std::string &str, int count)
        {
            // BUG #215 fix: Validate count >= 0
            if (count < 0)
                throw std::invalid_argument("repeat count cannot be negative");

            std::string result;
            for (int i = 0; i < count; i++)
            {
                result += str;
            }
            return result;
        }

        // Reverse string
        std::string reverse(const std::string &str)
        {
            return std::string(str.rbegin(), str.rend());
        }

        // ===== MOLECULE / GRAPH =====
        enum class BondType
        {
            SINGLE,       // - undirected edge
            DOUBLE,       // = strong connection
            ARROW,        // -> directed edge
            BIDIRECTIONAL // <-> bidirectional edge
        };

        template <typename T>
        struct MoleculeBond
        {
            T from;
            T to;
            BondType type;

            MoleculeBond(const T &f, const T &t, BondType bt)
                : from(f), to(t), type(bt) {}

            // Copy constructor
            MoleculeBond(const MoleculeBond &) = default;
            // Move constructor
            MoleculeBond(MoleculeBond &&) noexcept = default;
            // Copy assignment
            MoleculeBond &operator=(const MoleculeBond &) = default;
            // Move assignment
            MoleculeBond &operator=(MoleculeBond &&) noexcept = default;
        };

        template <typename T>
        class Molecule
        {
        private:
            std::unordered_map<T, std::vector<T>> adjacency;
            std::vector<MoleculeBond<T>> bonds;
            std::unordered_set<T> atoms;

        public:
            // Default constructor
            Molecule() = default;

            // Copy constructor
            Molecule(const Molecule &) = default;

            // Move constructor
            Molecule(Molecule &&) noexcept = default;

            // Copy assignment
            Molecule &operator=(const Molecule &) = default;

            // Move assignment
            Molecule &operator=(Molecule &&) noexcept = default;

            // Destructor
            ~Molecule() = default;

            // Add atom/node (with validation)
            void addAtom(const T &atom)
            {
                if (atoms.find(atom) != atoms.end())
                {
                    return; // Already exists, silently ignore
                }

                atoms.insert(atom);
                if (adjacency.find(atom) == adjacency.end())
                {
                    adjacency[atom] = std::vector<T>();
                }
            }

            // Add bond/edge (with validation)
            void addBond(const T &from, const T &to, BondType type)
            {
                // Ensure atoms exist
                addAtom(from);
                addAtom(to);

                // Check for duplicate bond (same from, to, type)
                for (const auto &bond : bonds)
                {
                    if (bond.from == from && bond.to == to && bond.type == type)
                    {
                        return; // Duplicate detected, ignore
                    }
                }

                bonds.push_back(MoleculeBond<T>(from, to, type));

                // Add to adjacency list based on bond type
                adjacency[from].push_back(to);
                if (type == BondType::SINGLE || type == BondType::BIDIRECTIONAL)
                {
                    // Undirected or bidirectional: add reverse edge
                    adjacency[to].push_back(from);
                }
                else if (type == BondType::DOUBLE)
                {
                    // Double bond treated as bidirectional
                    adjacency[to].push_back(from);
                }
            }

            // Get neighbors of an atom (const-correct)
            std::vector<T> neighbors(const T &atom) const
            {
                auto it = adjacency.find(atom);
                if (it != adjacency.end())
                {
                    return it->second;
                }
                return std::vector<T>();
            }

            // Check if atom exists
            bool hasAtom(const T &atom) const
            {
                return atoms.find(atom) != atoms.end();
            }

            // Check if bond exists
            bool hasBond(const T &from, const T &to, BondType type) const
            {
                for (const auto &bond : bonds)
                {
                    if (bond.from == from && bond.to == to && bond.type == type)
                    {
                        return true;
                    }
                }
                return false;
            }

            // Get all atoms (const reference)
            const std::unordered_set<T> &getAtoms() const
            {
                return atoms;
            }

            // Get all bonds (const reference)
            const std::vector<MoleculeBond<T>> &getBonds() const
            {
                return bonds;
            }

            // Atom count (size_t for consistency)
            size_t atomCount() const { return atoms.size(); }

            // Bond count (size_t for consistency)
            size_t bondCount() const { return bonds.size(); }

            // Clear all data
            void clear()
            {
                atoms.clear();
                bonds.clear();
                adjacency.clear();
            }

            // Check if molecule is empty
            bool empty() const
            {
                return atoms.empty() && bonds.empty();
            }

            // BFS traversal (returns nodes in BFS order)
            std::vector<T> bfs(const T &start) const
            {
                if (!hasAtom(start))
                {
                    return std::vector<T>(); // Start atom doesn't exist
                }

                std::vector<T> result;
                std::unordered_set<T> visited;
                std::queue<T> queue;

                queue.push(start);
                visited.insert(start);

                while (!queue.empty())
                {
                    T current = queue.front();
                    queue.pop();
                    result.push_back(current);

                    auto neighs = neighbors(current);
                    for (const auto &neighbor : neighs)
                    {
                        if (visited.find(neighbor) == visited.end())
                        {
                            visited.insert(neighbor);
                            queue.push(neighbor);
                        }
                    }
                }

                return result;
            }

            // DFS traversal (returns nodes in DFS order)
            std::vector<T> dfs(const T &start) const
            {
                if (!hasAtom(start))
                {
                    return std::vector<T>();
                }

                std::vector<T> result;
                std::unordered_set<T> visited;
                dfsHelper(start, visited, result);
                return result;
            }

            // Check if there's a path from 'from' to 'to'
            bool hasPath(const T &from, const T &to) const
            {
                if (!hasAtom(from) || !hasAtom(to))
                {
                    return false;
                }

                if (from == to)
                {
                    return true;
                }

                std::unordered_set<T> visited;
                std::queue<T> queue;

                queue.push(from);
                visited.insert(from);

                while (!queue.empty())
                {
                    T current = queue.front();
                    queue.pop();

                    if (current == to)
                    {
                        return true;
                    }

                    auto neighs = neighbors(current);
                    for (const auto &neighbor : neighs)
                    {
                        if (visited.find(neighbor) == visited.end())
                        {
                            visited.insert(neighbor);
                            queue.push(neighbor);
                        }
                    }
                }

                return false;
            }

            // Check if graph is connected
            bool isConnected() const
            {
                if (atoms.empty())
                {
                    return true; // Empty graph is vacuously connected
                }

                // BFS from first atom
                T start = *atoms.begin();
                auto reachable = bfs(start);
                return reachable.size() == atoms.size();
            }

            // Detect if graph has cycles (undirected)
            bool hasCycle() const
            {
                std::unordered_set<T> visited;
                std::unordered_map<T, T> parent;

                for (const auto &atom : atoms)
                {
                    if (visited.find(atom) == visited.end())
                    {
                        if (hasCycleHelper(atom, visited, parent, T()))
                        {
                            return true;
                        }
                    }
                }
                return false;
            }

        private:
            // DFS helper
            void dfsHelper(const T &node, std::unordered_set<T> &visited, std::vector<T> &result) const
            {
                visited.insert(node);
                result.push_back(node);

                auto neighs = neighbors(node);
                for (const auto &neighbor : neighs)
                {
                    if (visited.find(neighbor) == visited.end())
                    {
                        dfsHelper(neighbor, visited, result);
                    }
                }
            }

            // Cycle detection helper
            bool hasCycleHelper(const T &node, std::unordered_set<T> &visited,
                                std::unordered_map<T, T> &parent, const T &par) const
            {
                visited.insert(node);
                parent[node] = par;

                auto neighs = neighbors(node);
                for (const auto &neighbor : neighs)
                {
                    if (visited.find(neighbor) == visited.end())
                    {
                        if (hasCycleHelper(neighbor, visited, parent, node))
                        {
                            return true;
                        }
                    }
                    else if (neighbor != parent[node])
                    {
                        return true; // Back edge detected
                    }
                }
                return false;
            }
        };

        // ===== GRAPH UTILITIES (NEW in v0.8.16) =====
        // Additional graph algorithms as standalone functions

        // Check if a path exists using BFS
        template <typename T>
        bool graphHasPath(const std::unordered_map<T, std::vector<T>> &graph,
                          const T &start, const T &end)
        {
            if (graph.find(start) == graph.end())
                return false;
            if (start == end)
                return true;

            std::unordered_set<T> visited;
            std::queue<T> queue;
            queue.push(start);
            visited.insert(start);

            while (!queue.empty())
            {
                T current = queue.front();
                queue.pop();

                auto it = graph.find(current);
                if (it != graph.end())
                {
                    for (const T &neighbor : it->second)
                    {
                        if (neighbor == end)
                            return true;
                        if (visited.find(neighbor) == visited.end())
                        {
                            visited.insert(neighbor);
                            queue.push(neighbor);
                        }
                    }
                }
            }
            return false;
        }

        // Find shortest path (BFS for unweighted)
        template <typename T>
        std::vector<T> graphShortestPath(const std::unordered_map<T, std::vector<T>> &graph,
                                         const T &start, const T &end)
        {
            if (graph.find(start) == graph.end())
                return {};

            std::unordered_map<T, T> parent;
            std::unordered_set<T> visited;
            std::queue<T> queue;

            queue.push(start);
            visited.insert(start);
            parent[start] = start;

            while (!queue.empty())
            {
                T current = queue.front();
                queue.pop();

                if (current == end)
                {
                    // Reconstruct path
                    std::vector<T> path;
                    T node = end;
                    while (node != start)
                    {
                        path.push_back(node);
                        node = parent[node];
                    }
                    path.push_back(start);
                    std::reverse(path.begin(), path.end());
                    return path;
                }

                auto it = graph.find(current);
                if (it != graph.end())
                {
                    for (const T &neighbor : it->second)
                    {
                        if (visited.find(neighbor) == visited.end())
                        {
                            visited.insert(neighbor);
                            parent[neighbor] = current;
                            queue.push(neighbor);
                        }
                    }
                }
            }
            return {}; // No path found
        }

        // Count connected components
        template <typename T>
        int graphCountComponents(const std::unordered_map<T, std::vector<T>> &graph)
        {
            std::unordered_set<T> visited;
            int count = 0;

            for (const auto &[vertex, neighbors] : graph)
            {
                if (visited.find(vertex) == visited.end())
                {
                    count++;
                    // BFS to mark all reachable vertices
                    std::queue<T> queue;
                    queue.push(vertex);
                    visited.insert(vertex);

                    while (!queue.empty())
                    {
                        T current = queue.front();
                        queue.pop();

                        auto it = graph.find(current);
                        if (it != graph.end())
                        {
                            for (const T &neighbor : it->second)
                            {
                                if (visited.find(neighbor) == visited.end())
                                {
                                    visited.insert(neighbor);
                                    queue.push(neighbor);
                                }
                            }
                        }
                    }
                }
            }
            return count;
        }

        // Check if graph is bipartite (2-colorable)
        template <typename T>
        bool graphIsBipartite(const std::unordered_map<T, std::vector<T>> &graph)
        {
            std::unordered_map<T, int> color; // 0 or 1

            for (const auto &[vertex, neighbors] : graph)
            {
                if (color.find(vertex) == color.end())
                {
                    std::queue<T> queue;
                    queue.push(vertex);
                    color[vertex] = 0;

                    while (!queue.empty())
                    {
                        T current = queue.front();
                        queue.pop();

                        auto it = graph.find(current);
                        if (it != graph.end())
                        {
                            for (const T &neighbor : it->second)
                            {
                                if (color.find(neighbor) == color.end())
                                {
                                    color[neighbor] = 1 - color[current];
                                    queue.push(neighbor);
                                }
                                else if (color[neighbor] == color[current])
                                {
                                    return false; // Same color = not bipartite
                                }
                            }
                        }
                    }
                }
            }
            return true;
        }

    } // namespace stdlib
} // namespace lpp

#endif // LPP_STDLIB_H
