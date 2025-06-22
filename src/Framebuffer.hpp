#ifndef __FRAMEBUFFER_HPP__
#define __FRAMEBUFFER_HPP__

#include <Eigen/Dense>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <string>

// #######################
// ## Framebuffer class ##
// #######################
class Framebuffer {
public:
    int32_t width;
    int32_t height;
    std::vector<Eigen::Vector3d> buffer;

public:
    // Constructor
    Framebuffer(int32_t width_, int32_t height_) :
        width(width_), height(height_),
        buffer(std::vector<Eigen::Vector3d>(width*height)) {}

    // Buffer operations
    Eigen::Vector3d& get(int x, int y) { return buffer[x + y*width]; }

    // File I/O
    void save(std::string filepath) {
        // Set data
        std::vector<uint8_t> data = std::vector<uint8_t>(width*height*3);
        for (uint64_t i = 0; i < width*height; i++) {
            data[3*i    ] = 255 * buffer[i][0];
            data[3*i + 1] = 255 * buffer[i][1];
            data[3*i + 2] = 255 * buffer[i][2];
        }

        // Save
        stbi_write_png(filepath.c_str(), width, height, 3, &(data[0]), width*3);
    }


};

#endif