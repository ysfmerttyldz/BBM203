#include "modelstats.h"
#include <iostream>
using namespace std;

ModelStats::ModelStats(std::string name)
: name_(std::move(name)), total_(0.0), count_(0) 
{}

void ModelStats::addBenchmark(double score) {
    if (score < 0) {
        cout << "ERROR: negative score ignored\n";
        return;
    }
    total_ += score;
    ++count_;
}

double ModelStats::average() const {
    double avg_as_int = static_cast<double>(total_ / count_);
    return avg_as_int;
}

const std::string& ModelStats::getName() const {
    return name_;
}

const ModelStats* compareTop(const ModelStats* a, const ModelStats* b) {
    double avga = a->average();
    double avgb = b->average();

    if (avga >= avgb) return a;
    if (avgb > avga) return b;
    return nullptr;
}
