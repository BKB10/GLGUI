#include "gui/GlWindow.h"
#include "gui/GlDrawSurface.h"
#include "../GLAbs/utils/Utils.h"
#include "../GLAbs/render/Renderer.cpp"

/*
    Make GUI elements later. I'm too lazy to now!
*/
class StaticShader : public ShaderProgram {
public:
    StaticShader() : ShaderProgram("resources/shaders/VertexShader.txt", 34, "resources/shaders/FragmentShader.txt", 36) {}

    void loadTransformationMatrix(glm::mat4* matrix) {
        loadMatrix(location_transformationMatrix, matrix);
    }

    //void initialize() {createShader();}

private:
    unsigned int location_transformationMatrix;

protected:
    void bindAttributes() { bindAttribute(0, "position"); bindAttribute(1, "textureCoords"); }
    void getAllUniformLocations() { location_transformationMatrix = getUniformLocation("transformationMatrix"); }
};

class OtherShader : public ShaderProgram {
public:
    OtherShader() : ShaderProgram("resources/shaders/OtherVertexShader.txt", 39, "resources/shaders/OtherFragmentShader.txt", 41) {}

    void loadTransformationMatrix(glm::mat4* matrix) {
        loadMatrix(location_transformationMatrix, matrix);
    }

    //void initialize() {createShader();}

private:
    unsigned int location_transformationMatrix;

protected:
    void bindAttributes() { bindAttribute(0, "position"); bindAttribute(1, "color"); }
    void getAllUniformLocations() { location_transformationMatrix = getUniformLocation("transformationMatrix"); }
};

class StaticRenderer : Renderer {
public:
    StaticRenderer(StaticShader* _shader) : Renderer(_shader) {}

    void prepare() {
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_ALPHA_TEST);
    }

    void render(VaoData* model, glm::mat4* transformationMatrix) {
        RawModel* rawModel = model->rawModel;
        ModelTexture* modelTexture = model->modelTexture;

        shader->start();
        ((StaticShader*)shader)->loadTransformationMatrix(transformationMatrix);
        glBindVertexArray(rawModel->vaoId);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, modelTexture->textureId);
        glDrawElements(GL_TRIANGLES, rawModel->vertexCount, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, rawModel->vertexCount);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glBindVertexArray(0);
        shader->stop();
    }

    void cleanUp() {
        shader->cleanUp();
    }
};

class OtherRenderer : Renderer {
public:
    OtherRenderer(OtherShader* _shader) : Renderer(_shader) {}

    void prepare() {
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_ALPHA_TEST);
    }

    void render(VaoData* vaoData, glm::mat4* transformationMatrix) {
        RawModel* rawModel = vaoData->rawModel;

        shader->start();
        ((StaticShader*)shader)->loadTransformationMatrix(transformationMatrix);
        glBindVertexArray(rawModel->vaoId);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glDrawElements(GL_TRIANGLES, rawModel->vertexCount, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, rawModel->vertexCount);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glBindVertexArray(0);
        shader->stop();
    }

    void cleanUp() {
        shader->cleanUp();
    }
};

class TexturedVaoData : VaoData {
public:
    float* positions;
    unsigned int positionsLength;
    float* textureCoords;
    unsigned int textureCoordsLength;
    unsigned int* indices;
    unsigned int indicesLength;
    std::string texture;

    TexturedVaoData(float* _positions, unsigned int _positionsLength, float* _textureCoords, unsigned int _textureCoordsLength, unsigned int* _indices, unsigned int _indicesLength, std::string _texture) : VaoData() { positions = _positions; positionsLength = _positionsLength; textureCoords = _textureCoords; textureCoordsLength = _textureCoordsLength; indices = _indices; indicesLength = _indicesLength; texture = _texture; }

    void load() {
        unsigned int vaoId = createVao();

        bindIndicesBuffer(indices, indicesLength);

        storeDataInAttributeList(0, 2, positions, positionsLength);
        storeDataInAttributeList(1, 2, textureCoords, textureCoordsLength);

        unbindVao();

        rawModel = new RawModel{ vaoId, indicesLength };
        modelTexture = new ModelTexture{ loadTexture(texture) };
    }
};

class TestVaoData : VaoData {
public:
    float* positions;
    unsigned int positionsLength;
    float* textureCoords;
    unsigned int textureCoordsLength;
    unsigned int* indices;
    unsigned int indicesLength;
    unsigned int textureId;

    TestVaoData(float* _positions, unsigned int _positionsLength, float* _textureCoords, unsigned int _textureCoordsLength, unsigned int* _indices, unsigned int _indicesLength, unsigned int _textureId) : VaoData() { positions = _positions; positionsLength = _positionsLength; textureCoords = _textureCoords; textureCoordsLength = _textureCoordsLength; indices = _indices; indicesLength = _indicesLength; textureId = _textureId; }

    void load() {
        unsigned int vaoId = createVao();

        bindIndicesBuffer(indices, indicesLength);

        storeDataInAttributeList(0, 2, positions, positionsLength);
        storeDataInAttributeList(1, 2, textureCoords, textureCoordsLength);

        unbindVao();

        rawModel = new RawModel{ vaoId, indicesLength };
        modelTexture = new ModelTexture{ textureId };
    }
};

