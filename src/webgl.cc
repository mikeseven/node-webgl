#include <cstring>
#include <vector>
#include <iostream>

#include "webgl.h"
#include "image.h"
#include <node.h>
#include <node_buffer.h>
#include <GL/glew.h>

#ifdef _WIN32
  #define  strcasestr(s, t) strstr(strupr(s), strupr(t))
#endif

namespace webgl {

using namespace node;
using namespace v8;
using namespace std;

// forward declarations
enum GLObjectType {
  GLOBJECT_TYPE_BUFFER,
  GLOBJECT_TYPE_FRAMEBUFFER,
  GLOBJECT_TYPE_PROGRAM,
  GLOBJECT_TYPE_RENDERBUFFER,
  GLOBJECT_TYPE_SHADER,
  GLOBJECT_TYPE_TEXTURE,
};

void registerGLObj(GLObjectType type, GLuint obj);
void unregisterGLObj(GLuint obj);

v8::Handle<v8::Value> ThrowError(const char* msg) {
    NanThrowError(NanNew<String>(msg));
}

// A 32-bit and 64-bit compatible way of converting a pointer to a GLuint.
static GLuint ToGLuint(const void* ptr) {
  return static_cast<GLuint>(reinterpret_cast<size_t>(ptr));
}

static int SizeOfArrayElementForType(v8::ExternalArrayType type) {
  switch (type) {
  case v8::kExternalByteArray:
  case v8::kExternalUnsignedByteArray:
    return 1;
  case v8::kExternalShortArray:
  case v8::kExternalUnsignedShortArray:
    return 2;
  case v8::kExternalIntArray:
  case v8::kExternalUnsignedIntArray:
  case v8::kExternalFloatArray:
    return 4;
  default:
    return 0;
  }
}

inline void *getImageData(Local<Value> arg) {
  void *pixels = NULL;
  if (!arg->IsNull()) {
    Local<Object> obj = Local<Object>::Cast(arg);
    if (!obj->IsObject())
      NanThrowError("Bad texture argument");

    pixels = obj->GetIndexedPropertiesExternalArrayData();
  }
  return pixels;
}

NAN_WEAK_CALLBACK(ExternalCallback) {
  data.GetValue()->Neuter();
  delete[] static_cast<char*>(data.GetParameter());
}

template <typename Type>
inline Type* getArrayData(Local<Value> arg, int* num = NULL);

template<>
inline GLfloat* getArrayData<GLfloat>(Local<Value> arg, int* num) {
  GLfloat *data=NULL;
  if(num) *num=0;

  if(!arg->IsNull()) {
    if(arg->IsArray()) {
      Local<Array> arr = Local<Array>::Cast(arg);
      if(num) *num=arr->Length();
      data = reinterpret_cast<GLfloat*>(arr->GetIndexedPropertiesExternalArrayData());
    }
    else if(arg->IsArrayBufferView()) {
      Local<ArrayBufferView> obj = arg.As<ArrayBufferView>();
      Local<ArrayBuffer> buffer = obj->Buffer();
      if (!buffer->IsExternal()) {
          ArrayBuffer::Contents contents = buffer->Externalize();
          obj->SetIndexedPropertiesToExternalArrayData(contents.Data(), kExternalFloat32Array, contents.ByteLength() / 4);
          NanMakeWeakPersistent(buffer, contents.Data(), ExternalCallback);
      }
      if(num) *num = obj->GetIndexedPropertiesExternalArrayDataLength();
      data = reinterpret_cast<GLfloat*>(obj->GetIndexedPropertiesExternalArrayData());

    }
    else {
      NanThrowError("Bad array argument");
      return NULL;
    }
  }

  return data;
}

template<>
inline GLint* getArrayData<GLint>(Local<Value> arg, int* num) {
  GLint *data=NULL;
  if(num) *num=0;

  if(!arg->IsNull()) {
    if(arg->IsArray()) {
      Local<Array> arr = Local<Array>::Cast(arg);
      if(num) *num=arr->Length();
      data = reinterpret_cast<GLint*>(arr->GetIndexedPropertiesExternalArrayData());
    }
    else if(arg->IsArrayBufferView()) {
      Local<ArrayBufferView> obj = arg.As<ArrayBufferView>();
      Local<ArrayBuffer> buffer = obj->Buffer();
      if (!buffer->IsExternal()) {
          ArrayBuffer::Contents contents = buffer->Externalize();
          obj->SetIndexedPropertiesToExternalArrayData(contents.Data(), kExternalInt32Array, contents.ByteLength() / 4);
          NanMakeWeakPersistent(buffer, contents.Data(), ExternalCallback);
      }
      if(num) *num = obj->GetIndexedPropertiesExternalArrayDataLength();
      data = reinterpret_cast<GLint*>(obj->GetIndexedPropertiesExternalArrayData());

    }
    else {
      NanThrowError("Bad array argument");
      return NULL;
    }
  }

  return data;
}

NAN_METHOD(Init) {
  NanScope();
  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
    /* Problem: glewInit failed, something is seriously wrong. */
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    NanReturnValue(JS_INT(-1));
  }
  //fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
  NanReturnValue(JS_INT(0));
}

