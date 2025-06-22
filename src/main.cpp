#include <Model.hpp>
#include <Eigen/Dense>
#include <Depthbuffer.hpp>
#include <Framebuffer.hpp>

#include <cstdint>

// Model -> Camera
Eigen::Matrix4d model_camera {
    {1, 0, 0,  -2},
    {0, 1, 0,  -2},
    {0, 0, 1, -5},
    {0, 0, 0,  1}
};

// Projection
double l = -1, r = 1;
double b = -1, t = 1;
double n = 1, f = 10;
Eigen::Matrix4d proj {
    {2*n/(r-l),         0, -(r+l)/(r-l),             0},
    {        0, 2*n/(t-b), -(t+b)/(t-b),             0},
    {        0,         0, -(f+n)/(f-n),  -2*f*n/(f-n)},
    {        0,         0,           -1,             1}
};

double edge(Eigen::Vector4d v0, Eigen::Vector4d v1, Eigen::Vector4d v2) {
    return (v1-v0)[0] * (v2-v0)[1] - (v1-v0)[1] * (v2-v0)[0];
}

void triangle(Framebuffer& framebuffer, Depthbuffer& depthbuffer, Eigen::Vector4d v0, Eigen::Vector4d v1, Eigen::Vector4d v2) {
    // Get bounding box
    int32_t xmin = std::min({v0[0], v1[0], v2[0]});
    int32_t xmax = std::max({v0[0], v1[0], v2[0]});
    int32_t ymin = std::min({v0[1], v1[1], v2[1]});
    int32_t ymax = std::max({v0[1], v1[1], v2[1]});

    // Draw triangle
    for (int32_t y = ymin; y < ymax; y++) {
        for (int32_t x = xmin; x < xmax; x++) {
            Eigen::Vector4d pixel = Eigen::Vector4d(x, y, 0, 0);

            double e0 = edge(v1, v2, pixel);
            double e1 = edge(v2, v0, pixel);
            double e2 = edge(v0, v1, pixel);
            if (e0 <= 0 && e1 <= 0 && e2 <= 0) {
                // Calculate barycentric coordinates
                double area = edge(v0, v1, v2);
                e0 /= area; e1 /= area; e2 /= area;
                
                // Interpolate z attribute
                double z = 1 / (e0*v0[3] + e1*v1[3] + e2*v2[3]);

                // Depthbuffer check
                if (depthbuffer.get(x, y) < z) continue;
                
                depthbuffer.get(x, y) = z;
                framebuffer.get(x, y) = Eigen::Vector3d(1, 1, 1);
            }
        }
    }
}

int main() {

    // Read model from file
    Model model;
    model.load("../data/cube.obj");

    // Apply model->camera + projection
    for (uint64_t i = 0; i < model.vertex.size(); i++) {
        model.vertex[i] = proj * model_camera * model.vertex[i];
    }

    // Perform clipping
    // CUR ASSUMPTIONS : Currently, we do no clipping
    // TODO : Maybe if I want to do something cool
    // THOUGHTS : Needs to have data as triangles in this stage


    // Z-divide
    for (uint64_t i = 0; i < model.vertex.size(); i++) {
        model.vertex[i][0] /= model.vertex[i][3];
        model.vertex[i][1] /= model.vertex[i][3];
        model.vertex[i][2] /= model.vertex[i][3];
        model.vertex[i][3] = 1/model.vertex[i][3];
    }

    // Change to raster coordinates
    int32_t img_width  = 800;
    int32_t img_height = 600;
    Framebuffer framebuffer(img_width, img_height);
    for (uint64_t i = 0; i < model.vertex.size(); i++) {
        model.vertex[i][0] =  0.5*(model.vertex[i][0] + 1) * (img_width-1);
        model.vertex[i][1] = -0.5*(model.vertex[i][1] - 1) * (img_height-1);
    }

    // Rasterise
    Depthbuffer depthbuffer(img_width, img_height);
    depthbuffer.clear();
    for (uint64_t i = 0; i < model.faces.size(); i++) {
        Eigen::Vector4d v0 = model.vertex[model.faces[i][0]];
        Eigen::Vector4d v1 = model.vertex[model.faces[i][1]];
        Eigen::Vector4d v2 = model.vertex[model.faces[i][2]];

        triangle(framebuffer, depthbuffer, v0, v1, v2);
    }
    

    // Save output
    framebuffer.save("../data/img.png");
    return 0;
}