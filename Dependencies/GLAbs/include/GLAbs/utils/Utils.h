#pragma once
#include <string>
#include <glm.hpp>

class Utils {
public:
    static std::string convertToString(const char* a, int size);

    static glm::mat4 createTransformationMatrix(glm::vec3* translation, glm::vec3* rotation, glm::vec3* scale);

    static double toRadians(double degrees);
};