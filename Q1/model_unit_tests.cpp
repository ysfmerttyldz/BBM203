// <DO NOT CHANGE>
/**
 * Debugging Quiz: Best Performing AI Models
 * -----------------------------------------------------------------------------------
 * We track each model's average benchmark score (higher is better).
 * 
 * 
 * Tasks:
 * 1. Open bankAccount.cpp and locate the logical and runtime errors.
 * 2. Fix them so that all tests below produce the *expected outputs*.
 *
 * Notes:
 * - You should not modify code within <DO NOT CHANGE>...</DO NOT CHANGE> tags.
 * - The program should compile, run, and match the expected outputs.
 *
 * Expected console flow:
 * Llama avg after 2 benches: 12.5
 * Llama avg after +0.5: 8.5
 * GPT avg after 2 benches: 12.5
 * Top between Llama and GPT: (none)
 * Mistral avg (single zero bench): 0
 * Top between Llama and Mistral: Llama (12.5)
 * Notes -> Llama: Good, Mistral: Needs Improvement, GPT: Good
 */
#include <iostream>
#include <string>
#include "modelstats.h"
using namespace std;

static string noteFor(double avg) {
    if (avg >= 30) return "Excellent";
    if (avg >= 10) return "Good";
    return "Needs Improvement";
}

int main() {
    // Llama: two benchmarks → avg should be 12.5
    ModelStats llama("Llama");
    llama.addBenchmark(10.0);
    llama.addBenchmark(15.0);
    cout << "Llama avg after 2 benches: " << llama.average() << " (expected 12.5)\n";

    // Add a fractional benchmark to expose truncation → avg should be 8.5
    llama.addBenchmark(0.5);
    cout << "Llama avg after +0.5: " << llama.average() << " (expected 8.5)\n";

    // GPT mirrors Llama's first two benchmarks to test ties
    ModelStats gpt("GPT");
    gpt.addBenchmark(10.0);
    gpt.addBenchmark(15.0);
    cout << "GPT avg after 2 benches: " << gpt.average() << " (expected 12.5)\n";

    // Tie between Llama (recreated with two benches) and GPT should yield (none)
    ModelStats llama2("Llama");
    llama2.addBenchmark(10.0);
    llama2.addBenchmark(15.0);
    const ModelStats* topTie = compareTop(&llama2, &gpt);
    if (topTie) {
        cout << "Top between Llama and GPT: " << topTie->getName()
             << " (" << topTie->average() << ")\n";
    } else {
        cout << "Top between Llama and GPT: (none)\n";
    }

    // Mistral: add a single 0.0 to avoid division by zero in buggy code (still reveals wrong rounding)
    ModelStats mistral("Mistral");
    mistral.addBenchmark(0.0);
    cout << "Mistral avg (single zero bench): " << mistral.average() << " (expected 0)\n";

    // Compare Llama(12.5) vs Mistral(0) → top is Llama
    const ModelStats* top = compareTop(&llama2, &mistral);
    if (top) {
        cout << "Top between Llama and Mistral: " << top->getName()
             << " (" << top->average() << ")\n";
    } else {
        cout << "Top between Llama and Mistral: (none)\n";
    }

    cout << "Notes -> "
         << llama2.getName() << ": " << noteFor(llama2.average())
         << ", " << mistral.getName() << ": " << noteFor(mistral.average())
         << ", " << gpt.getName() << ": " << noteFor(gpt.average())
         << "\n";

    return 0;
}
// </DO NOT CHANGE>
