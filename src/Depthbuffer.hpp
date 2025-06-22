#ifndef __DEPTHBUFFER_HPP__
#define __DEPTHBUFFER_HPP__

#include <cmath>
#include <cstdint>
#include <string>

// #######################
// ## Depthbuffer class ##
// #######################
class Depthbuffer {
public:
    int32_t width;
    int32_t height;
    std::vector<double> buffer;

public:
    // Constructor
    Depthbuffer(int32_t width_, int32_t height_) :
        width(width_), height(height_),
        buffer(std::vector<double>(width*height)) {}

    // Clear buffer
    void clear() {
        for (uint64_t i = 0; i < buffer.size(); i++) {
            buffer[i] = INFINITY;
        }
    }

    // Buffer operations
    double& get(int x, int y) { return buffer[x + y*width]; }

};

#endif