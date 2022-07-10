#define _USE_MATH_DEFINES

#include "Utils.h"
#include <iostream>
#include <ext\matrix_transform.hpp>
#include <gtx\transform.hpp>
#include <math.h>

std::string Utils::convertToString(const char* a, int size) {
    int i;
    std::string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}

glm::mat4 Utils::createTransformationMatrix(glm::vec3* translation, glm::vec3* rotation, glm::vec3* scale) {
    glm::mat4 identityMatrix = glm::mat4();
    glm::mat4 translationMatrix = glm::translate(*translation);
    glm::mat4 scaleMatrix = glm::scale(*scale);
    glm::mat4 rotationMatrix1 = glm::rotate(rotation->x, glm::vec3(1, 0, 0));
    glm::mat4 rotationMatrix2 = glm::rotate(rotation->y, glm::vec3(0, 1, 0));
    glm::mat4 rotationMatrix3 = glm::rotate(rotation->z, glm::vec3(0, 0, 1));

    return translationMatrix * rotationMatrix1 * rotationMatrix2 * rotationMatrix3 * scaleMatrix;
}

double Utils::toRadians(double degrees) {
    return 2.0 * M_PI * (degrees / 360.0);
}