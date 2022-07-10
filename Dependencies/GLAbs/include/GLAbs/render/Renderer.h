#pragma once

#include <vector>
#include <GL\glew.h>
#include <glm.hpp>
#include <iostream>

class ErrorChecking {
public:
	static void GLClearErrors() { while (GLenum error = glGetError()); }

	static void GLFlushErrors() { while (GLenum error = glGetError()) { std::cout << "GL Error: " << error << std::endl; } }

	//static void GLCheckError() { while(GLenum error = glGetError()) { std::cout << "GL Error: " << error << std::endl; } }
};

struct ModelTexture {
	unsigned int textureId;
};

struct RawModel {
	unsigned int vaoId;
	unsigned int vertexCount;
};

/*
struct TexturedModel {
	RawModel* rawModel;
	ModelTexture* texture;
};
*/

class ShaderProgram {
public:
	ShaderProgram(const char* _vertexFile, int _vertexFileLength, const char* _fragmentFile, int _fragmentFileLength) { vertexFile = _vertexFile, vertexFileLength = _vertexFileLength; fragmentFile = _fragmentFile; fragmentFileLength = _fragmentFileLength; }

	void cleanUp();

	void start();

	void stop();

	void createShader();

	//virtual void initialize() = 0;

	unsigned int programId;
	unsigned int vertexShaderId;
	unsigned int fragmentShaderId;

private:
	unsigned int compileShader(unsigned int type, const std::string& source);
	const char* vertexFile;
	const char* fragmentFile;
	int vertexFileLength;
	int fragmentFileLength;

protected:
	virtual void bindAttributes() = 0;
	virtual void getAllUniformLocations() = 0;
	void bindAttribute(int attribute, std::string variableName);
	void loadFloat(unsigned int location, float value);
	void loadVector(unsigned int location, glm::vec3 vector);
	void loadBoolean(int location, bool value);
	void loadMatrix(int location, glm::mat4* matrix);
	unsigned int getUniformLocation(std::string uniformName);
};

class VaoData {
public:
	RawModel* rawModel;

	ModelTexture* modelTexture;

	VaoData() {}

	virtual void load() = 0;

	void cleanUp();

protected:
	std::vector<unsigned int*> vaos;
	std::vector<unsigned int*> vbos;
	std::vector<unsigned int*> textures;

	unsigned int createVao(); //todo: should functions return a pointer of primative?

	void storeDataInAttributeList(unsigned int attributeNumber, unsigned int coordinateSize, float* data, unsigned int dataLength);

	void unbindVao();

	void bindIndicesBuffer(unsigned int* indices, unsigned int indicesLength);

	unsigned int loadTexture(std::string fileName);
};

class Renderer {
public:
	ShaderProgram* shader;

	Renderer(ShaderProgram* _shader) { shader = _shader; }

	virtual void prepare() = 0;

	virtual void render(VaoData* vaoData, glm::mat4* transformationMatrix) = 0;

	virtual void cleanUp() = 0;
};

class Loader {
public:
	std::vector<VaoData*> vaoDatas;

	void loadToVao(VaoData* vaoData); //float* positions, unsigned int positionsLength, float* textureCoords, unsigned int textureCoordsLength, unsigned int* indices, unsigned int indicesLength

	void deleteVao(VaoData* vaoData);

	void cleanUp();
};