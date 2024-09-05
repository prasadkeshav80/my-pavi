#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

std::mutex mtx;

// Function to add elements of two vectors
void addVectors(const std::vector<int>& vec1, const std::vector<int>& vec2, std::vector<int>& result, size_t start, size_t end) {
    for (size_t i = start; i < end; ++i) {
        result[i] = vec1[i] + vec2[i];
    }
}

// Function to multiply elements of two vectors
void multiplyVectors(const std::vector<int>& vec1, const std::vector<int>& vec2, std::vector<int>& result, size_t start, size_t end) {
    for (size_t i = start; i < end; ++i) {
        result[i] = vec1[i] * vec2[i];
    }
}

int main() {
    const size_t SIZE = 1000;
    const size_t NUM_THREADS = 4;
    
    // Initialize vectors
    std::vector<int> vec1(SIZE, 1);
    std::vector<int> vec2(SIZE, 2);
    std::vector<int> addResult(SIZE);
    std::vector<int> mulResult(SIZE);

    // Create threads for addition
    std::vector<std::thread> threads;
    size_t chunkSize = SIZE / NUM_THREADS;

    for (size_t i = 0; i < NUM_THREADS; ++i) {
        size_t start = i * chunkSize;
        size_t end = (i == NUM_THREADS - 1) ? SIZE : start + chunkSize;
        threads.push_back(std::thread(addVectors, std::cref(vec1), std::cref(vec2), std::ref(addResult), start, end));
    }

    // Join threads
    for (auto& t : threads) {
        t.join();
    }

    // Clear thread vector and reuse for multiplication
    threads.clear();

    // Create threads for multiplication
    for (size_t i = 0; i < NUM_THREADS; ++i) {
        size_t start = i * chunkSize;
        size_t end = (i == NUM_THREADS - 1) ? SIZE : start + chunkSize;
        threads.push_back(std::thread(multiplyVectors, std::cref(vec1), std::cref(vec2), std::ref(mulResult), start, end));
    }

    // Join threads
    for (auto& t : threads) {
        t.join();
    }

    // Output results (for verification)
    std::cout << "Addition Result (first 10 elements): ";
    for (size_t i = 0; i < 10; ++i) {
        std::cout << addResult[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Multiplication Result (first 10 elements): ";
    for (size_t i = 0; i < 10; ++i) {
        std::cout << mulResult[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
