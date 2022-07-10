#include <iostream>
#include <fstream>
#include <gtc\type_ptr.hpp>
#include "Renderer.h";
#include "../utils/Utils.h";
#include "lodepng.h"

/*
==================================================

ShaderProgram

==================================================
*/
    unsigned int ShaderProgram::compileShader(unsigned int type, const std::string& source) {
        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);

        if (!result) {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);
            std::cout << "Faled to compile shader!" << std::endl;
            std::cout << message << std::endl;

            glDeleteShader(id);

            return 0;
        }

        return id;
    }

    void ShaderProgram::createShader() {
        std::ifstream ifs;
        ifs.open(Utils::convertToString(vertexFile, vertexFileLength));
        std::string vertex((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
        ifs.close();

        ifs.open(Utils::convertToString(fragmentFile, fragmentFileLength));
        std::string fragment((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
        ifs.close();

        programId = glCreateProgram();
        vertexShaderId = compileShader(GL_VERTEX_SHADER, vertex);
        fragmentShaderId = compileShader(GL_FRAGMENT_SHADER, fragment);

        glAttachShader(programId, vertexShaderId);
        glAttachShader(programId, fragmentShaderId);

        bindAttributes();

        glLinkProgram(programId);
        glValidateProgram(programId);

        getAllUniformLocations();
    }

    unsigned int ShaderProgram::getUniformLocation(std::string uniformName) {
        return glGetUniformLocation(programId, uniformName.c_str());
    }

    void ShaderProgram::loadFloat(unsigned int location, float value) {
        return glUniform1f(location, value);
    }

    void ShaderProgram::loadVector(unsigned int location, glm::vec3 vector) {
        glUniform3f(location, vector.x, vector.y, vector.z);
    }

    void ShaderProgram::loadBoolean(int location, bool value) {
        float toLoad = 0;
        if (value) {
            toLoad = 1;
        }
        glUniform1f(location, toLoad);
    }

    void ShaderProgram::loadMatrix(int location, glm::mat4* matrix) {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(*matrix));
    }

    void ShaderProgram::cleanUp() {
        stop();
        glDetachShader(programId, vertexShaderId);
        glDetachShader(programId, fragmentShaderId);
        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);
        glDeleteProgram(programId);
    }

    void ShaderProgram::start() {
        glUseProgram(programId);
    }

    void ShaderProgram::stop() {
        glUseProgram(0);
    }

    void ShaderProgram::bindAttribute(int attribute, std::string variableName) {
        char* cstr = new char[variableName.size() + 1];

        variableName.copy(cstr, variableName.size() + 1);
        cstr[variableName.size()] = '\0';

        glBindAttribLocation(programId, attribute, "positions");
    }

/*
==================================================

Loader

==================================================
*/
void Loader::loadToVao(VaoData* vaoData) {
    vaoDatas.push_back(vaoData);

    vaoData->load();
}

void Loader::cleanUp() {
    for (int i = 0; i < vaoDatas.size(); i++) {
        vaoDatas[i]->cleanUp();
    }
}

/*
==================================================

VaoData

==================================================
*/
void VaoData::cleanUp() {
    for (unsigned int* vao : vaos) {
        glDeleteVertexArrays(1, vao);
    }

    for (unsigned int* vbo : vbos) {
        glDeleteBuffers(1, vbo);
    }

    for (unsigned int* texture : textures) {
        glDeleteBuffers(1, texture);
    }
}

unsigned int VaoData::createVao() {
	unsigned int vaoId;
	vaos.push_back(&vaoId);
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	return vaoId;
}

void VaoData::storeDataInAttributeList(unsigned int attributeNumber, unsigned int coordinateSize, float* data, unsigned int dataLength) {
	unsigned int vboId;
	glGenBuffers(1, &vboId);
	vbos.push_back(&vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, dataLength * sizeof(float), data, GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, GL_FALSE, sizeof(float) * coordinateSize, 0); //thin matrix made argument 4 = 0!
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VaoData::unbindVao() {
	glBindVertexArray(0);
}

void VaoData::bindIndicesBuffer(unsigned int* indices, unsigned int indicesLength) {
	unsigned int vboId;
	glGenBuffers(1, &vboId);
	vbos.push_back(&vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesLength * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

unsigned int VaoData::loadTexture(std::string fileName) {
    // Load file and decode image.
    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, fileName);

    if (error != 0) {
        std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
        return 1;
    }

    // Texture size must be power of two for the primitive OpenGL version this is written for. Find next power of two.
    size_t u2 = 1; while (u2 < width) u2 *= 2;
    size_t v2 = 1; while (v2 < height) v2 *= 2;
    // Ratio for power of two version compared to actual version, to render the non power of two image with proper size.
    double u3 = (double)width / u2;
    double v3 = (double)height / v2;

    // Make power of two version of the image.
    std::vector<unsigned char> image2(u2 * v2 * 4);
    for (size_t y = 0; y < height; y++)
        for (size_t x = 0; x < width; x++)
            for (size_t c = 0; c < 4; c++) {
                image2[4 * u2 * y + 4 * x + c] = image[4 * width * y + 4 * x + c];
            }

    // Store the texture in a texture ID
    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_NEAREST = no smoothing
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, u2, v2, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image2[0]);

    glBindTexture(GL_TEXTURE_2D, 0);

    textures.push_back(&textureId);

    return textureId;
}