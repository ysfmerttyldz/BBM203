// <DO NOT CHANGE>
#ifndef MODELSTATS_H
#define MODELSTATS_H

#include <string>

class ModelStats {
public:
    explicit ModelStats(std::string name);

    void addBenchmark(double score);  // add a non-negative benchmark score
    double average() const;           // average score; 0 if no benchmarks

    const std::string& getName() const;

private:
    std::string name_;
    double total_;   // running total of scores
    int    count_;   // number of benchmarks recorded
};

// Return the model with the higher average (higher is better).
// If one pointer is nullptr, return the other.
// If both are nullptr or averages tie, return nullptr.
const ModelStats* compareTop(const ModelStats* a, const ModelStats* b);

#endif
// </DO NOT CHANGE>
