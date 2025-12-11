#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <iomanip>
#include "json.hpp"

using json = nlohmann::json;

// Constants
const double PI = 3.14159265358979323846;

struct Vector3 {
    double x, y, z;
};

// Helper to rotate vector (x, z) around Y axis
void rotate(double px, double pz, double angle_deg, double &nx, double &nz) {
    double rad = angle_deg * PI / 180.0;
    // Standard 2D rotation matrix:
    // [ cos -sin ]
    // [ sin  cos ]
    // Applied to (x, z)
    nx = px * cos(rad) - pz * sin(rad);
    nz = px * sin(rad) + pz * cos(rad);
}

int main() {
    // Scene setup
    json scene;
    
    // Camera
    scene["camera"] = {
        {"type", "perspective"},
        {"focal_length", 3},
        {"eye", {0, 2, 15}},
        {"up", {0, 1, 0}},
        {"look", {0, -0.1, -1}},
        {"height", 1},
        {"width", 1.7777777778}
    };

    // Materials
    scene["materials"] = json::array({
        {{"name", "body_red"}, {"ka", {0.8, 0.1, 0.1}}, {"kd", {0.8, 0.1, 0.1}}, {"ks", {0.7, 0.2, 0.2}}, {"km", {0.0, 0.0, 0.0}}, {"phong_exponent", 300}},
        {{"name", "body_blue"}, {"ka", {0.1, 0.1, 0.8}}, {"kd", {0.1, 0.1, 0.8}}, {"ks", {0.2, 0.2, 0.7}}, {"km", {0.0, 0.0, 0.0}}, {"phong_exponent", 300}},
        {{"name", "body_green"}, {"ka", {0.1, 0.8, 0.1}}, {"kd", {0.1, 0.8, 0.1}}, {"ks", {0.2, 0.7, 0.2}}, {"km", {0.0, 0.0, 0.0}}, {"phong_exponent", 300}},
        {{"name", "body_yellow"}, {"ka", {0.8, 0.8, 0.1}}, {"kd", {0.8, 0.8, 0.1}}, {"ks", {0.7, 0.7, 0.2}}, {"km", {0.0, 0.0, 0.0}}, {"phong_exponent", 300}},
        {{"name", "body_cyan"}, {"ka", {0.1, 0.8, 0.8}}, {"kd", {0.1, 0.8, 0.8}}, {"ks", {0.2, 0.7, 0.7}}, {"km", {0.0, 0.0, 0.0}}, {"phong_exponent", 300}},
        {{"name", "body_purple"}, {"ka", {0.5, 0.1, 0.8}}, {"kd", {0.5, 0.1, 0.8}}, {"ks", {0.4, 0.2, 0.7}}, {"km", {0.0, 0.0, 0.0}}, {"phong_exponent", 300}},
        {{"name", "visor"}, {"ka", {0.6, 0.8, 1.0}}, {"kd", {0.6, 0.8, 1.0}}, {"ks", {0.9, 0.9, 0.9}}, {"km", {0.0, 0.0, 0.0}}, {"phong_exponent", 400}},
        {{"name", "floor"}, {"ka", {0.1, 0.1, 0.1}}, {"kd", {0.3, 0.3, 0.35}}, {"ks", {0.1, 0.1, 0.1}}, {"km", {0.0, 0.0, 0.0}}, {"phong_exponent", 10}},
        {{"name", "background"}, {"ka", {0.05, 0.05, 0.1}}, {"kd", {0.05, 0.05, 0.1}}, {"ks", {0.0, 0.0, 0.0}}, {"km", {0.0, 0.0, 0.0}}, {"phong_exponent", 1}},
        {{"name", "star"}, {"ka", {1.0, 1.0, 1.0}}, {"kd", {1.0, 1.0, 1.0}}, {"ks", {1.0, 1.0, 1.0}}, {"km", {0.0, 0.0, 0.0}}, {"phong_exponent", 1000}},
        {{"name", "rock"}, {"ka", {0.3, 0.2, 0.2}}, {"kd", {0.4, 0.3, 0.3}}, {"ks", {0.1, 0.1, 0.1}}, {"km", {0.0, 0.0, 0.0}}, {"phong_exponent", 5}}
    });

    // Lights
    scene["lights"] = json::array({
        {{"type", "point"}, {"position", {5, 10, 10}}, {"color", {0.8, 0.8, 0.8}}},
        {{"type", "point"}, {"position", {-10, 5, 0}}, {"color", {0.4, 0.4, 0.5}}},
        {{"type", "point"}, {"position", {0, 2, -5}}, {"color", {0.2, 0.2, 0.3}}}
    });

    // Objects
    scene["objects"] = json::array({
        {{"type", "plane"}, {"material", "floor"}, {"point", {0, -1, 0}}, {"normal", {0, 1, 0}}},
        {{"type", "plane"}, {"material", "background"}, {"point", {0, 0, -20}}, {"normal", {0, 0, 1}}}
    });

    std::vector<std::string> colors = {"body_red", "body_blue", "body_green", "body_yellow", "body_cyan", "body_purple"};
    
    // Random generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis_angle(-30.0, 30.0); // Random offset for facing
    std::uniform_real_distribution<> dis_rock_x(-10.0, 10.0);
    std::uniform_real_distribution<> dis_rock_z(-5.0, 10.0);
    std::uniform_real_distribution<> dis_rock_r(0.05, 0.15);
    std::uniform_real_distribution<> dis_star_x(-20.0, 20.0);
    std::uniform_real_distribution<> dis_star_y(0.0, 15.0);
    std::uniform_real_distribution<> dis_star_r(0.05, 0.1);

    // Positions
    std::vector<std::pair<double, double>> positions = {
        {0, 0}, {2, 2}, {-2, 2}, {3, -1}, {-3, -1},
        {1, 4}, {-1, 4}, {4, 1}, {-4, 1},
        {0, 6}, {5, 5}, {-5, 5}
    };

    // Camera position for facing calculation
    double cam_x = 0;
    double cam_z = 15;

    for (size_t i = 0; i < positions.size(); ++i) {
        double x = positions[i].first;
        double z = positions[i].second - 2; // Shift z back a bit
        std::string color = colors[i % colors.size()];

        // Calculate angle to face camera
        double dx = cam_x - x;
        double dz = cam_z - z;
        // atan2(x, z) gives angle from Z axis in X-Z plane (where X is like Y in standard atan2(y,x))
        // We want 0 degrees to be +Z.
        // If dx=0, dz=1, atan2(0, 1) = 0. Correct.
        // If dx=1, dz=0, atan2(1, 0) = 90. Correct.
        double base_angle = atan2(dx, dz) * 180.0 / PI;
        
        double angle = base_angle + dis_angle(gen);

        // Add Among Us character
        double y_shift = -0.48;
        
        struct Part {
            std::string type;
            std::string material;
            double radius;
            Vector3 center;
        };

        std::vector<Part> parts = {
            {"sphere", color, 0.7, {0, 0.5, 0}},
            {"sphere", "visor", 0.28, {0.18, 0.8, 0.55}},
            {"sphere", color, 0.22, {-0.22, -0.3, 0.18}},
            {"sphere", color, 0.22, {0.22, -0.3, 0.18}},
            {"sphere", color, 0.35, {0, 0.6, -0.5}}
        };

        for (const auto& part : parts) {
            double cx = part.center.x;
            double cy = part.center.y;
            double cz = part.center.z;

            double rx, rz;
            rotate(cx, cz, angle, rx, rz);

            scene["objects"].push_back({
                {"type", "sphere"},
                {"material", part.material},
                {"radius", part.radius},
                {"center", {x + rx, cy + y_shift, z + rz}}
            });
        }
    }

    // Rocks
    for (int i = 0; i < 20; ++i) {
        double rx = dis_rock_x(gen);
        double rz = dis_rock_z(gen);
        double rr = dis_rock_r(gen);
        scene["objects"].push_back({
            {"type", "sphere"},
            {"material", "rock"},
            {"radius", rr},
            {"center", {rx, -1 + rr * 0.8, rz}}
        });
    }

    // Stars
    for (int i = 0; i < 50; ++i) {
        double sx = dis_star_x(gen);
        double sy = dis_star_y(gen);
        double sz = -19;
        double sr = dis_star_r(gen);
        scene["objects"].push_back({
            {"type", "sphere"},
            {"material", "star"},
            {"radius", sr},
            {"center", {sx, sy, sz}}
        });
    }

    std::cout << std::setw(4) << scene << std::endl;
    return 0;
}