class UntexturedVaoData : VaoData {
public:
    float* positions;
    unsigned int positionsLength;
    float* color;
    unsigned int colorLength;
    unsigned int* indices;
    unsigned int indicesLength;

    UntexturedVaoData(float* _positions, unsigned int _positionsLength, float* _color, unsigned int _colorLength, unsigned int* _indices, unsigned int _indicesLength) : VaoData() { positions = _positions; positionsLength = _positionsLength; color = _color; colorLength = _colorLength; indices = _indices; indicesLength = _indicesLength; }

    void load() {
        unsigned int vaoId = createVao();

        bindIndicesBuffer(indices, indicesLength);

        storeDataInAttributeList(0, 2, positions, positionsLength);
        storeDataInAttributeList(1, 2, color, colorLength);

        unbindVao();

        rawModel = new RawModel{ vaoId, indicesLength };
    }
};

class TestRenderAction : GlAction {
public:
    Loader* loader;
    TexturedVaoData* texturedData;
    StaticShader* shader;
    StaticRenderer* renderer;
    glm::mat4 transformationMatrix;

    UntexturedVaoData* untexturedData;
    TestVaoData* testData;
    OtherShader* otherShader;
    OtherRenderer* otherRenderer;
    glm::mat4 otherTransformationMatrix;

    ModelTexture* modelTexture;

    bool firstFrame = true;

    TestRenderAction() : GlAction() {
        unsigned int i = 0;

        unsigned int positionsLength = 8;
        float* positions = new float[positionsLength];
        positions[i++] = 0.0f;
        positions[i++] = 1.0f;

        positions[i++] = 0.0f;
        positions[i++] = 0.0f;

        positions[i++] = 1.0f;
        positions[i++] = 0.0f;

        positions[i++] = 1.0f;
        positions[i++] = 1.0f;

        i = 0;
        unsigned int textureCoordsLength = 8;
        float* textureCoords = new float[textureCoordsLength];
        textureCoords[i++] = 0;
        textureCoords[i++] = 0;

        textureCoords[i++] = 0;
        textureCoords[i++] = 1;

        textureCoords[i++] = 1;
        textureCoords[i++] = 1;

        textureCoords[i++] = 1;
        textureCoords[i++] = 0;

        i = 0;
        unsigned int colorLength = 12;
        float* color = new float[colorLength];
        color[i++] = 1;
        color[i++] = 0;
        color[i++] = 0;

        color[i++] = 0;
        color[i++] = 1;
        color[i++] = 0;

        color[i++] = 0;
        color[i++] = 0;
        color[i++] = 1;

        color[i++] = 1;
        color[i++] = 1;
        color[i++] = 1;

        i = 0;
        unsigned int indicesLength = 6;
        unsigned int* indices = new unsigned int[indicesLength];
        indices[i++] = 0;
        indices[i++] = 1;
        indices[i++] = 3;

        indices[i++] = 3;
        indices[i++] = 1;
        indices[i++] = 2;

        loader = new Loader();

        texturedData = new TexturedVaoData(positions, positionsLength, textureCoords, textureCoordsLength, indices, indicesLength, "resources/textures/cookieTexture.png");

        //testData = new TestVaoData(positions, positionsLength, textureCoords, textureCoordsLength, indices, indicesLength, ((GlWindow*)((GlComponent*)this)->parent)->textManager.characters['A'].texture.textureId);

        untexturedData = new UntexturedVaoData(positions, positionsLength, color, colorLength, indices, indicesLength);

        transformationMatrix = Utils::createTransformationMatrix(new glm::vec3(0, 0, 0), new glm::vec3(0, 0, 0.5), new glm::vec3(1, 1, 0)); //Remeber not to do memeory mismanagement here by making this a pointer when it shouldn't be!!

        otherTransformationMatrix = Utils::createTransformationMatrix(new glm::vec3(-0.5f, -0.25f, 0), new glm::vec3(0, 0, 0), new glm::vec3(1, 1, 0));
    };

    void action() {
        if (firstFrame) {
            firstFrame = false;

            shader = new StaticShader();
            shader->createShader();
            renderer = new StaticRenderer(shader);

            otherShader = new OtherShader();
            otherShader->createShader();
            otherRenderer = new OtherRenderer(otherShader);

            loader->loadToVao((VaoData*)texturedData);

            loader->loadToVao((VaoData*)untexturedData);

            loader->loadToVao((VaoData*) testData);
        }

        renderer->prepare();
        renderer->render((VaoData*) texturedData, &transformationMatrix);
        renderer->render((VaoData*)testData, &transformationMatrix);

        otherRenderer->prepare();
        otherRenderer->render((VaoData*)untexturedData, &otherTransformationMatrix);
    }
};

int main(void)
{
    int code = 0;

    GlWindow* window = new GlWindow("Good Cookies", 1280, 720);

    GlDrawSurface* drawSurface = new GlDrawSurface();
    drawSurface->actions.insert({ GlDrawSurface::RENDER_ACTION, (GlAction*) new TestRenderAction() });

    window->components.push_back(drawSurface);

    code = window->startWindow();

    return code;
}