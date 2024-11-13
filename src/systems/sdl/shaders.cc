// -*- Mode: C++; tab-width:2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi:tw=80:et:ts=2:sts=2
//
// -----------------------------------------------------------------------
//
// This file is part of RLVM, a RealLive virtual machine clone.
//
// -----------------------------------------------------------------------
//
// Copyright (C) 2013 Elliot Glaysher
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
// -----------------------------------------------------------------------

#include "GL/glew.h"

#include "systems/base/graphics_object.h"
#include "systems/base/system_error.h"
#include "systems/sdl/sdl_utils.h"
#include "systems/sdl/shaders.h"

namespace {

const char kColorMaskShader[] =
    "uniform sampler2D current_values, mask;"
    ""
    "void main()"
    "{"
    "vec4 bg_colour = texture2D(current_values, gl_TexCoord[0].st);"
    "vec4 mask_vector = texture2D(mask, gl_TexCoord[1].st);"
    "float mask_colour = clamp(mask_vector.a * gl_Color.a, 0.0, 1.0);"
    "gl_FragColor = clamp(bg_colour - mask_colour + gl_Color * mask_colour, "
    "                     0.0, 1.0);"
    "}";

const char kObjectShader[] =
    "uniform sampler2D image;\n"
    "uniform vec4 colour;\n"
    "uniform float mono;\n"
    "uniform float invert;\n"
    "uniform float light;\n"
    "uniform vec3 tint;\n"
    "uniform float alpha;\n"
    "\n"
    "void tinter(in float pixel_val, in float tint_val, out float mixed) {\n"
    "  if (tint_val > 0.0) {\n"
    "    mixed = pixel_val + tint_val - (pixel_val * tint_val);\n"
    "  } else if (tint_val < 0.0) {\n"
    "    mixed = pixel_val * abs(tint_val);\n"
    "  } else {\n"
    "    mixed = pixel_val;\n"
    "  }\n"
    "}\n"
    "\n"
    "void main() {\n"
    "  vec4 pixel = texture2D(image, gl_TexCoord[0].st);\n"
    "\n"
    "  // The colour is blended directly with the incoming pixel value.\n"
    "  vec3 coloured = mix(pixel.rgb, colour.rgb, colour.a);\n"
    "  pixel = vec4(coloured.r, coloured.g, coloured.b, pixel.a);\n"
    "\n"
    "  if (mono > 0.0) {\n"
    "    // NTSC grayscale\n"
    "    float gray = dot(pixel.rgb, vec3(0.299, 0.587, 0.114));\n"
    "    vec3 mixed = mix(pixel.rgb, vec3(gray, gray, gray), mono);\n"
    "    pixel = vec4(mixed.r, mixed.g, mixed.b, pixel.a);\n"
    "  }\n"
    "\n"
    "  if (invert > 0.0) {\n"
    "    vec3 inverted = vec3(1.0, 1.0, 1.0) - pixel.rgb;\n"
    "    vec3 mixed = mix(pixel.rgb, inverted, invert);\n"
    "    pixel = vec4(mixed.r, mixed.g, mixed.b, pixel.a);\n"
    "  }\n"
    "\n"
    "  float out_r, out_g, out_b;\n"
    "  tinter(pixel.r, light, out_r);\n"
    "  tinter(pixel.g, light, out_g);\n"
    "  tinter(pixel.b, light, out_b);\n"
    "  pixel = vec4(out_r, out_g, out_b, pixel.a);\n"
    "\n"
    "  tinter(pixel.r, tint.r, out_r);\n"
    "  tinter(pixel.g, tint.g, out_g);\n"
    "  tinter(pixel.b, tint.b, out_b);\n"
    "  pixel = vec4(out_r, out_g, out_b, pixel.a);\n"
    "\n"
    "  // We're responsible for doing the main alpha blending, too.\n"
    "  pixel.a = pixel.a * alpha;\n"
    "  gl_FragColor = pixel;\n"
    "}\n";

}  // namespace

GLuint Shaders::color_mask_program_object_id_ = 0;
GLint Shaders::color_mask_current_values_ = 0;
GLint Shaders::color_mask_mask_ = 0;

GLuint Shaders::object_program_object_id_ = 0;
GLint Shaders::object_image_ = 0;
GLint Shaders::object_colour_ = 0;
GLint Shaders::object_tint_ = 0;
GLint Shaders::object_light_ = 0;
GLint Shaders::object_alpha_ = 0;
GLint Shaders::object_mono_ = 0;
GLint Shaders::object_invert_ = 0;

// static
void Shaders::Reset() {
  if (color_mask_program_object_id_) {
    glDeleteProgram(color_mask_program_object_id_);
    DebugShowGLErrors();

    color_mask_program_object_id_ = 0;
    color_mask_current_values_ = 0;
    color_mask_mask_ = 0;
  }

  if (object_program_object_id_) {
    glDeleteProgram(object_program_object_id_);
    DebugShowGLErrors();

    object_program_object_id_ = 0;
    object_image_ = 0;
    object_colour_ = 0;
    object_tint_ = 0;
    object_light_ = 0;
    object_alpha_ = 0;
    object_mono_ = 0;
    object_invert_ = 0;
  }
}

