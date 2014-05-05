/*
 * bindings.cc
 *
 *  Created on: Dec 13, 2011
 *      Author: ngk437
 */


#include "webgl.h"
#include "image.h"
#include <cstdlib>

#define JS_GL_CONSTANT(name) target->Set(JS_STR( #name ), JS_INT(GL_ ## name))

extern "C" {
void init(Handle<Object> target)
{
  atexit(webgl::AtExit);
  atexit(Image::AtExit);

  Image::Initialize(target);

  NODE_SET_METHOD(target, "Init", webgl::Init);
 
  NODE_SET_METHOD(target, "uniform1f", webgl::Uniform1f);
  NODE_SET_METHOD(target, "uniform2f", webgl::Uniform2f);
  NODE_SET_METHOD(target, "uniform3f", webgl::Uniform3f);
  NODE_SET_METHOD(target, "uniform4f", webgl::Uniform4f);
  NODE_SET_METHOD(target, "uniform1i", webgl::Uniform1i);
  NODE_SET_METHOD(target, "uniform2i", webgl::Uniform2i);
  NODE_SET_METHOD(target, "uniform3i", webgl::Uniform3i);
  NODE_SET_METHOD(target, "uniform4i", webgl::Uniform4i);
  NODE_SET_METHOD(target, "uniform1fv", webgl::Uniform1fv);
  NODE_SET_METHOD(target, "uniform2fv", webgl::Uniform2fv);
  NODE_SET_METHOD(target, "uniform3fv", webgl::Uniform3fv);
  NODE_SET_METHOD(target, "uniform4fv", webgl::Uniform4fv);
  NODE_SET_METHOD(target, "uniform1iv", webgl::Uniform1iv);
  NODE_SET_METHOD(target, "uniform2iv", webgl::Uniform2iv);
  NODE_SET_METHOD(target, "uniform3iv", webgl::Uniform3iv);
  NODE_SET_METHOD(target, "uniform4iv", webgl::Uniform4iv);
  NODE_SET_METHOD(target, "pixelStorei", webgl::PixelStorei);
  NODE_SET_METHOD(target, "bindAttribLocation", webgl::BindAttribLocation);
  NODE_SET_METHOD(target, "getError", webgl::GetError);
  NODE_SET_METHOD(target, "drawArrays", webgl::DrawArrays);
  NODE_SET_METHOD(target, "uniformMatrix2fv", webgl::UniformMatrix2fv);
  NODE_SET_METHOD(target, "uniformMatrix3fv", webgl::UniformMatrix3fv);
  NODE_SET_METHOD(target, "uniformMatrix4fv", webgl::UniformMatrix4fv);

  NODE_SET_METHOD(target, "generateMipmap", webgl::GenerateMipmap);

  NODE_SET_METHOD(target, "getAttribLocation", webgl::GetAttribLocation);
  NODE_SET_METHOD(target, "depthFunc", webgl::DepthFunc);
  NODE_SET_METHOD(target, "viewport", webgl::Viewport);
  NODE_SET_METHOD(target, "createShader", webgl::CreateShader);
  NODE_SET_METHOD(target, "shaderSource", webgl::ShaderSource);
  NODE_SET_METHOD(target, "compileShader", webgl::CompileShader);
  NODE_SET_METHOD(target, "getShaderParameter", webgl::GetShaderParameter);
  NODE_SET_METHOD(target, "getShaderInfoLog", webgl::GetShaderInfoLog);
  NODE_SET_METHOD(target, "createProgram", webgl::CreateProgram);
  NODE_SET_METHOD(target, "attachShader", webgl::AttachShader);
  NODE_SET_METHOD(target, "linkProgram", webgl::LinkProgram);
  NODE_SET_METHOD(target, "getProgramParameter", webgl::GetProgramParameter);
  NODE_SET_METHOD(target, "getUniformLocation", webgl::GetUniformLocation);
  NODE_SET_METHOD(target, "clearColor", webgl::ClearColor);
  NODE_SET_METHOD(target, "clearDepth", webgl::ClearDepth);

  NODE_SET_METHOD(target, "disable", webgl::Disable);
  NODE_SET_METHOD(target, "createTexture", webgl::CreateTexture);
  NODE_SET_METHOD(target, "bindTexture", webgl::BindTexture);
  NODE_SET_METHOD(target, "texImage2D", webgl::TexImage2D);
  NODE_SET_METHOD(target, "texParameteri", webgl::TexParameteri);
  NODE_SET_METHOD(target, "texParameterf", webgl::TexParameterf);
  NODE_SET_METHOD(target, "clear", webgl::Clear);
  NODE_SET_METHOD(target, "useProgram", webgl::UseProgram);
  NODE_SET_METHOD(target, "createFramebuffer", webgl::CreateFramebuffer);
  NODE_SET_METHOD(target, "bindFramebuffer", webgl::BindFramebuffer);
  NODE_SET_METHOD(target, "framebufferTexture2D", webgl::FramebufferTexture2D);
  NODE_SET_METHOD(target, "createBuffer", webgl::CreateBuffer);
  NODE_SET_METHOD(target, "bindBuffer", webgl::BindBuffer);
  NODE_SET_METHOD(target, "bufferData", webgl::BufferData);
  NODE_SET_METHOD(target, "bufferSubData", webgl::BufferSubData);
  NODE_SET_METHOD(target, "enable", webgl::Enable);
  NODE_SET_METHOD(target, "blendEquation", webgl::BlendEquation);
  NODE_SET_METHOD(target, "blendFunc", webgl::BlendFunc);
  NODE_SET_METHOD(target, "enableVertexAttribArray", webgl::EnableVertexAttribArray);
  NODE_SET_METHOD(target, "vertexAttribPointer", webgl::VertexAttribPointer);
  NODE_SET_METHOD(target, "activeTexture", webgl::ActiveTexture);
  NODE_SET_METHOD(target, "drawElements", webgl::DrawElements);
  NODE_SET_METHOD(target, "flush", webgl::Flush);
  NODE_SET_METHOD(target, "finish", webgl::Finish);

  NODE_SET_METHOD(target, "vertexAttrib1f", webgl::VertexAttrib1f);
  NODE_SET_METHOD(target, "vertexAttrib2f", webgl::VertexAttrib2f);
  NODE_SET_METHOD(target, "vertexAttrib3f", webgl::VertexAttrib3f);
  NODE_SET_METHOD(target, "vertexAttrib4f", webgl::VertexAttrib4f);
  NODE_SET_METHOD(target, "vertexAttrib1fv", webgl::VertexAttrib1fv);
  NODE_SET_METHOD(target, "vertexAttrib2fv", webgl::VertexAttrib2fv);
  NODE_SET_METHOD(target, "vertexAttrib3fv", webgl::VertexAttrib3fv);
  NODE_SET_METHOD(target, "vertexAttrib4fv", webgl::VertexAttrib4fv);

  NODE_SET_METHOD(target, "blendColor", webgl::BlendColor);
  NODE_SET_METHOD(target, "blendEquationSeparate", webgl::BlendEquationSeparate);
  NODE_SET_METHOD(target, "blendFuncSeparate", webgl::BlendFuncSeparate);
  NODE_SET_METHOD(target, "clearStencil", webgl::ClearStencil);
  NODE_SET_METHOD(target, "colorMask", webgl::ColorMask);
  NODE_SET_METHOD(target, "copyTexImage2D", webgl::CopyTexImage2D);
  NODE_SET_METHOD(target, "copyTexSubImage2D", webgl::CopyTexSubImage2D);
  NODE_SET_METHOD(target, "cullFace", webgl::CullFace);
  NODE_SET_METHOD(target, "depthMask", webgl::DepthMask);
  NODE_SET_METHOD(target, "depthRange", webgl::DepthRange);
  NODE_SET_METHOD(target, "disableVertexAttribArray", webgl::DisableVertexAttribArray);
  NODE_SET_METHOD(target, "hint", webgl::Hint);
  NODE_SET_METHOD(target, "isEnabled", webgl::IsEnabled);
  NODE_SET_METHOD(target, "lineWidth", webgl::LineWidth);
  NODE_SET_METHOD(target, "polygonOffset", webgl::PolygonOffset);

  NODE_SET_METHOD(target, "scissor", webgl::Scissor);
  NODE_SET_METHOD(target, "stencilFunc", webgl::StencilFunc);
  NODE_SET_METHOD(target, "stencilFuncSeparate", webgl::StencilFuncSeparate);
  NODE_SET_METHOD(target, "stencilMask", webgl::StencilMask);
  NODE_SET_METHOD(target, "stencilMaskSeparate", webgl::StencilMaskSeparate);
  NODE_SET_METHOD(target, "stencilOp", webgl::StencilOp);
  NODE_SET_METHOD(target, "stencilOpSeparate", webgl::StencilOpSeparate);
  NODE_SET_METHOD(target, "bindRenderbuffer", webgl::BindRenderbuffer);
  NODE_SET_METHOD(target, "createRenderbuffer", webgl::CreateRenderbuffer);

  NODE_SET_METHOD(target, "deleteBuffer", webgl::DeleteBuffer);
  NODE_SET_METHOD(target, "deleteFramebuffer", webgl::DeleteFramebuffer);
  NODE_SET_METHOD(target, "deleteProgram", webgl::DeleteProgram);
  NODE_SET_METHOD(target, "deleteRenderbuffer", webgl::DeleteRenderbuffer);
  NODE_SET_METHOD(target, "deleteShader", webgl::DeleteShader);
  NODE_SET_METHOD(target, "deleteTexture", webgl::DeleteTexture);
  NODE_SET_METHOD(target, "detachShader", webgl::DetachShader);
  NODE_SET_METHOD(target, "framebufferRenderbuffer", webgl::FramebufferRenderbuffer);
  NODE_SET_METHOD(target, "getVertexAttribOffset", webgl::GetVertexAttribOffset);

  NODE_SET_METHOD(target, "isBuffer", webgl::IsBuffer);
  NODE_SET_METHOD(target, "isFramebuffer", webgl::IsFramebuffer);
  NODE_SET_METHOD(target, "isProgram", webgl::IsProgram);
  NODE_SET_METHOD(target, "isRenderbuffer", webgl::IsRenderbuffer);
  NODE_SET_METHOD(target, "isShader", webgl::IsShader);
  NODE_SET_METHOD(target, "isTexture", webgl::IsTexture);

  NODE_SET_METHOD(target, "renderbufferStorage", webgl::RenderbufferStorage);
  NODE_SET_METHOD(target, "getShaderSource", webgl::GetShaderSource);
  NODE_SET_METHOD(target, "validateProgram", webgl::ValidateProgram);

  NODE_SET_METHOD(target, "texSubImage2D", webgl::TexSubImage2D);
  NODE_SET_METHOD(target, "readPixels", webgl::ReadPixels);
  NODE_SET_METHOD(target, "getTexParameter", webgl::GetTexParameter);
  NODE_SET_METHOD(target, "getActiveAttrib", webgl::GetActiveAttrib);
  NODE_SET_METHOD(target, "getActiveUniform", webgl::GetActiveUniform);
  NODE_SET_METHOD(target, "getAttachedShaders", webgl::GetAttachedShaders);
  NODE_SET_METHOD(target, "getParameter", webgl::GetParameter);
  NODE_SET_METHOD(target, "getBufferParameter", webgl::GetBufferParameter);
  NODE_SET_METHOD(target, "getFramebufferAttachmentParameter", webgl::GetFramebufferAttachmentParameter);
  NODE_SET_METHOD(target, "getProgramInfoLog", webgl::GetProgramInfoLog);
  NODE_SET_METHOD(target, "getRenderbufferParameter", webgl::GetRenderbufferParameter);
  NODE_SET_METHOD(target, "getVertexAttrib", webgl::GetVertexAttrib);
  NODE_SET_METHOD(target, "getSupportedExtensions", webgl::GetSupportedExtensions);
  NODE_SET_METHOD(target, "getExtension", webgl::GetExtension);
  NODE_SET_METHOD(target, "checkFramebufferStatus", webgl::CheckFramebufferStatus);

  NODE_SET_METHOD(target, "frontFace", webgl::FrontFace);

  // OpenGL ES 2.1 constants

  /* ClearBufferMask */
  JS_GL_CONSTANT(DEPTH_BUFFER_BIT);
  JS_GL_CONSTANT(STENCIL_BUFFER_BIT);
  JS_GL_CONSTANT(COLOR_BUFFER_BIT);

  /* Boolean */
  JS_GL_CONSTANT(FALSE);
  JS_GL_CONSTANT(TRUE);

  /* BeginMode */
  JS_GL_CONSTANT(POINTS);
  JS_GL_CONSTANT(LINES);
  JS_GL_CONSTANT(LINE_LOOP);
  JS_GL_CONSTANT(LINE_STRIP);
  JS_GL_CONSTANT(TRIANGLES);
  JS_GL_CONSTANT(TRIANGLE_STRIP);
  JS_GL_CONSTANT(TRIANGLE_FAN);

  /* AlphaFunction (not supported in ES20) */
  /*      GL_NEVER */
  /*      GL_LESS */
  /*      GL_EQUAL */
  /*      GL_LEQUAL */
  /*      GL_GREATER */
  /*      GL_NOTEQUAL */
  /*      GL_GEQUAL */
  /*      GL_ALWAYS */

  /* BlendingFactorDest */
  JS_GL_CONSTANT(ZERO);
  JS_GL_CONSTANT(ONE);
  JS_GL_CONSTANT(SRC_COLOR);
  JS_GL_CONSTANT(ONE_MINUS_SRC_COLOR);
  JS_GL_CONSTANT(SRC_ALPHA);
  JS_GL_CONSTANT(ONE_MINUS_SRC_ALPHA);
  JS_GL_CONSTANT(DST_ALPHA);
  JS_GL_CONSTANT(ONE_MINUS_DST_ALPHA);

  /* BlendingFactorSrc */
  /*      GL_ZERO */
  /*      GL_ONE */
  JS_GL_CONSTANT(DST_COLOR);
  JS_GL_CONSTANT(ONE_MINUS_DST_COLOR);
  JS_GL_CONSTANT(SRC_ALPHA_SATURATE);
  /*      GL_SRC_ALPHA */
  /*      GL_ONE_MINUS_SRC_ALPHA */
  /*      GL_DST_ALPHA */
  /*      GL_ONE_MINUS_DST_ALPHA */

  /* BlendEquationSeparate */
  JS_GL_CONSTANT(FUNC_ADD);
  JS_GL_CONSTANT(BLEND_EQUATION);
  JS_GL_CONSTANT(BLEND_EQUATION_RGB);    /* same as BLEND_EQUATION */
  JS_GL_CONSTANT(BLEND_EQUATION_ALPHA);

  /* BlendSubtract */
  JS_GL_CONSTANT(FUNC_SUBTRACT);
  JS_GL_CONSTANT(FUNC_REVERSE_SUBTRACT);

  /* Separate Blend Functions */
  JS_GL_CONSTANT(BLEND_DST_RGB);
  JS_GL_CONSTANT(BLEND_SRC_RGB);
  JS_GL_CONSTANT(BLEND_DST_ALPHA);
  JS_GL_CONSTANT(BLEND_SRC_ALPHA);
  JS_GL_CONSTANT(CONSTANT_COLOR);
  JS_GL_CONSTANT(ONE_MINUS_CONSTANT_COLOR);
  JS_GL_CONSTANT(CONSTANT_ALPHA);
  JS_GL_CONSTANT(ONE_MINUS_CONSTANT_ALPHA);
  JS_GL_CONSTANT(BLEND_COLOR);

  /* Buffer Objects */
  JS_GL_CONSTANT(ARRAY_BUFFER);
  JS_GL_CONSTANT(ELEMENT_ARRAY_BUFFER);
  JS_GL_CONSTANT(ARRAY_BUFFER_BINDING);
  JS_GL_CONSTANT(ELEMENT_ARRAY_BUFFER_BINDING);

  JS_GL_CONSTANT(STREAM_DRAW);
  JS_GL_CONSTANT(STATIC_DRAW);
  JS_GL_CONSTANT(DYNAMIC_DRAW);

  JS_GL_CONSTANT(BUFFER_SIZE);
  JS_GL_CONSTANT(BUFFER_USAGE);

  JS_GL_CONSTANT(CURRENT_VERTEX_ATTRIB);

  /* CullFaceMode */
  JS_GL_CONSTANT(FRONT);
  JS_GL_CONSTANT(BACK);
  JS_GL_CONSTANT(FRONT_AND_BACK);

  /* DepthFunction */
  /*      GL_NEVER */
  /*      GL_LESS */
  /*      GL_EQUAL */
  /*      GL_LEQUAL */
  /*      GL_GREATER */
  /*      GL_NOTEQUAL */
  /*      GL_GEQUAL */
  /*      GL_ALWAYS */

  /* EnableCap */
  JS_GL_CONSTANT(TEXTURE_2D);
  JS_GL_CONSTANT(CULL_FACE);
  JS_GL_CONSTANT(BLEND);
  JS_GL_CONSTANT(DITHER);
  JS_GL_CONSTANT(STENCIL_TEST);
  JS_GL_CONSTANT(DEPTH_TEST);
  JS_GL_CONSTANT(SCISSOR_TEST);
  JS_GL_CONSTANT(POLYGON_OFFSET_FILL);
  JS_GL_CONSTANT(SAMPLE_ALPHA_TO_COVERAGE);
  JS_GL_CONSTANT(SAMPLE_COVERAGE);

  /* ErrorCode */
  JS_GL_CONSTANT(NO_ERROR);
  JS_GL_CONSTANT(INVALID_ENUM);
  JS_GL_CONSTANT(INVALID_VALUE);
  JS_GL_CONSTANT(INVALID_OPERATION);
  JS_GL_CONSTANT(OUT_OF_MEMORY);

  /* FrontFaceDirection */
  JS_GL_CONSTANT(CW);
  JS_GL_CONSTANT(CCW);

  /* GetPName */
  JS_GL_CONSTANT(LINE_WIDTH);
  JS_GL_CONSTANT(ALIASED_POINT_SIZE_RANGE);
  JS_GL_CONSTANT(ALIASED_LINE_WIDTH_RANGE);
  JS_GL_CONSTANT(CULL_FACE_MODE);
  JS_GL_CONSTANT(FRONT_FACE);
  JS_GL_CONSTANT(DEPTH_RANGE);
  JS_GL_CONSTANT(DEPTH_WRITEMASK);
  JS_GL_CONSTANT(DEPTH_CLEAR_VALUE);
  JS_GL_CONSTANT(DEPTH_FUNC);
  JS_GL_CONSTANT(STENCIL_CLEAR_VALUE);
  JS_GL_CONSTANT(STENCIL_FUNC);
  JS_GL_CONSTANT(STENCIL_FAIL);
  JS_GL_CONSTANT(STENCIL_PASS_DEPTH_FAIL);
  JS_GL_CONSTANT(STENCIL_PASS_DEPTH_PASS);
  JS_GL_CONSTANT(STENCIL_REF);
  JS_GL_CONSTANT(STENCIL_VALUE_MASK);
  JS_GL_CONSTANT(STENCIL_WRITEMASK);
  JS_GL_CONSTANT(STENCIL_BACK_FUNC);
  JS_GL_CONSTANT(STENCIL_BACK_FAIL);
  JS_GL_CONSTANT(STENCIL_BACK_PASS_DEPTH_FAIL);
  JS_GL_CONSTANT(STENCIL_BACK_PASS_DEPTH_PASS);
  JS_GL_CONSTANT(STENCIL_BACK_REF);
  JS_GL_CONSTANT(STENCIL_BACK_VALUE_MASK);
  JS_GL_CONSTANT(STENCIL_BACK_WRITEMASK);
  JS_GL_CONSTANT(VIEWPORT);
  JS_GL_CONSTANT(SCISSOR_BOX);
  /*      GL_SCISSOR_TEST */
  JS_GL_CONSTANT(COLOR_CLEAR_VALUE);
  JS_GL_CONSTANT(COLOR_WRITEMASK);
  JS_GL_CONSTANT(UNPACK_ALIGNMENT);
  JS_GL_CONSTANT(PACK_ALIGNMENT);
  JS_GL_CONSTANT(MAX_TEXTURE_SIZE);
  JS_GL_CONSTANT(MAX_VIEWPORT_DIMS);
  JS_GL_CONSTANT(SUBPIXEL_BITS);
  JS_GL_CONSTANT(RED_BITS);
  JS_GL_CONSTANT(GREEN_BITS);
  JS_GL_CONSTANT(BLUE_BITS);
  JS_GL_CONSTANT(ALPHA_BITS);
  JS_GL_CONSTANT(DEPTH_BITS);
  JS_GL_CONSTANT(STENCIL_BITS);
  JS_GL_CONSTANT(POLYGON_OFFSET_UNITS);
  /*      GL_POLYGON_OFFSET_FILL */
  JS_GL_CONSTANT(POLYGON_OFFSET_FACTOR);
  JS_GL_CONSTANT(TEXTURE_BINDING_2D);
  JS_GL_CONSTANT(SAMPLE_BUFFERS);
  JS_GL_CONSTANT(SAMPLES);
  JS_GL_CONSTANT(SAMPLE_COVERAGE_VALUE);
  JS_GL_CONSTANT(SAMPLE_COVERAGE_INVERT);

  /* GetTextureParameter */
  /*      GL_TEXTURE_MAG_FILTER */
  /*      GL_TEXTURE_MIN_FILTER */
  /*      GL_TEXTURE_WRAP_S */
  /*      GL_TEXTURE_WRAP_T */

  JS_GL_CONSTANT(NUM_COMPRESSED_TEXTURE_FORMATS);
  JS_GL_CONSTANT(COMPRESSED_TEXTURE_FORMATS);

  /* HintMode */
  JS_GL_CONSTANT(DONT_CARE);
  JS_GL_CONSTANT(FASTEST);
  JS_GL_CONSTANT(NICEST);

  /* HintTarget */
  JS_GL_CONSTANT(GENERATE_MIPMAP_HINT);

  /* DataType */
  JS_GL_CONSTANT(BYTE);
  JS_GL_CONSTANT(UNSIGNED_BYTE);
  JS_GL_CONSTANT(SHORT);
  JS_GL_CONSTANT(UNSIGNED_SHORT);
  JS_GL_CONSTANT(INT);
  JS_GL_CONSTANT(UNSIGNED_INT);
  JS_GL_CONSTANT(FLOAT);
#ifndef __APPLE__
  JS_GL_CONSTANT(FIXED);
#endif

  /* PixelFormat */
  JS_GL_CONSTANT(DEPTH_COMPONENT);
  JS_GL_CONSTANT(ALPHA);
  JS_GL_CONSTANT(RGB);
  JS_GL_CONSTANT(RGBA);
  JS_GL_CONSTANT(LUMINANCE);
  JS_GL_CONSTANT(LUMINANCE_ALPHA);

  /* PixelType */
  /*      GL_UNSIGNED_BYTE */
  JS_GL_CONSTANT(UNSIGNED_SHORT_4_4_4_4);
  JS_GL_CONSTANT(UNSIGNED_SHORT_5_5_5_1);
  JS_GL_CONSTANT(UNSIGNED_SHORT_5_6_5);

  /* Shaders */
  JS_GL_CONSTANT(FRAGMENT_SHADER);
  JS_GL_CONSTANT(VERTEX_SHADER);
  JS_GL_CONSTANT(MAX_VERTEX_ATTRIBS);
#ifndef __APPLE__
  JS_GL_CONSTANT(MAX_VERTEX_UNIFORM_VECTORS);
  JS_GL_CONSTANT(MAX_VARYING_VECTORS);
#endif
  JS_GL_CONSTANT(MAX_COMBINED_TEXTURE_IMAGE_UNITS);
  JS_GL_CONSTANT(MAX_VERTEX_TEXTURE_IMAGE_UNITS);
  JS_GL_CONSTANT(MAX_TEXTURE_IMAGE_UNITS);
#ifndef __APPLE__
  JS_GL_CONSTANT(MAX_FRAGMENT_UNIFORM_VECTORS);
#endif
  JS_GL_CONSTANT(SHADER_TYPE);
  JS_GL_CONSTANT(DELETE_STATUS);
  JS_GL_CONSTANT(LINK_STATUS);
  JS_GL_CONSTANT(VALIDATE_STATUS);
  JS_GL_CONSTANT(ATTACHED_SHADERS);
  JS_GL_CONSTANT(ACTIVE_UNIFORMS);
  JS_GL_CONSTANT(ACTIVE_UNIFORM_MAX_LENGTH);
  JS_GL_CONSTANT(ACTIVE_ATTRIBUTES);
  JS_GL_CONSTANT(ACTIVE_ATTRIBUTE_MAX_LENGTH);
  JS_GL_CONSTANT(SHADING_LANGUAGE_VERSION);
  JS_GL_CONSTANT(CURRENT_PROGRAM);

  /* StencilFunction */
  JS_GL_CONSTANT(NEVER);
  JS_GL_CONSTANT(LESS);
  JS_GL_CONSTANT(EQUAL);
  JS_GL_CONSTANT(LEQUAL);
  JS_GL_CONSTANT(GREATER);
  JS_GL_CONSTANT(NOTEQUAL);
  JS_GL_CONSTANT(GEQUAL);
  JS_GL_CONSTANT(ALWAYS);

  /* StencilOp */
  /*      GL_ZERO */
  JS_GL_CONSTANT(KEEP);
  JS_GL_CONSTANT(REPLACE);
  JS_GL_CONSTANT(INCR);
  JS_GL_CONSTANT(DECR);
  JS_GL_CONSTANT(INVERT);
  JS_GL_CONSTANT(INCR_WRAP);
  JS_GL_CONSTANT(DECR_WRAP);

  /* StringName */
  JS_GL_CONSTANT(VENDOR);
  JS_GL_CONSTANT(RENDERER);
  JS_GL_CONSTANT(VERSION);
  JS_GL_CONSTANT(EXTENSIONS);

  /* TextureMagFilter */
  JS_GL_CONSTANT(NEAREST);
  JS_GL_CONSTANT(LINEAR);

  /* TextureMinFilter */
  /*      GL_NEAREST */
  /*      GL_LINEAR */
  JS_GL_CONSTANT(NEAREST_MIPMAP_NEAREST);
  JS_GL_CONSTANT(LINEAR_MIPMAP_NEAREST);
  JS_GL_CONSTANT(NEAREST_MIPMAP_LINEAR);
  JS_GL_CONSTANT(LINEAR_MIPMAP_LINEAR);

  /* TextureParameterName */
  JS_GL_CONSTANT(TEXTURE_MAG_FILTER);
  JS_GL_CONSTANT(TEXTURE_MIN_FILTER);
  JS_GL_CONSTANT(TEXTURE_WRAP_S);
  JS_GL_CONSTANT(TEXTURE_WRAP_T);

  /* TextureTarget */
  /*      GL_TEXTURE_2D */
  JS_GL_CONSTANT(TEXTURE);

  JS_GL_CONSTANT(TEXTURE_CUBE_MAP);
  JS_GL_CONSTANT(TEXTURE_BINDING_CUBE_MAP);
  JS_GL_CONSTANT(TEXTURE_CUBE_MAP_POSITIVE_X);
  JS_GL_CONSTANT(TEXTURE_CUBE_MAP_NEGATIVE_X);
  JS_GL_CONSTANT(TEXTURE_CUBE_MAP_POSITIVE_Y);
  JS_GL_CONSTANT(TEXTURE_CUBE_MAP_NEGATIVE_Y);
  JS_GL_CONSTANT(TEXTURE_CUBE_MAP_POSITIVE_Z);
  JS_GL_CONSTANT(TEXTURE_CUBE_MAP_NEGATIVE_Z);
  JS_GL_CONSTANT(MAX_CUBE_MAP_TEXTURE_SIZE);

  /* TextureUnit */
  JS_GL_CONSTANT(TEXTURE0);
  JS_GL_CONSTANT(TEXTURE1);
  JS_GL_CONSTANT(TEXTURE2);
  JS_GL_CONSTANT(TEXTURE3);
  JS_GL_CONSTANT(TEXTURE4);
  JS_GL_CONSTANT(TEXTURE5);
  JS_GL_CONSTANT(TEXTURE6);
  JS_GL_CONSTANT(TEXTURE7);
  JS_GL_CONSTANT(TEXTURE8);
  JS_GL_CONSTANT(TEXTURE9);
  JS_GL_CONSTANT(TEXTURE10);
  JS_GL_CONSTANT(TEXTURE11);
  JS_GL_CONSTANT(TEXTURE12);
  JS_GL_CONSTANT(TEXTURE13);
  JS_GL_CONSTANT(TEXTURE14);
  JS_GL_CONSTANT(TEXTURE15);
  JS_GL_CONSTANT(TEXTURE16);
  JS_GL_CONSTANT(TEXTURE17);
  JS_GL_CONSTANT(TEXTURE18);
  JS_GL_CONSTANT(TEXTURE19);
  JS_GL_CONSTANT(TEXTURE20);
  JS_GL_CONSTANT(TEXTURE21);
  JS_GL_CONSTANT(TEXTURE22);
  JS_GL_CONSTANT(TEXTURE23);
  JS_GL_CONSTANT(TEXTURE24);
  JS_GL_CONSTANT(TEXTURE25);
  JS_GL_CONSTANT(TEXTURE26);
  JS_GL_CONSTANT(TEXTURE27);
  JS_GL_CONSTANT(TEXTURE28);
  JS_GL_CONSTANT(TEXTURE29);
  JS_GL_CONSTANT(TEXTURE30);
  JS_GL_CONSTANT(TEXTURE31);
  JS_GL_CONSTANT(ACTIVE_TEXTURE);

  /* TextureWrapMode */
  JS_GL_CONSTANT(REPEAT);
  JS_GL_CONSTANT(CLAMP_TO_EDGE);
  JS_GL_CONSTANT(MIRRORED_REPEAT);

  /* Uniform Types */
  JS_GL_CONSTANT(FLOAT_VEC2);
  JS_GL_CONSTANT(FLOAT_VEC3);
  JS_GL_CONSTANT(FLOAT_VEC4);
  JS_GL_CONSTANT(INT_VEC2);
  JS_GL_CONSTANT(INT_VEC3);
  JS_GL_CONSTANT(INT_VEC4);
  JS_GL_CONSTANT(BOOL);
  JS_GL_CONSTANT(BOOL_VEC2);
  JS_GL_CONSTANT(BOOL_VEC3);
  JS_GL_CONSTANT(BOOL_VEC4);
  JS_GL_CONSTANT(FLOAT_MAT2);
  JS_GL_CONSTANT(FLOAT_MAT3);
  JS_GL_CONSTANT(FLOAT_MAT4);
  JS_GL_CONSTANT(SAMPLER_2D);
  JS_GL_CONSTANT(SAMPLER_CUBE);

  /* Vertex Arrays */
  JS_GL_CONSTANT(VERTEX_ATTRIB_ARRAY_ENABLED);
  JS_GL_CONSTANT(VERTEX_ATTRIB_ARRAY_SIZE);
  JS_GL_CONSTANT(VERTEX_ATTRIB_ARRAY_STRIDE);
  JS_GL_CONSTANT(VERTEX_ATTRIB_ARRAY_TYPE);
  JS_GL_CONSTANT(VERTEX_ATTRIB_ARRAY_NORMALIZED);
  JS_GL_CONSTANT(VERTEX_ATTRIB_ARRAY_POINTER);
  JS_GL_CONSTANT(VERTEX_ATTRIB_ARRAY_BUFFER_BINDING);

  /* Read Format */
#ifndef __APPLE__
  JS_GL_CONSTANT(IMPLEMENTATION_COLOR_READ_TYPE);
  JS_GL_CONSTANT(IMPLEMENTATION_COLOR_READ_FORMAT);
#endif

  /* Shader Source */
  JS_GL_CONSTANT(COMPILE_STATUS);
  JS_GL_CONSTANT(INFO_LOG_LENGTH);
  JS_GL_CONSTANT(SHADER_SOURCE_LENGTH);
#ifndef __APPLE__
  JS_GL_CONSTANT(SHADER_COMPILER);
#endif

  /* Shader Binary */
#ifndef __APPLE__
  JS_GL_CONSTANT(SHADER_BINARY_FORMATS);
  JS_GL_CONSTANT(NUM_SHADER_BINARY_FORMATS);
#endif

  /* Shader Precision-Specified Types */
#ifndef __APPLE__
  JS_GL_CONSTANT(LOW_FLOAT);
  JS_GL_CONSTANT(MEDIUM_FLOAT);
  JS_GL_CONSTANT(HIGH_FLOAT);
  JS_GL_CONSTANT(LOW_INT);
  JS_GL_CONSTANT(MEDIUM_INT);
  JS_GL_CONSTANT(HIGH_INT);
#endif

  /* Framebuffer Object. */
  JS_GL_CONSTANT(FRAMEBUFFER);
  JS_GL_CONSTANT(RENDERBUFFER);

  JS_GL_CONSTANT(RGBA4);
  JS_GL_CONSTANT(RGB5_A1);
#ifndef __APPLE__
  //JS_GL_CONSTANT(RGB565);
#endif
  JS_GL_CONSTANT(DEPTH_COMPONENT16);
  JS_GL_CONSTANT(STENCIL_INDEX);
  JS_GL_CONSTANT(STENCIL_INDEX8);
  JS_GL_CONSTANT(DEPTH_STENCIL);
  JS_GL_CONSTANT(DEPTH24_STENCIL8);

  JS_GL_CONSTANT(RENDERBUFFER_WIDTH);
  JS_GL_CONSTANT(RENDERBUFFER_HEIGHT);
  JS_GL_CONSTANT(RENDERBUFFER_INTERNAL_FORMAT);
  JS_GL_CONSTANT(RENDERBUFFER_RED_SIZE);
  JS_GL_CONSTANT(RENDERBUFFER_GREEN_SIZE);
  JS_GL_CONSTANT(RENDERBUFFER_BLUE_SIZE);
  JS_GL_CONSTANT(RENDERBUFFER_ALPHA_SIZE);
  JS_GL_CONSTANT(RENDERBUFFER_DEPTH_SIZE);
  JS_GL_CONSTANT(RENDERBUFFER_STENCIL_SIZE);

  JS_GL_CONSTANT(FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE);
  JS_GL_CONSTANT(FRAMEBUFFER_ATTACHMENT_OBJECT_NAME);
  JS_GL_CONSTANT(FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL);
  JS_GL_CONSTANT(FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE);

  JS_GL_CONSTANT(COLOR_ATTACHMENT0);
  JS_GL_CONSTANT(DEPTH_ATTACHMENT);
  JS_GL_CONSTANT(STENCIL_ATTACHMENT);
  JS_GL_CONSTANT(DEPTH_STENCIL_ATTACHMENT);

  JS_GL_CONSTANT(NONE);

  JS_GL_CONSTANT(FRAMEBUFFER_COMPLETE);
  JS_GL_CONSTANT(FRAMEBUFFER_INCOMPLETE_ATTACHMENT);
  JS_GL_CONSTANT(FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT);
#ifndef __APPLE__
  //JS_GL_CONSTANT(FRAMEBUFFER_INCOMPLETE_DIMENSIONS);
#endif
  JS_GL_CONSTANT(FRAMEBUFFER_UNSUPPORTED);

  JS_GL_CONSTANT(FRAMEBUFFER_BINDING);
  JS_GL_CONSTANT(RENDERBUFFER_BINDING);
  JS_GL_CONSTANT(MAX_RENDERBUFFER_SIZE);

  JS_GL_CONSTANT(INVALID_FRAMEBUFFER_OPERATION);

  /* WebGL-specific enums */
  target->Set(JS_STR( "UNPACK_FLIP_Y_WEBGL" ), JS_INT(0x9240));
  target->Set(JS_STR( "UNPACK_PREMULTIPLY_ALPHA_WEBGL" ), JS_INT(0x9241));
  target->Set(JS_STR( "CONTEXT_LOST_WEBGL" ), JS_INT(0x9242));
  target->Set(JS_STR( "UNPACK_COLORSPACE_CONVERSION_WEBGL" ), JS_INT(0x9243));
  target->Set(JS_STR( "BROWSER_DEFAULT_WEBGL" ), JS_INT(0x9244));

  //////////////////////////////
  // NOT in WebGL spec
  //////////////////////////////

  // PBO
  target->Set(JS_STR( "PIXEL_PACK_BUFFER" ), JS_INT(0x88EB));
  target->Set(JS_STR( "PIXEL_UNPACK_BUFFER" ), JS_INT(0x88EC));
  target->Set(JS_STR( "PIXEL_PACK_BUFFER_BINDING" ), JS_INT(0x88ED));
  target->Set(JS_STR( "PIXEL_UNPACK_BUFFER_BINDING" ), JS_INT(0x88EF));
}

NODE_MODULE(webgl, init)
} // extern "C"
