#include "../includes/fontTest.hpp"

using namespace TerreateCore;
using namespace TerreateCore::Core;

class FontTest : public WindowController {
private:
  Buffer mBuffer;
  Shader mShader;
  Font mFont;

public:
  FontTest()
      : mBuffer(BufferUsage::DYNAMIC_DRAW),
        mFont("../resources/AsebiMin-Light.otf", 48) {
    mFont.LoadCharacter(L'A');

    auto attrs = Attribute::GenerateAttributes({2, 2});

    mBuffer.LoadVertices({-0.5f, -0.5f, 0.0f, 0.0f, 0.5f, -0.5f, 1.0f, 0.0f,
                          0.5f, 0.5f, 1.0f, 1.0f, -0.5f, 0.5f, 0.0f, 1.0f});
    mBuffer.LoadAttributes(attrs);
    mBuffer.LoadIndices({0, 1, 2, 2, 3, 0});

    mShader.AddVertexShaderSource("#version 330 core\n"
                                  "layout (location = 0) in vec2 pos;\n"
                                  "layout (location = 1) in vec2 tex;\n"
                                  "out vec2 texCoord;\n"
                                  "void main() {\n"
                                  "    gl_Position = vec4(pos, 0.0, 1.0);\n"
                                  "    texCoord = vec2(tex.x, 1 - tex.y);\n"
                                  "}\n");
    mShader.AddFragmentShaderSource("#version 330 core\n"
                                    "in vec2 texCoord;\n"
                                    "out vec4 color;\n"
                                    "uniform sampler2D tex;\n"
                                    "void main() {\n"
                                    "    float a = texture(tex, texCoord).r;\n"
                                    "    color = vec4(a, 0.0, 0.0, a);\n"
                                    "}\n");
    mShader.Compile();
    mShader.ActiveTexture(TextureTargets::TEX_0);
    mShader.SetInt("tex", 0);
    mShader.SetBlending(BlendingFuntion::SRC_ALPHA,
                        BlendingFuntion::ONE_MINUS_SRC_ALPHA);
    mShader.UseBlending(true);
  }

  void OnFrame(Window *window) override {
    window->PollEvents();
    window->Fill({0.0, 0.0, 1.0});
    window->Clear();

    mShader.Use();
    auto charData = mFont.GetCharacter(L'A');
    charData->texture->Bind();
    mBuffer.Draw(DrawMode::TRIANGLES);
    charData->texture->Unbind();
    window->Swap();
  }
};

void font_drawing_test() {
  Initialize();

  Window window(800, 600, "Font Drawing Test", WindowSettings());

  FontTest controller;
  window.SetWindowController(&controller);

  while (!window.IsClosed()) {
    window.Frame();
  }

  Terminate();
}

int main() {
  font_drawing_test();
  return 0;
}