NAN_METHOD(Uniform1f) {
  NanScope();

  int location = args[0]->Int32Value();
  float x = (float) args[1]->NumberValue();

  glUniform1f(location, x);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(Uniform2f) {
  NanScope();

  int location = args[0]->Int32Value();
  float x = (float) args[1]->NumberValue();
  float y = (float) args[2]->NumberValue();

  glUniform2f(location, x, y);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(Uniform3f) {
  NanScope();

  int location = args[0]->Int32Value();
  float x = (float) args[1]->NumberValue();
  float y = (float) args[2]->NumberValue();
  float z = (float) args[3]->NumberValue();

  glUniform3f(location, x, y, z);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(Uniform4f) {
  NanScope();

  int location = args[0]->Int32Value();
  float x = (float) args[1]->NumberValue();
  float y = (float) args[2]->NumberValue();
  float z = (float) args[3]->NumberValue();
  float w = (float) args[4]->NumberValue();

  glUniform4f(location, x, y, z, w);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(Uniform1i) {
  NanScope();

  int location = args[0]->Int32Value();
  int x = args[1]->Int32Value();

  glUniform1i(location, x);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(Uniform2i) {
  NanScope();

  int location = args[0]->Int32Value();
  int x = args[1]->Int32Value();
  int y = args[2]->Int32Value();

  glUniform2i(location, x, y);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(Uniform3i) {
  NanScope();

  int location = args[0]->Int32Value();
  int x = args[1]->Int32Value();
  int y = args[2]->Int32Value();
  int z = args[3]->Int32Value();

  glUniform3i(location, x, y, z);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(Uniform4i) {
  NanScope();

  int location = args[0]->Int32Value();
  int x = args[1]->Int32Value();
  int y = args[2]->Int32Value();
  int z = args[3]->Int32Value();
  int w = args[4]->Int32Value();

  glUniform4i(location, x, y, z, w);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(Uniform1fv) {
  NanScope();

  int location = args[0]->Int32Value();
  int num=0;
  GLfloat *ptr=getArrayData<GLfloat>(args[1],&num);
  glUniform1fv(location, num, ptr);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(Uniform2fv) {
  NanScope();

  int location = args[0]->Int32Value();
  int num=0;
  GLfloat *ptr=getArrayData<GLfloat>(args[1],&num);
  num /= 2;

  glUniform2fv(location, num, ptr);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(Uniform3fv) {
  NanScope();

  int location = args[0]->Int32Value();
  int num=0;
  GLfloat *ptr=getArrayData<GLfloat>(args[1],&num);
  num /= 3;

  glUniform3fv(location, num, ptr);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(Uniform4fv) {
  NanScope();

  int location = args[0]->Int32Value();
  int num=0;
  GLfloat *ptr=getArrayData<GLfloat>(args[1],&num);
  num /= 4;

  glUniform4fv(location, num, ptr);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(Uniform1iv) {
  NanScope();

  int location = args[0]->Int32Value();
  int num=0;
  GLint *ptr=getArrayData<GLint>(args[1],&num);

  glUniform1iv(location, num, ptr);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(Uniform2iv) {
  NanScope();

  int location = args[0]->Int32Value();
  int num=0;
  GLint *ptr=getArrayData<GLint>(args[1],&num);
  num /= 2;

  glUniform2iv(location, num, ptr);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(Uniform3iv) {
  NanScope();

  int location = args[0]->Int32Value();
  int num=0;
  GLint *ptr=getArrayData<GLint>(args[1],&num);
  num /= 3;
  glUniform3iv(location, num, ptr);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(Uniform4iv) {
  NanScope();

  int location = args[0]->Int32Value();
  int num=0;
  GLint *ptr=getArrayData<GLint>(args[1],&num);
  num /= 4;
  glUniform4iv(location, num, ptr);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(PixelStorei) {
  NanScope();

  int pname = args[0]->Int32Value();
  int param = args[1]->Int32Value();

  glPixelStorei(pname,param);

  NanReturnValue(NanUndefined());
}

NAN_METHOD(BindAttribLocation) {
  NanScope();

  int program = args[0]->Int32Value();
  int index = args[1]->Int32Value();
  String::Utf8Value name(args[2]);

  glBindAttribLocation(program, index, *name);

  NanReturnValue(NanUndefined());
}


NAN_METHOD(GetError) {
  NanScope();

  NanReturnValue(NanNew<Number>(glGetError()));
}


NAN_METHOD(DrawArrays) {
  NanScope();

  int mode = args[0]->Int32Value();
  int first = args[1]->Int32Value();
  int count = args[2]->Int32Value();

  glDrawArrays(mode, first, count);

  NanReturnValue(NanUndefined());
}

NAN_METHOD(UniformMatrix2fv) {
  NanScope();

  GLint location = args[0]->Int32Value();
  GLboolean transpose = args[1]->BooleanValue();

  GLsizei count=0;
  GLfloat* data=getArrayData<GLfloat>(args[2],&count);

  if (count < 4) {
    NanThrowError("Not enough data for UniformMatrix2fv");
  }

  glUniformMatrix2fv(location, count / 4, transpose, data);

  NanReturnValue(NanUndefined());
}

NAN_METHOD(UniformMatrix3fv) {
  NanScope();

  GLint location = args[0]->Int32Value();
  GLboolean transpose = args[1]->BooleanValue();
  GLsizei count=0;
  GLfloat* data=getArrayData<GLfloat>(args[2],&count);

  if (count < 9) {
    NanThrowError("Not enough data for UniformMatrix3fv");
  }

  glUniformMatrix3fv(location, count / 9, transpose, data);

  NanReturnValue(NanUndefined());
}

NAN_METHOD(UniformMatrix4fv) {
  NanScope();

  GLint location = args[0]->Int32Value();
  GLboolean transpose = args[1]->BooleanValue();
  GLsizei count=0;
  GLfloat* data=getArrayData<GLfloat>(args[2],&count);

  if (count < 16) {
    NanThrowError("Not enough data for UniformMatrix4fv");
  }

  glUniformMatrix4fv(location, count / 16, transpose, data);

  NanReturnValue(NanUndefined());
}

NAN_METHOD(GenerateMipmap) {
  NanScope();

  GLint target = args[0]->Int32Value();
  glGenerateMipmap(target);

  NanReturnValue(NanUndefined());
}

NAN_METHOD(GetAttribLocation) {
  NanScope();

  int program = args[0]->Int32Value();
  String::Utf8Value name(args[1]);

  NanReturnValue(NanNew<Number>(glGetAttribLocation(program, *name)));
}


NAN_METHOD(DepthFunc) {
  NanScope();

  glDepthFunc(args[0]->Int32Value());

  NanReturnValue(NanUndefined());
}


NAN_METHOD(Viewport) {
  NanScope();

  int x = args[0]->Int32Value();
  int y = args[1]->Int32Value();
  int width = args[2]->Int32Value();
  int height = args[3]->Int32Value();

  glViewport(x, y, width, height);

  NanReturnValue(NanUndefined());
}

NAN_METHOD(CreateShader) {
  NanScope();

  GLuint shader=glCreateShader(args[0]->Int32Value());
  #ifdef LOGGING
  cout<<"createShader "<<shader<<endl;
  #endif
  registerGLObj(GLOBJECT_TYPE_SHADER, shader);
  NanReturnValue(NanNew<Number>(shader));
}


NAN_METHOD(ShaderSource) {
  NanScope();

  int id = args[0]->Int32Value();
  String::Utf8Value code(args[1]);

  const char* codes[1];
  codes[0] = *code;
  GLint length=code.length();

  glShaderSource  (id, 1, codes, &length);

  NanReturnValue(NanUndefined());
}


NAN_METHOD(CompileShader) {
  NanScope();

  glCompileShader(args[0]->Int32Value());

  NanReturnValue(NanUndefined());
}

NAN_METHOD(FrontFace) {
  NanScope();

  glFrontFace(args[0]->Int32Value());

  NanReturnValue(NanUndefined());
}


NAN_METHOD(GetShaderParameter) {
  NanScope();

  int shader = args[0]->Int32Value();
  int pname = args[1]->Int32Value();
  int value = 0;
  switch (pname) {
  case GL_DELETE_STATUS:
  case GL_COMPILE_STATUS:
    glGetShaderiv(shader, pname, &value);
    NanReturnValue(JS_BOOL(static_cast<bool>(value!=0)));
  case GL_SHADER_TYPE:
    glGetShaderiv(shader, pname, &value);
    NanReturnValue(JS_FLOAT(static_cast<unsigned long>(value)));
  case GL_INFO_LOG_LENGTH:
  case GL_SHADER_SOURCE_LENGTH:
    glGetShaderiv(shader, pname, &value);
    NanReturnValue(JS_FLOAT(static_cast<long>(value)));
  default:
    NanThrowTypeError("GetShaderParameter: Invalid Enum");
  }
  NanReturnUndefined();
}

NAN_METHOD(GetShaderInfoLog) {
  NanScope();

  int id = args[0]->Int32Value();
  int Len = 1024;
  char Error[1024];
  glGetShaderInfoLog(id, 1024, &Len, Error);

  NanReturnValue(NanNew<String>(Error));
}


NAN_METHOD(CreateProgram) {
  NanScope();

  GLuint program=glCreateProgram();
  #ifdef LOGGING
  cout<<"createProgram "<<program<<endl;
  #endif
  registerGLObj(GLOBJECT_TYPE_PROGRAM, program);
  NanReturnValue(NanNew<Number>(program));
}


NAN_METHOD(AttachShader) {
  NanScope();

  int program = args[0]->Int32Value();
  int shader = args[1]->Int32Value();

  glAttachShader(program, shader);

  NanReturnValue(NanUndefined());
}


NAN_METHOD(LinkProgram) {
  NanScope();

  glLinkProgram(args[0]->Int32Value());

  NanReturnValue(NanUndefined());
}


NAN_METHOD(GetProgramParameter) {
  NanScope();

  int program = args[0]->Int32Value();
  int pname = args[1]->Int32Value();

  int value = 0;
  switch (pname) {
  case GL_DELETE_STATUS:
  case GL_LINK_STATUS:
  case GL_VALIDATE_STATUS:
    glGetProgramiv(program, pname, &value);
    NanReturnValue(JS_BOOL(static_cast<bool>(value!=0)));
  case GL_ATTACHED_SHADERS:
  case GL_ACTIVE_ATTRIBUTES:
  case GL_ACTIVE_UNIFORMS:
    glGetProgramiv(program, pname, &value);
    NanReturnValue(JS_FLOAT(static_cast<long>(value)));
  default:
    NanThrowTypeError("GetProgramParameter: Invalid Enum");
  }
  NanReturnUndefined();
}


NAN_METHOD(GetUniformLocation) {
  NanScope();

  int program = args[0]->Int32Value();
  NanAsciiString name(args[1]);

  NanReturnValue(JS_INT(glGetUniformLocation(program, *name)));
}


NAN_METHOD(ClearColor) {
  NanScope();

  float red = (float) args[0]->NumberValue();
  float green = (float) args[1]->NumberValue();
  float blue = (float) args[2]->NumberValue();
  float alpha = (float) args[3]->NumberValue();

  glClearColor(red, green, blue, alpha);

  NanReturnValue(NanUndefined());
}


NAN_METHOD(ClearDepth) {
  NanScope();

  float depth = (float) args[0]->NumberValue();

  glClearDepth(depth);

  NanReturnValue(NanUndefined());
}

NAN_METHOD(Disable) {
  NanScope();

  glDisable(args[0]->Int32Value());
  NanReturnValue(NanUndefined());
}

NAN_METHOD(Enable) {
  NanScope();

  glEnable(args[0]->Int32Value());
  NanReturnValue(NanUndefined());
}


NAN_METHOD(CreateTexture) {
  NanScope();

  GLuint texture;
  glGenTextures(1, &texture);
  #ifdef LOGGING
  cout<<"createTexture "<<texture<<endl;
  #endif
  registerGLObj(GLOBJECT_TYPE_TEXTURE, texture);
  NanReturnValue(NanNew<Number>(texture));
}


NAN_METHOD(BindTexture) {
  NanScope();

  int target = args[0]->Int32Value();
  int texture = args[1]->IsNull() ? 0 : args[1]->Int32Value();

  glBindTexture(target, texture);
  NanReturnValue(NanUndefined());
}


NAN_METHOD(TexImage2D) {
  NanScope();

  int target = args[0]->Int32Value();
  int level = args[1]->Int32Value();
  int internalformat = args[2]->Int32Value();
  int width = args[3]->Int32Value();
  int height = args[4]->Int32Value();
  int border = args[5]->Int32Value();
  int format = args[6]->Int32Value();
  int type = args[7]->Int32Value();
  void *pixels=getImageData(args[8]);

  glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);

  NanReturnValue(NanUndefined());
}


NAN_METHOD(TexParameteri) {
  NanScope();

  int target = args[0]->Int32Value();
  int pname = args[1]->Int32Value();
  int param = args[2]->Int32Value();

  glTexParameteri(target, pname, param);

  NanReturnValue(NanUndefined());
}

NAN_METHOD(TexParameterf) {
  NanScope();

  int target = args[0]->Int32Value();
  int pname = args[1]->Int32Value();
  float param = (float) args[2]->NumberValue();

  glTexParameterf(target, pname, param);

  NanReturnValue(NanUndefined());
}


NAN_METHOD(Clear) {
  NanScope();

  glClear(args[0]->Int32Value());

  NanReturnValue(NanUndefined());
}


NAN_METHOD(UseProgram) {
  NanScope();

  glUseProgram(args[0]->Int32Value());

  NanReturnValue(NanUndefined());
}

NAN_METHOD(CreateBuffer) {
  NanScope();

  GLuint buffer;
  glGenBuffers(1, &buffer);
  #ifdef LOGGING
  cout<<"createBuffer "<<buffer<<endl;
  #endif
  registerGLObj(GLOBJECT_TYPE_BUFFER, buffer);
  NanReturnValue(NanNew<Number>(buffer));
}

NAN_METHOD(BindBuffer) {
  NanScope();

  int target = args[0]->Int32Value();
  int buffer = args[1]->Uint32Value();
  glBindBuffer(target,buffer);

  NanReturnValue(NanUndefined());
}


NAN_METHOD(CreateFramebuffer) {
  NanScope();

  GLuint buffer;
  glGenFramebuffers(1, &buffer);
  #ifdef LOGGING
  cout<<"createFrameBuffer "<<buffer<<endl;
  #endif
  registerGLObj(GLOBJECT_TYPE_FRAMEBUFFER, buffer);
  NanReturnValue(NanNew<Number>(buffer));
}


NAN_METHOD(BindFramebuffer) {
  NanScope();

  int target = args[0]->Int32Value();
  int buffer = args[1]->IsNull() ? 0 : args[1]->Int32Value();

  glBindFramebuffer(target, buffer);

  NanReturnValue(NanUndefined());
}


NAN_METHOD(FramebufferTexture2D) {
  NanScope();

  int target = args[0]->Int32Value();
  int attachment = args[1]->Int32Value();
  int textarget = args[2]->Int32Value();
  int texture = args[3]->Int32Value();
  int level = args[4]->Int32Value();

  glFramebufferTexture2D(target, attachment, textarget, texture, level);

  NanReturnValue(NanUndefined());
}


NAN_METHOD(BufferData) {
  NanScope();

  int target = args[0]->Int32Value();
  if(args[1]->IsObject()) {
    Local<Object> obj = Local<Object>::Cast(args[1]);
    GLenum usage = args[2]->Int32Value();

    int element_size = SizeOfArrayElementForType(obj->GetIndexedPropertiesExternalArrayDataType());
    GLsizeiptr size = obj->GetIndexedPropertiesExternalArrayDataLength() * element_size;
    void* data = obj->GetIndexedPropertiesExternalArrayData();
    glBufferData(target, size, data, usage);
  }
  else if(args[1]->IsNumber()) {
    GLsizeiptr size = args[1]->Uint32Value();
    GLenum usage = args[2]->Int32Value();
    glBufferData(target, size, NULL, usage);
  }
  NanReturnValue(NanUndefined());
}


NAN_METHOD(BufferSubData) {
  NanScope();

  int target = args[0]->Int32Value();
  int offset = args[1]->Int32Value();
  Local<Object> obj = Local<Object>::Cast(args[2]);

  int element_size = SizeOfArrayElementForType( obj->GetIndexedPropertiesExternalArrayDataType());
  int size = obj->GetIndexedPropertiesExternalArrayDataLength() * element_size;
  void* data = obj->GetIndexedPropertiesExternalArrayData();

  glBufferSubData(target, offset, size, data);

  NanReturnValue(NanUndefined());
}


NAN_METHOD(BlendEquation) {
  NanScope();

  int mode=args[0]->Int32Value();;

  glBlendEquation(mode);

  NanReturnValue(NanUndefined());
}


NAN_METHOD(BlendFunc) {
  NanScope();

  int sfactor=args[0]->Int32Value();;
  int dfactor=args[1]->Int32Value();;

  glBlendFunc(sfactor,dfactor);

  NanReturnValue(NanUndefined());
}


NAN_METHOD(EnableVertexAttribArray) {
  NanScope();

  glEnableVertexAttribArray(args[0]->Int32Value());

  NanReturnValue(NanUndefined());
}


NAN_METHOD(VertexAttribPointer) {
  NanScope();

  int indx = args[0]->Int32Value();
  int size = args[1]->Int32Value();
  int type = args[2]->Int32Value();
  int normalized = args[3]->BooleanValue();
  int stride = args[4]->Int32Value();
  long offset = args[5]->Int32Value();

  //    printf("VertexAttribPointer %d %d %d %d %d %d\n", indx, size, type, normalized, stride, offset);
  glVertexAttribPointer(indx, size, type, normalized, stride, (const GLvoid *)offset);

  NanReturnValue(NanUndefined());
}


NAN_METHOD(ActiveTexture) {
  NanScope();

  glActiveTexture(args[0]->Int32Value());
  NanReturnValue(NanUndefined());
}


NAN_METHOD(DrawElements) {
  NanScope();

  int mode = args[0]->Int32Value();
  int count = args[1]->Int32Value();
  int type = args[2]->Int32Value();
  GLvoid *offset = reinterpret_cast<GLvoid*>(args[3]->Uint32Value());
  glDrawElements(mode, count, type, offset);
  NanReturnValue(NanUndefined());
}


NAN_METHOD(Flush) {
  NanScope();
  glFlush();
  NanReturnValue(NanUndefined());
}

NAN_METHOD(Finish) {
  NanScope();
  glFinish();
  NanReturnValue(NanUndefined());
}

NAN_METHOD(VertexAttrib1f) {
  NanScope();

  GLuint indx = args[0]->Int32Value();
  float x = (float) args[1]->NumberValue();

  glVertexAttrib1f(indx, x);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(VertexAttrib2f) {
  NanScope();

  GLuint indx = args[0]->Int32Value();
  float x = (float) args[1]->NumberValue();
  float y = (float) args[2]->NumberValue();

  glVertexAttrib2f(indx, x, y);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(VertexAttrib3f) {
  NanScope();

  GLuint indx = args[0]->Int32Value();
  float x = (float) args[1]->NumberValue();
  float y = (float) args[2]->NumberValue();
  float z = (float) args[3]->NumberValue();

  glVertexAttrib3f(indx, x, y, z);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(VertexAttrib4f) {
  NanScope();

  GLuint indx = args[0]->Int32Value();
  float x = (float) args[1]->NumberValue();
  float y = (float) args[2]->NumberValue();
  float z = (float) args[3]->NumberValue();
  float w = (float) args[4]->NumberValue();

  glVertexAttrib4f(indx, x, y, z, w);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(VertexAttrib1fv) {
  NanScope();

  int indx = args[0]->Int32Value();
  GLfloat *data = getArrayData<GLfloat>(args[1]);
  glVertexAttrib1fv(indx, data);

  NanReturnValue(NanUndefined());
}

NAN_METHOD(VertexAttrib2fv) {
  NanScope();

  int indx = args[0]->Int32Value();
  GLfloat *data = getArrayData<GLfloat>(args[1]);
  glVertexAttrib2fv(indx, data);

  NanReturnValue(NanUndefined());
}

NAN_METHOD(VertexAttrib3fv) {
  NanScope();

  int indx = args[0]->Int32Value();
  GLfloat *data = getArrayData<GLfloat>(args[1]);
  glVertexAttrib3fv(indx, data);

  NanReturnValue(NanUndefined());
}

NAN_METHOD(VertexAttrib4fv) {
  NanScope();

  int indx = args[0]->Int32Value();
  GLfloat *data = getArrayData<GLfloat>(args[1]);
  glVertexAttrib4fv(indx, data);

  NanReturnValue(NanUndefined());
}

NAN_METHOD(BlendColor) {
  NanScope();

  GLclampf r= (float) args[0]->NumberValue();
  GLclampf g= (float) args[1]->NumberValue();
  GLclampf b= (float) args[2]->NumberValue();
  GLclampf a= (float) args[3]->NumberValue();

  glBlendColor(r,g,b,a);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(BlendEquationSeparate) {
  NanScope();

  GLenum modeRGB= args[0]->Int32Value();
  GLenum modeAlpha= args[1]->Int32Value();

  glBlendEquationSeparate(modeRGB,modeAlpha);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(BlendFuncSeparate) {
  NanScope();

  GLenum srcRGB= args[0]->Int32Value();
  GLenum dstRGB= args[1]->Int32Value();
  GLenum srcAlpha= args[2]->Int32Value();
  GLenum dstAlpha= args[3]->Int32Value();

  glBlendFuncSeparate(srcRGB,dstRGB,srcAlpha,dstAlpha);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(ClearStencil) {
  NanScope();

  GLint s = args[0]->Int32Value();

  glClearStencil(s);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(ColorMask) {
  NanScope();

  GLboolean r = args[0]->BooleanValue();
  GLboolean g = args[1]->BooleanValue();
  GLboolean b = args[2]->BooleanValue();
  GLboolean a = args[3]->BooleanValue();

  glColorMask(r,g,b,a);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(CopyTexImage2D) {
  NanScope();

  GLenum target = args[0]->Int32Value();
  GLint level = args[1]->Int32Value();
  GLenum internalformat = args[2]->Int32Value();
  GLint x = args[3]->Int32Value();
  GLint y = args[4]->Int32Value();
  GLsizei width = args[5]->Int32Value();
  GLsizei height = args[6]->Int32Value();
  GLint border = args[7]->Int32Value();

  glCopyTexImage2D( target, level, internalformat, x, y, width, height, border);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(CopyTexSubImage2D) {
  NanScope();

  GLenum target = args[0]->Int32Value();
  GLint level = args[1]->Int32Value();
  GLint xoffset = args[2]->Int32Value();
  GLint yoffset = args[3]->Int32Value();
  GLint x = args[4]->Int32Value();
  GLint y = args[5]->Int32Value();
  GLsizei width = args[6]->Int32Value();
  GLsizei height = args[7]->Int32Value();

  glCopyTexSubImage2D( target, level, xoffset, yoffset, x, y, width, height);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(CullFace) {
  NanScope();

  GLenum mode = args[0]->Int32Value();

  glCullFace(mode);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(DepthMask) {
  NanScope();

  GLboolean flag = args[0]->BooleanValue();

  glDepthMask(flag);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(DepthRange) {
  NanScope();

  GLclampf zNear = (float) args[0]->NumberValue();
  GLclampf zFar = (float) args[1]->NumberValue();

  glDepthRangef(zNear, zFar);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(DisableVertexAttribArray) {
  NanScope();

  GLuint index = args[0]->Int32Value();

  glDisableVertexAttribArray(index);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(Hint) {
  NanScope();

  GLenum target = args[0]->Int32Value();
  GLenum mode = args[1]->Int32Value();

  glHint(target, mode);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(IsEnabled) {
  NanScope();

  GLenum cap = args[0]->Int32Value();

  bool ret=glIsEnabled(cap)!=0;
  NanReturnValue(NanNew<Boolean>(ret));
}

NAN_METHOD(LineWidth) {
  NanScope();

  GLfloat width = (float) args[0]->NumberValue();

  glLineWidth(width);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(PolygonOffset) {
  NanScope();

  GLfloat factor = (float) args[0]->NumberValue();
  GLfloat units = (float) args[1]->NumberValue();

  glPolygonOffset(factor, units);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(SampleCoverage) {
  NanScope();

  GLclampf value = (float) args[0]->NumberValue();
  GLboolean invert = args[1]->BooleanValue();

  glSampleCoverage(value, invert);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(Scissor) {
  NanScope();

  GLint x = args[0]->Int32Value();
  GLint y = args[1]->Int32Value();
  GLsizei width = args[2]->Int32Value();
  GLsizei height = args[3]->Int32Value();

  glScissor(x, y, width, height);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(StencilFunc) {
  NanScope();

  GLenum func = args[0]->Int32Value();
  GLint ref = args[1]->Int32Value();
  GLuint mask = args[2]->Int32Value();

  glStencilFunc(func, ref, mask);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(StencilFuncSeparate) {
  NanScope();

  GLenum face = args[0]->Int32Value();
  GLenum func = args[1]->Int32Value();
  GLint ref = args[2]->Int32Value();
  GLuint mask = args[3]->Int32Value();

  glStencilFuncSeparate(face, func, ref, mask);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(StencilMask) {
  NanScope();

  GLuint mask = args[0]->Uint32Value();

  glStencilMask(mask);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(StencilMaskSeparate) {
  NanScope();

  GLenum face = args[0]->Int32Value();
  GLuint mask = args[1]->Uint32Value();

  glStencilMaskSeparate(face, mask);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(StencilOp) {
  NanScope();

  GLenum fail = args[0]->Int32Value();
  GLenum zfail = args[1]->Int32Value();
  GLenum zpass = args[2]->Int32Value();

  glStencilOp(fail, zfail, zpass);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(StencilOpSeparate) {
  NanScope();

  GLenum face = args[0]->Int32Value();
  GLenum fail = args[1]->Int32Value();
  GLenum zfail = args[2]->Int32Value();
  GLenum zpass = args[3]->Int32Value();

  glStencilOpSeparate(face, fail, zfail, zpass);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(BindRenderbuffer) {
  NanScope();

  GLenum target = args[0]->Int32Value();
  GLuint buffer = args[1]->IsNull() ? 0 : args[1]->Int32Value();

  glBindRenderbuffer(target, buffer);

  NanReturnValue(NanUndefined());
}

NAN_METHOD(CreateRenderbuffer) {
  NanScope();

  GLuint renderbuffers;
  glGenRenderbuffers(1,&renderbuffers);
  #ifdef LOGGING
  cout<<"createRenderBuffer "<<renderbuffers<<endl;
  #endif
  registerGLObj(GLOBJECT_TYPE_RENDERBUFFER, renderbuffers);
  NanReturnValue(NanNew<Number>(renderbuffers));
}

NAN_METHOD(DeleteBuffer) {
  NanScope();

  GLuint buffer = args[0]->Uint32Value();

  glDeleteBuffers(1,&buffer);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(DeleteFramebuffer) {
  NanScope();

  GLuint buffer = args[0]->Uint32Value();

  glDeleteFramebuffers(1,&buffer);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(DeleteProgram) {
  NanScope();

  GLuint program = args[0]->Uint32Value();

  glDeleteProgram(program);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(DeleteRenderbuffer) {
  NanScope();

  GLuint renderbuffer = args[0]->Uint32Value();

  glDeleteRenderbuffers(1, &renderbuffer);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(DeleteShader) {
  NanScope();

  GLuint shader = args[0]->Uint32Value();

  glDeleteShader(shader);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(DeleteTexture) {
  NanScope();

  GLuint texture = args[0]->Uint32Value();

  glDeleteTextures(1,&texture);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(DetachShader) {
  NanScope();

  GLuint program = args[0]->Uint32Value();
  GLuint shader = args[1]->Uint32Value();

  glDetachShader(program, shader);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(FramebufferRenderbuffer) {
  NanScope();

  GLenum target = args[0]->Int32Value();
  GLenum attachment = args[1]->Int32Value();
  GLenum renderbuffertarget = args[2]->Int32Value();
  GLuint renderbuffer = args[3]->Uint32Value();

  glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(GetVertexAttribOffset) {
  NanScope();

  GLuint index = args[0]->Uint32Value();
  GLenum pname = args[1]->Int32Value();
  void *ret=NULL;

  glGetVertexAttribPointerv(index, pname, &ret);
  NanReturnValue(JS_INT(ToGLuint(ret)));
}

NAN_METHOD(IsBuffer) {
  NanScope();

  NanReturnValue(NanNew<Boolean>(glIsBuffer(args[0]->Uint32Value())!=0));
}

NAN_METHOD(IsFramebuffer) {
  NanScope();

  NanReturnValue(JS_BOOL(glIsFramebuffer(args[0]->Uint32Value())!=0));
}

NAN_METHOD(IsProgram) {
  NanScope();

  NanReturnValue(JS_BOOL(glIsProgram(args[0]->Uint32Value())!=0));
}

NAN_METHOD(IsRenderbuffer) {
  NanScope();

  NanReturnValue(JS_BOOL(glIsRenderbuffer( args[0]->Uint32Value())!=0));
}

NAN_METHOD(IsShader) {
  NanScope();

  NanReturnValue(JS_BOOL(glIsShader(args[0]->Uint32Value())!=0));
}

NAN_METHOD(IsTexture) {
  NanScope();

  NanReturnValue(JS_BOOL(glIsTexture(args[0]->Uint32Value())!=0));
}

NAN_METHOD(RenderbufferStorage) {
  NanScope();

  GLenum target = args[0]->Int32Value();
  GLenum internalformat = args[1]->Int32Value();
  GLsizei width = args[2]->Uint32Value();
  GLsizei height = args[3]->Uint32Value();

  glRenderbufferStorage(target, internalformat, width, height);
  NanReturnValue(NanUndefined());
}

NAN_METHOD(GetShaderSource) {
  NanScope();

  int shader = args[0]->Int32Value();

  GLint len;
  glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &len);
  GLchar *source=new GLchar[len];
  glGetShaderSource(shader, len, NULL, source);

  Local<String> str = NanNew<String>(source);
  delete source;

  NanReturnValue(str);
}

NAN_METHOD(ValidateProgram) {
  NanScope();

  glValidateProgram(args[0]->Int32Value());

  NanReturnValue(NanUndefined());
}

NAN_METHOD(TexSubImage2D) {
  NanScope();

  GLenum target = args[0]->Int32Value();
  GLint level = args[1]->Int32Value();
  GLint xoffset = args[2]->Int32Value();
  GLint yoffset = args[3]->Int32Value();
  GLsizei width = args[4]->Int32Value();
  GLsizei height = args[5]->Int32Value();
  GLenum format = args[6]->Int32Value();
  GLenum type = args[7]->Int32Value();
  void *pixels=getImageData(args[8]);

  glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);

  NanReturnValue(NanUndefined());
}

NAN_METHOD(ReadPixels) {
  NanScope();

  GLint x = args[0]->Int32Value();
  GLint y = args[1]->Int32Value();
  GLsizei width = args[2]->Int32Value();
  GLsizei height = args[3]->Int32Value();
  GLenum format = args[4]->Int32Value();
  GLenum type = args[5]->Int32Value();
  void *pixels=getImageData(args[6]);

  glReadPixels(x, y, width, height, format, type, pixels);

  NanReturnValue(NanUndefined());
}

NAN_METHOD(GetTexParameter) {
  NanScope();

  GLenum target = args[0]->Int32Value();
  GLenum pname = args[1]->Int32Value();

  GLint param_value=0;
  glGetTexParameteriv(target, pname, &param_value);

  NanReturnValue(NanNew<Number>(param_value));
}

NAN_METHOD(GetActiveAttrib) {
  NanScope();

  GLuint program = args[0]->Int32Value();
  GLuint index = args[1]->Int32Value();

  char name[1024];
  GLsizei length=0;
  GLenum type;
  GLsizei size;
  glGetActiveAttrib(program, index, 1024, &length, &size, &type, name);

  Local<Array> activeInfo = NanNew<Array>(3);
  activeInfo->Set(JS_STR("size"), JS_INT(size));
  activeInfo->Set(JS_STR("type"), JS_INT((int)type));
  activeInfo->Set(JS_STR("name"), JS_STR(name));

  NanReturnValue(activeInfo);
}

NAN_METHOD(GetActiveUniform) {
  NanScope();

  GLuint program = args[0]->Int32Value();
  GLuint index = args[1]->Int32Value();

  char name[1024];
  GLsizei length=0;
  GLenum type;
  GLsizei size;
  glGetActiveUniform(program, index, 1024, &length, &size, &type, name);

  Local<Array> activeInfo = NanNew<Array>(3);
  activeInfo->Set(JS_STR("size"), JS_INT(size));
  activeInfo->Set(JS_STR("type"), JS_INT((int)type));
  activeInfo->Set(JS_STR("name"), JS_STR(name));

  NanReturnValue(activeInfo);
}

NAN_METHOD(GetAttachedShaders) {
  NanScope();

  GLuint program = args[0]->Int32Value();

  GLuint shaders[1024];
  GLsizei count;
  glGetAttachedShaders(program, 1024, &count, shaders);

  Local<Array> shadersArr = NanNew<Array>(count);
  for(int i=0;i<count;i++)
    shadersArr->Set(i, JS_INT((int)shaders[i]));

  NanReturnValue(shadersArr);
}

NAN_METHOD(GetParameter) {
  NanScope();

  GLenum name = args[0]->Int32Value();

  switch(name) {
  case GL_BLEND:
  case GL_CULL_FACE:
  case GL_DEPTH_TEST:
  case GL_DEPTH_WRITEMASK:
  case GL_DITHER:
  case GL_POLYGON_OFFSET_FILL:
  case GL_SAMPLE_COVERAGE_INVERT:
  case GL_SCISSOR_TEST:
  case GL_STENCIL_TEST:
  case 0x9240 /* UNPACK_FLIP_Y_WEBGL */:
  case 0x9241 /* UNPACK_PREMULTIPLY_ALPHA_WEBGL*/:
  {
    // return a boolean
    GLboolean params;
    ::glGetBooleanv(name, &params);
    NanReturnValue(JS_BOOL(params!=0));
  }
  case GL_DEPTH_CLEAR_VALUE:
  case GL_LINE_WIDTH:
  case GL_POLYGON_OFFSET_FACTOR:
  case GL_POLYGON_OFFSET_UNITS:
  case GL_SAMPLE_COVERAGE_VALUE:
  {
    // return a float
    GLfloat params;
    ::glGetFloatv(name, &params);
    NanReturnValue(JS_FLOAT(params));
  }
  case GL_RENDERER:
  case GL_SHADING_LANGUAGE_VERSION:
  case GL_VENDOR:
  case GL_VERSION:
  case GL_EXTENSIONS:
  {
    // return a string
    char *params=(char*) ::glGetString(name);
    if(params)
      NanReturnValue(JS_STR(params));
    NanReturnUndefined();
  }
  case GL_MAX_VIEWPORT_DIMS:
  {
    // return a int32[2]
    GLint params[2];
    ::glGetIntegerv(name, params);

    Local<Array> arr=NanNew<Array>(2);
    arr->Set(0,JS_INT(params[0]));
    arr->Set(1,JS_INT(params[1]));
    NanReturnValue(arr);
  }
  case GL_SCISSOR_BOX:
  case GL_VIEWPORT:
  {
    // return a int32[4]
    GLint params[4];
    ::glGetIntegerv(name, params);

    Local<Array> arr=NanNew<Array>(4);
    arr->Set(0,JS_INT(params[0]));
    arr->Set(1,JS_INT(params[1]));
    arr->Set(2,JS_INT(params[2]));
    arr->Set(3,JS_INT(params[3]));
    NanReturnValue(arr);
  }
  case GL_ALIASED_LINE_WIDTH_RANGE:
  case GL_ALIASED_POINT_SIZE_RANGE:
  case GL_DEPTH_RANGE:
  {
    // return a float[2]
    GLfloat params[2];
    ::glGetFloatv(name, params);
    Local<Array> arr=NanNew<Array>(2);
    arr->Set(0,JS_FLOAT(params[0]));
    arr->Set(1,JS_FLOAT(params[1]));
    NanReturnValue(arr);
  }
  case GL_BLEND_COLOR:
  case GL_COLOR_CLEAR_VALUE:
  {
    // return a float[4]
    GLfloat params[4];
    ::glGetFloatv(name, params);
    Local<Array> arr=NanNew<Array>(4);
    arr->Set(0,JS_FLOAT(params[0]));
    arr->Set(1,JS_FLOAT(params[1]));
    arr->Set(2,JS_FLOAT(params[2]));
    arr->Set(3,JS_FLOAT(params[3]));
    NanReturnValue(arr);
  }
  case GL_COLOR_WRITEMASK:
  {
    // return a boolean[4]
    GLboolean params[4];
    ::glGetBooleanv(name, params);
    Local<Array> arr=NanNew<Array>(4);
    arr->Set(0,JS_BOOL(params[0]==1));
    arr->Set(1,JS_BOOL(params[1]==1));
    arr->Set(2,JS_BOOL(params[2]==1));
    arr->Set(3,JS_BOOL(params[3]==1));
    NanReturnValue(arr);
  }
  case GL_ARRAY_BUFFER_BINDING:
  case GL_CURRENT_PROGRAM:
  case GL_ELEMENT_ARRAY_BUFFER_BINDING:
  case GL_FRAMEBUFFER_BINDING:
  case GL_RENDERBUFFER_BINDING:
  case GL_TEXTURE_BINDING_2D:
  case GL_TEXTURE_BINDING_CUBE_MAP:
  {
    GLint params;
    ::glGetIntegerv(name, &params);
    NanReturnValue(JS_INT(params));
  }
  default: {
    // return a long
    GLint params;
    ::glGetIntegerv(name, &params);
    NanReturnValue(JS_INT(params));
  }
  }

  NanReturnValue(NanUndefined());
}

NAN_METHOD(GetBufferParameter) {
  NanScope();

  GLenum target = args[0]->Int32Value();
  GLenum pname = args[1]->Int32Value();

  GLint params;
  glGetBufferParameteriv(target,pname,&params);
  NanReturnValue(JS_INT(params));
}

NAN_METHOD(GetFramebufferAttachmentParameter) {
  NanScope();

  GLenum target = args[0]->Int32Value();
  GLenum attachment = args[1]->Int32Value();
  GLenum pname = args[2]->Int32Value();

  GLint params;
  glGetFramebufferAttachmentParameteriv(target,attachment, pname,&params);
  NanReturnValue(JS_INT(params));
}

NAN_METHOD(GetProgramInfoLog) {
  NanScope();

  GLuint program = args[0]->Int32Value();
  int Len = 1024;
  char Error[1024];
  glGetProgramInfoLog(program, 1024, &Len, Error);

  NanReturnValue(NanNew<String>(Error));
}

NAN_METHOD(GetRenderbufferParameter) {
  NanScope();

  int target = args[0]->Int32Value();
  int pname = args[1]->Int32Value();
  int value = 0;
  glGetRenderbufferParameteriv(target,pname,&value);

  NanReturnValue(JS_INT(value));
}

NAN_METHOD(GetUniform) {
  NanScope();

  GLuint program = args[0]->Int32Value();
  GLint location = args[1]->Int32Value();
  if(location < 0 ) NanReturnValue(NanUndefined());

  float data[16]; // worst case scenario is 16 floats

  glGetUniformfv(program, location, data);

  Local<Array> arr=NanNew<Array>(16);
  for(int i=0;i<16;i++)
    arr->Set(i,JS_FLOAT(data[i]));

  NanReturnValue(arr);
}

NAN_METHOD(GetVertexAttrib) {
  NanScope();

  GLuint index = args[0]->Int32Value();
  GLuint pname = args[1]->Int32Value();

  GLint value=0;

  switch (pname) {
  case GL_VERTEX_ATTRIB_ARRAY_ENABLED:
  case GL_VERTEX_ATTRIB_ARRAY_NORMALIZED:
    glGetVertexAttribiv(index,pname,&value);
    NanReturnValue(JS_BOOL(value!=0));
  case GL_VERTEX_ATTRIB_ARRAY_SIZE:
  case GL_VERTEX_ATTRIB_ARRAY_STRIDE:
  case GL_VERTEX_ATTRIB_ARRAY_TYPE:
    glGetVertexAttribiv(index,pname,&value);
    NanReturnValue(JS_INT(value));
  case GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING:
    glGetVertexAttribiv(index,pname,&value);
    NanReturnValue(JS_INT(value));
  case GL_CURRENT_VERTEX_ATTRIB: {
    float vextex_attribs[4];
    glGetVertexAttribfv(index,pname,vextex_attribs);
    Local<Array> arr=NanNew<Array>(4);
    arr->Set(0,JS_FLOAT(vextex_attribs[0]));
    arr->Set(1,JS_FLOAT(vextex_attribs[1]));
    arr->Set(2,JS_FLOAT(vextex_attribs[2]));
    arr->Set(3,JS_FLOAT(vextex_attribs[3]));
    NanReturnValue(arr);
  }
  default:
    NanThrowError("GetVertexAttrib: Invalid Enum");
  }

  NanReturnValue(NanUndefined());
}

NAN_METHOD(GetSupportedExtensions) {
  NanScope();

  char *extensions=(char*) glGetString(GL_EXTENSIONS);

  NanReturnValue(JS_STR(extensions));
}

// TODO GetExtension(name) return the extension name if found, should be an object...
NAN_METHOD(GetExtension) {
  NanScope();

  NanAsciiString name(args[0]);
  char *sname=*name;
  char *extensions=(char*) glGetString(GL_EXTENSIONS);

  char *ext=strcasestr(extensions, sname);

  if(!ext) NanReturnValue(NanUndefined());
  NanReturnValue(JS_STR(ext, (int)::strlen(sname)));
}

NAN_METHOD(CheckFramebufferStatus) {
  NanScope();

  GLenum target=args[0]->Int32Value();

  NanReturnValue(JS_INT((int)glCheckFramebufferStatus(target)));
}

struct GLObj {
  GLObjectType type;
  GLuint obj;
  GLObj(GLObjectType type, GLuint obj) {
    this->type=type;
    this->obj=obj;
  }
};

vector<GLObj*> globjs;
static bool atExit=false;

void registerGLObj(GLObjectType type, GLuint obj) {
  globjs.push_back(new GLObj(type,obj));
}


void unregisterGLObj(GLuint obj) {
  if(atExit) return;

  vector<GLObj*>::iterator it = globjs.begin();
  while(globjs.size() && it != globjs.end()) {
    GLObj *globj=*it;
    if(globj->obj==obj) {
      delete globj;
      globjs.erase(it);
      break;
    }
    ++it;
  }
}

void AtExit() {
  atExit=true;
  //glFinish();

  vector<GLObj*>::iterator it;

  #ifdef LOGGING
  cout<<"WebGL AtExit() called"<<endl;
  cout<<"  # objects allocated: "<<globjs.size()<<endl;
  it = globjs.begin();
  while(globjs.size() && it != globjs.end()) {
    GLObj *obj=*it;
    cout<<"[";
    switch(obj->type) {
    case GLOBJECT_TYPE_BUFFER: cout<<"buffer"; break;
    case GLOBJECT_TYPE_FRAMEBUFFER: cout<<"framebuffer"; break;
    case GLOBJECT_TYPE_PROGRAM: cout<<"program"; break;
    case GLOBJECT_TYPE_RENDERBUFFER: cout<<"renderbuffer"; break;
    case GLOBJECT_TYPE_SHADER: cout<<"shader"; break;
    case GLOBJECT_TYPE_TEXTURE: cout<<"texture"; break;
    };
    cout<<": "<<obj->obj<<"] ";
    ++it;
  }
  cout<<endl;
  #endif

  it = globjs.begin();
  while(globjs.size() && it != globjs.end()) {
    GLObj *globj=*it;
    GLuint obj=globj->obj;

    switch(globj->type) {
    case GLOBJECT_TYPE_PROGRAM:
      #ifdef LOGGING
      cout<<"  Destroying GL program "<<obj<<endl;
      #endif
      glDeleteProgram(obj);
      break;
    case GLOBJECT_TYPE_BUFFER:
      #ifdef LOGGING
      cout<<"  Destroying GL buffer "<<obj<<endl;
      #endif
      glDeleteBuffers(1,&obj);
      break;
    case GLOBJECT_TYPE_FRAMEBUFFER:
      #ifdef LOGGING
      cout<<"  Destroying GL frame buffer "<<obj<<endl;
      #endif
      glDeleteFramebuffers(1,&obj);
      break;
    case GLOBJECT_TYPE_RENDERBUFFER:
      #ifdef LOGGING
      cout<<"  Destroying GL render buffer "<<obj<<endl;
      #endif
      glDeleteRenderbuffers(1,&obj);
      break;
    case GLOBJECT_TYPE_SHADER:
      #ifdef LOGGING
      cout<<"  Destroying GL shader "<<obj<<endl;
      #endif
      glDeleteShader(obj);
      break;
    case GLOBJECT_TYPE_TEXTURE:
      #ifdef LOGGING
      cout<<"  Destroying GL texture "<<obj<<endl;
      #endif
      glDeleteTextures(1,&obj);
      break;
    default:
      #ifdef LOGGING
      cout<<"  Unknown object "<<obj<<endl;
      #endif
      break;
    }
    delete globj;
    ++it;
  }

  globjs.clear();
}

} // end namespace webgl
