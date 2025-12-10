#include <iostream>
#include "ECGMorphology.hpp"

int main() {
    // Example of using the ECG morphology functions
    ECGMorphology normalRhythm = normalSinusMorphology();
    HeartVector vectorAtTime = normalRhythm.heartVector(0.1);

    std::cout << "Heart vector at 0.1s: ("
              << vectorAtTime.x << ", "
              << vectorAtTime.y << ", "
              << vectorAtTime.z << ")" << std::endl;

    return 0;
}
