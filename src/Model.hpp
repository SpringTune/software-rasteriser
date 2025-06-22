#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <Eigen/Dense>

#include <cstdint>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

// #################
// ## Model class ##
// #################
class Model {
public:
    std::vector<Eigen::Vector4d> vertex;
    std::vector<std::vector<uint>> faces;

public:
    // Constructor
    Model() :
        vertex(std::vector<Eigen::Vector4d>()),
        faces(std::vector<std::vector<uint>>()) {}

    // Load from file
    void load(std::string filepath) {
        // Open file
        std::ifstream f = std::ifstream(filepath);
        if (!f.is_open()) return;

        std::string line;
        while (getline(f, line)) {
            // Turn line into stringstream
            std::stringstream sstream;
            sstream << line << "\n";

            // Read cmd
            std::string cmd;
            sstream >> cmd;

            // Read vertex
            if (cmd == "v") {
                Eigen::Vector4d v {0, 0, 0, 1};
                sstream >> v[0] >> v[1] >> v[2];
                vertex.push_back(v);
            }
            // Read faces
            else if (cmd == "f") {
                // Read vertex index
                std::vector<uint> face;
                uint vertex_val;
                while (sstream >> vertex_val) {
                    sstream.ignore(256, ' ');
                    vertex_val--;
                    face.push_back(vertex_val);
                }
                
                // Triangulate
                for (uint64_t i = 0; i < face.size()-2; i++) {
                    std::vector<uint> tri_face = {face[0], face[i+1], face[i+2]};
                    faces.push_back(tri_face);
                }
            }
        }

        // Close file
        f.close();
    }
};

#endif