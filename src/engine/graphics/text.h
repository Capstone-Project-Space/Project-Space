#pragma once
#include <map>

#include "src/engine/graphics/buffers.h"
#include "src/engine/graphics/vertex_array.h"
#include "src/engine/graphics/shader.h"

#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
     unsigned int TextureID;  // ID handle of the glyph texture
     glm::ivec2   Size;       // Size of glyph
     glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
     unsigned int Advance;    // Offset to advance to next glyph
};

class Text {
public:
     static std::shared_ptr<Text> Text::CreateText();
     static std::shared_ptr<Text> Text::CreateText(const std::string src);

     Text();
     ~Text();

     void AddFont(const std::string src);
     void Free();
     void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);

private:
     std::shared_ptr<VertexArray> va;
     std::shared_ptr<VertexBuffer> vb;
     std::shared_ptr<ShaderProgram> shader;

     FT_Library *ft;
     FT_Face *face;

     std::map<char, Character> Characters;
};