// static
GLuint Shaders::getColorMaskProgram() {
  if (color_mask_program_object_id_ == 0) {
    buildShader(kColorMaskShader, &color_mask_program_object_id_);
  }

  return color_mask_program_object_id_;
}

GLint Shaders::getColorMaskUniformCurrentValues() {
  if (color_mask_current_values_ == 0) {
    color_mask_current_values_ =
        glGetUniformLocation(getColorMaskProgram(), "current_values");
    if (color_mask_current_values_ == -1)
      throw SystemError("Bad uniform value: current_values");
  }

  return color_mask_current_values_;
}

GLint Shaders::getColorMaskUniformMask() {
  if (color_mask_mask_ == 0) {
    color_mask_mask_ = glGetUniformLocation(getColorMaskProgram(), "mask");
    if (color_mask_mask_ == -1)
      throw SystemError("Bad uniform value: mask");
  }

  return color_mask_mask_;
}

GLuint Shaders::GetObjectProgram() {
  if (object_program_object_id_ == 0) {
    buildShader(kObjectShader, &object_program_object_id_);
  }

  return object_program_object_id_;
}

GLint Shaders::GetObjectUniformImage() {
  if (object_image_ == 0) {
    object_image_ = glGetUniformLocation(GetObjectProgram(), "image");
    if (object_image_ == -1)
      throw SystemError("Bad uniform value: image");
  }

  return object_image_;
}

void Shaders::loadObjectUniformFromGraphicsObject(const GraphicsObject& go) {
  RGBAColour colour = go.colour();
  glUniform4f(Shaders::GetObjectUniformColour(),
                 colour.r_float(),
                 colour.g_float(),
                 colour.b_float(),
                 colour.a_float());

  RGBColour tint = go.tint();
  glUniform3f(Shaders::GetObjectUniformTint(),
                 tint.r_float(),
                 tint.g_float(),
                 tint.b_float());

  glUniform1f(Shaders::GetObjectUniformLight(), go.light() / 255.0f);

  glUniform1f(Shaders::GetObjectUniformMono(), go.mono() / 255.0f);

  glUniform1f(Shaders::GetObjectUniformInvert(), go.invert() / 255.0f);
}

GLint Shaders::GetObjectUniformColour() {
  if (object_colour_ == 0) {
    object_colour_ = glGetUniformLocation(GetObjectProgram(), "colour");
    if (object_colour_ == -1)
      throw SystemError("Bad uniform value: colour");
  }

  return object_colour_;
}

GLint Shaders::GetObjectUniformTint() {
  if (object_tint_ == 0) {
    object_tint_ = glGetUniformLocation(GetObjectProgram(), "tint");
    if (object_tint_ == -1)
      throw SystemError("Bad uniform value: tint");
  }

  return object_tint_;
}

GLint Shaders::GetObjectUniformLight() {
  if (object_light_ == 0) {
    object_light_ = glGetUniformLocation(GetObjectProgram(), "light");
    if (object_light_ == -1)
      throw SystemError("Bad uniform value: light");
  }

  return object_light_;
}

GLint Shaders::GetObjectUniformAlpha() {
  if (object_alpha_ == 0) {
    object_alpha_ = glGetUniformLocation(GetObjectProgram(), "alpha");
    if (object_alpha_ == -1)
      throw SystemError("Bad uniform value: alpha");
  }

  return object_alpha_;
}

GLint Shaders::GetObjectUniformMono() {
  if (object_mono_ == 0) {
    object_mono_ = glGetUniformLocation(GetObjectProgram(), "mono");
    if (object_mono_ == -1)
      throw SystemError("Bad uniform value: mono");
  }

  return object_mono_;
}

GLint Shaders::GetObjectUniformInvert() {
  if (object_invert_ == 0) {
    object_invert_ = glGetUniformLocation(GetObjectProgram(), "invert");
    if (object_invert_ == -1)
      throw SystemError("Bad uniform value: invert");
  }

  return object_invert_;
}

// static
void Shaders::buildShader(const char* shaderSource, GLuint* program_object) {
  GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(shader, 1, &shaderSource, NULL);
  glCompileShader(shader);

  GLint compiled;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  if (!compiled) {
      GLint logLength;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
      std::vector<GLchar> log(logLength);
      glGetShaderInfoLog(shader, logLength, &logLength, log.data());
      std::cerr << "Shader compilation failed: " << log.data() << std::endl;
      glDeleteShader(shader);
      return;
  }

  *program_object = glCreateProgram();
  glAttachShader(*program_object, shader);
  glLinkProgram(*program_object);

  GLint linked;
  glGetProgramiv(*program_object, GL_LINK_STATUS, &linked);
  if (!linked) {
      GLint logLength;
      glGetProgramiv(*program_object, GL_INFO_LOG_LENGTH, &logLength);
      std::vector<GLchar> log(logLength);
      glGetProgramInfoLog(*program_object, logLength, &logLength, log.data());
      std::cerr << "Program linking failed: " << log.data() << std::endl;
      glDeleteProgram(*program_object);
      return;
  }

  glDeleteShader(shader);
}
