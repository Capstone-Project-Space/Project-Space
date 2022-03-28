#include "text.h"

std::shared_ptr<Text> Text::CreateText() {
     std::shared_ptr<Text> text = std::shared_ptr<Text>(new Text());
     return text;
}

std::shared_ptr<Text> Text::CreateText(const std::string src) {
     std::shared_ptr<Text> text = std::shared_ptr<Text>(new Text());
     if (src != "") text->AddFont(src);
     return text;
}

Text::Text() {
     vb = VertexBuffer::CreateVertexBuffer(24, NULL);
     VertexLayout vl;
     va = VertexArray::CreateVertexArray(vl, vb);

     shader = ShaderProgram::Create("resources/shaders/text_render.vert", "resources/shaders/text_render.frag");

     ft = new FT_Library;
     face = new FT_Face;

     if (FT_Init_FreeType(ft)) {
          fprintf(stderr, "ERROR::FREETYPE: Could not init FreeType Library\n");
          exit(0);
     }
}

Text::~Text() {
     delete &va;
     delete &vb;
     Free();
     delete ft;
     delete face;
}

void Text::AddFont(const std::string src) {
     printf("Adding Font: <%s>\n", src.c_str());
     if (FT_New_Face(*ft, src.c_str(), 0, face)) {
          fprintf(stderr, "ERROR::FREETYPE: Failed to load font\n");
          exit(0);
     }

     FT_Set_Pixel_Sizes(*face, 0, 48);

     glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

     for (unsigned char c = 0; c < 128; c++){
          // load character glyph 
          if (FT_Load_Char(*face, c, FT_LOAD_RENDER)){
               fprintf(stderr, "ERROR::FREETYTPE: Failed to load Glyph\n");
               continue;
          }
          // generate texture
          unsigned int texture;
          glGenTextures(1, &texture);
          glBindTexture(GL_TEXTURE_2D, texture);
          glTexImage2D(
               GL_TEXTURE_2D,
               0,
               GL_RED,
               (*face)->glyph->bitmap.width,
               (*face)->glyph->bitmap.rows,
               0,
               GL_RED,
               GL_UNSIGNED_BYTE,
               (*face)->glyph->bitmap.buffer
          );
          // set texture options
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          // now store character for later use
          Character character = {
              texture,
              glm::ivec2((*face)->glyph->bitmap.width, (*face)->glyph->bitmap.rows),
              glm::ivec2((*face)->glyph->bitmap_left, (*face)->glyph->bitmap_top),
              (*face)->glyph->advance.x
          };
          Characters.insert(std::pair<char, Character>(c, character));
     }
     glBindTexture(GL_TEXTURE_2D, 0);
     FT_Done_Face(*face);
}

void Text::Free() {
     FT_Done_FreeType(*ft);
}

void Text::RenderText(std::string text, float x, float y, float scale, glm::vec3 color) {
     shader->bind();
     shader->uploadFloat3("TextColor", color);
     glActiveTexture(GL_TEXTURE0);
     va->bind();

     // iterate through all characters
     std::string::const_iterator c;
     for (c = text.begin(); c != text.end(); c++){
          Character ch = Characters[*c];

          float xpos = x + ch.Bearing.x * scale;
          float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

          float w = ch.Size.x * scale;
          float h = ch.Size.y * scale;
          // update VBO for each character
          float vertices[6][4] = {
              { xpos,     ypos + h,   0.0f, 0.0f },
              { xpos,     ypos,       0.0f, 1.0f },
              { xpos + w, ypos,       1.0f, 1.0f },

              { xpos,     ypos + h,   0.0f, 0.0f },
              { xpos + w, ypos,       1.0f, 1.0f },
              { xpos + w, ypos + h,   1.0f, 0.0f }
          };
          // render glyph texture over quad
          glBindTexture(GL_TEXTURE_2D, ch.TextureID);
          // update content of VBO memory
          vb->updateBuffer(24, vertices, 0);
          glBindBuffer(GL_ARRAY_BUFFER, 0);
          // render quad
          glDrawArrays(GL_TRIANGLES, 0, 6);
          // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
          x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
     }
     glBindVertexArray(0);
}