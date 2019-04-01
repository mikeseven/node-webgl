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

#define CHECK_ARRAY_BUFFER(val) if(!val->IsArrayBufferView()) \
        {Nan::ThrowTypeError("Only support array buffer"); return;}

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

// A 32-bit and 64-bit compatible way of converting a pointer to a GLuint.
static GLuint ToGLuint(const void* ptr) {
  return static_cast<GLuint>(reinterpret_cast<size_t>(ptr));
}

template<typename Type>
inline Type* getArrayData(Local<Value> arg, int* num = NULL) {
  Type *data=NULL;
  if(num) *num=0;

  if(!arg->IsNull()) {
    if(arg->IsArray()) {
      Nan::ThrowError("Not support array type");
      /*
      Local<Array> arr = Local<Array>::Cast(arg);
      if(num) *num=arr->Length();
      data = reinterpret_cast<Type*>(arr->GetIndexedPropertiesExternalArrayData());*/
    }
    else if(arg->IsObject()) {
      Local<ArrayBufferView> arr = Local<ArrayBufferView>::Cast(arg);
      if(num) *num=arr->ByteLength()/sizeof(Type);
      data = reinterpret_cast<Type*>((uint8_t*)arr->Buffer()->GetContents().Data() + arr->ByteOffset());
    }
    else
      Nan::ThrowError("Bad array argument");
  }

  return data;
}

inline void *getImageData(Local<Value> arg) {
  void *pixels = NULL;
  if (!arg->IsNull()) {
    Local<Object> obj = Local<Object>::Cast(arg);
    if (!obj->IsObject()){
      Nan::ThrowError("Bad texture argument");
    }else if(obj->IsArrayBufferView()){
        int num;

        pixels = getArrayData<BYTE>(obj, &num);
    }else{
        pixels = node::Buffer::Data(Nan::Get(obj, JS_STR("data")).ToLocalChecked());
    }
  }
  return pixels;
}

NAN_METHOD(Init) {
  Nan::HandleScope scope;
  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
    /* Problem: glewInit failed, something is seriously wrong. */
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    info.GetReturnValue().Set(JS_INT(-1));
  }else{
    //fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    info.GetReturnValue().Set(JS_INT(0));
  }

}

NAN_METHOD(Uniform1f) {
  Nan::HandleScope scope;

  int location = info[0]->Int32Value();
  float x = (float) info[1]->NumberValue();

  glUniform1f(location, x);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(Uniform2f) {
  Nan::HandleScope scope;

  int location = info[0]->Int32Value();
  float x = (float) info[1]->NumberValue();
  float y = (float) info[2]->NumberValue();

  glUniform2f(location, x, y);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(Uniform3f) {
  Nan::HandleScope scope;

  int location = info[0]->Int32Value();
  float x = (float) info[1]->NumberValue();
  float y = (float) info[2]->NumberValue();
  float z = (float) info[3]->NumberValue();

  glUniform3f(location, x, y, z);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(Uniform4f) {
  Nan::HandleScope scope;

  int location = info[0]->Int32Value();
  float x = (float) info[1]->NumberValue();
  float y = (float) info[2]->NumberValue();
  float z = (float) info[3]->NumberValue();
  float w = (float) info[4]->NumberValue();

  glUniform4f(location, x, y, z, w);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(Uniform1i) {
  Nan::HandleScope scope;

  int location = info[0]->Int32Value();
  int x = info[1]->Int32Value();

  glUniform1i(location, x);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(Uniform2i) {
  Nan::HandleScope scope;

  int location = info[0]->Int32Value();
  int x = info[1]->Int32Value();
  int y = info[2]->Int32Value();

  glUniform2i(location, x, y);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(Uniform3i) {
  Nan::HandleScope scope;

  int location = info[0]->Int32Value();
  int x = info[1]->Int32Value();
  int y = info[2]->Int32Value();
  int z = info[3]->Int32Value();

  glUniform3i(location, x, y, z);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(Uniform4i) {
  Nan::HandleScope scope;

  int location = info[0]->Int32Value();
  int x = info[1]->Int32Value();
  int y = info[2]->Int32Value();
  int z = info[3]->Int32Value();
  int w = info[4]->Int32Value();

  glUniform4i(location, x, y, z, w);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(Uniform1fv) {
  Nan::HandleScope scope;

  int location = info[0]->Int32Value();
  int num=0;
  GLfloat *ptr=getArrayData<GLfloat>(info[1],&num);
  glUniform1fv(location, num, ptr);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(Uniform2fv) {
  Nan::HandleScope scope;

  int location = info[0]->Int32Value();
  int num=0;
  GLfloat *ptr=getArrayData<GLfloat>(info[1],&num);
  num /= 2;

  glUniform2fv(location, num, ptr);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(Uniform3fv) {
  Nan::HandleScope scope;

  int location = info[0]->Int32Value();
  int num=0;
  GLfloat *ptr=getArrayData<GLfloat>(info[1],&num);
  num /= 3;

  glUniform3fv(location, num, ptr);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(Uniform4fv) {
  Nan::HandleScope scope;

  int location = info[0]->Int32Value();
  int num=0;
  GLfloat *ptr=getArrayData<GLfloat>(info[1],&num);
  num /= 4;

  glUniform4fv(location, num, ptr);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(Uniform1iv) {
  Nan::HandleScope scope;

  int location = info[0]->Int32Value();
  int num=0;
  GLint *ptr=getArrayData<GLint>(info[1],&num);

  glUniform1iv(location, num, ptr);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(Uniform2iv) {
  Nan::HandleScope scope;

  int location = info[0]->Int32Value();
  int num=0;
  GLint *ptr=getArrayData<GLint>(info[1],&num);
  num /= 2;

  glUniform2iv(location, num, ptr);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(Uniform3iv) {
  Nan::HandleScope scope;

  int location = info[0]->Int32Value();
  int num=0;
  GLint *ptr=getArrayData<GLint>(info[1],&num);
  num /= 3;
  glUniform3iv(location, num, ptr);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(Uniform4iv) {
  Nan::HandleScope scope;

  int location = info[0]->Int32Value();
  int num=0;
  GLint *ptr=getArrayData<GLint>(info[1],&num);
  num /= 4;
  glUniform4iv(location, num, ptr);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(PixelStorei) {
  Nan::HandleScope scope;

  int pname = info[0]->Int32Value();
  int param = info[1]->Int32Value();

  glPixelStorei(pname,param);

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(BindAttribLocation) {
  Nan::HandleScope scope;

  int program = info[0]->Int32Value();
  int index = info[1]->Int32Value();
  String::Utf8Value name(info[2]);

  glBindAttribLocation(program, index, *name);

  info.GetReturnValue().Set(Nan::Undefined());
}


NAN_METHOD(GetError) {
  Nan::HandleScope scope;

  info.GetReturnValue().Set(Nan::New<Integer>(glGetError()));
}


NAN_METHOD(DrawArrays) {
  Nan::HandleScope scope;

  int mode = info[0]->Int32Value();
  int first = info[1]->Int32Value();
  int count = info[2]->Int32Value();

  glDrawArrays(mode, first, count);

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(UniformMatrix2fv) {
  Nan::HandleScope scope;

  GLint location = info[0]->Int32Value();
  GLboolean transpose = info[1]->BooleanValue();

  GLsizei count=0;
  GLfloat* data=getArrayData<GLfloat>(info[2],&count);

  if (count < 4) {
    Nan::ThrowError("Not enough data for UniformMatrix2fv");
  }else{
    glUniformMatrix2fv(location, count / 4, transpose, data);

    info.GetReturnValue().Set(Nan::Undefined());
  }
}

NAN_METHOD(UniformMatrix3fv) {
  Nan::HandleScope scope;

  GLint location = info[0]->Int32Value();
  GLboolean transpose = info[1]->BooleanValue();
  GLsizei count=0;
  GLfloat* data=getArrayData<GLfloat>(info[2],&count);

  if (count < 9) {
    Nan::ThrowError("Not enough data for UniformMatrix3fv");
  }else{
    glUniformMatrix3fv(location, count / 9, transpose, data);
    info.GetReturnValue().Set(Nan::Undefined());
  }
}

NAN_METHOD(UniformMatrix4fv) {
  Nan::HandleScope scope;

  GLint location = info[0]->Int32Value();
  GLboolean transpose = info[1]->BooleanValue();
  GLsizei count=0;
  GLfloat* data=getArrayData<GLfloat>(info[2],&count);

  if (count < 16) {
    Nan::ThrowError("Not enough data for UniformMatrix4fv");
  }else{
    glUniformMatrix4fv(location, count / 16, transpose, data);
    info.GetReturnValue().Set(Nan::Undefined());
  }
}

NAN_METHOD(GenerateMipmap) {
  Nan::HandleScope scope;

  GLint target = info[0]->Int32Value();
  glGenerateMipmap(target);

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(GetAttribLocation) {
  Nan::HandleScope scope;

  int program = info[0]->Int32Value();
  String::Utf8Value name(info[1]);

  info.GetReturnValue().Set(Nan::New<Number>(glGetAttribLocation(program, *name)));
}


NAN_METHOD(DepthFunc) {
  Nan::HandleScope scope;

  glDepthFunc(info[0]->Int32Value());

  info.GetReturnValue().Set(Nan::Undefined());
}


NAN_METHOD(Viewport) {
  Nan::HandleScope scope;

  int x = info[0]->Int32Value();
  int y = info[1]->Int32Value();
  int width = info[2]->Int32Value();
  int height = info[3]->Int32Value();

  glViewport(x, y, width, height);

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(CreateShader) {
  Nan::HandleScope scope;

  GLuint shader=glCreateShader(info[0]->Int32Value());
  #ifdef LOGGING
  cout<<"createShader "<<shader<<endl;
  #endif
  registerGLObj(GLOBJECT_TYPE_SHADER, shader);
  info.GetReturnValue().Set(Nan::New<Number>(shader));
}


NAN_METHOD(ShaderSource) {
  Nan::HandleScope scope;

  int id = info[0]->Int32Value();
  String::Utf8Value code(info[1]);

  const char* codes[1];
  codes[0] = *code;
  GLint length=code.length();

  glShaderSource  (id, 1, codes, &length);

  info.GetReturnValue().Set(Nan::Undefined());
}


NAN_METHOD(CompileShader) {
  Nan::HandleScope scope;

  glCompileShader(info[0]->Int32Value());

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(FrontFace) {
  Nan::HandleScope scope;

  glFrontFace(info[0]->Int32Value());

  info.GetReturnValue().Set(Nan::Undefined());
}


NAN_METHOD(GetShaderParameter) {
  Nan::HandleScope scope;

  int shader = info[0]->Int32Value();
  int pname = info[1]->Int32Value();
  int value = 0;
  switch (pname) {
  case GL_DELETE_STATUS:
  case GL_COMPILE_STATUS:
    glGetShaderiv(shader, pname, &value);
    info.GetReturnValue().Set(JS_BOOL(static_cast<bool>(value!=0)));
    break;
  case GL_SHADER_TYPE:
    glGetShaderiv(shader, pname, &value);
    info.GetReturnValue().Set(JS_FLOAT(static_cast<unsigned long>(value)));
    break;
  case GL_INFO_LOG_LENGTH:
  case GL_SHADER_SOURCE_LENGTH:
    glGetShaderiv(shader, pname, &value);
    info.GetReturnValue().Set(JS_FLOAT(static_cast<long>(value)));
    break;
  default:
    Nan::ThrowTypeError("GetShaderParameter: Invalid Enum");
  }
  //info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(GetShaderInfoLog) {
  Nan::HandleScope scope;

  int id = info[0]->Int32Value();
  int Len = 1024;
  char Error[1024];
  glGetShaderInfoLog(id, 1024, &Len, Error);

  info.GetReturnValue().Set(JS_STR(Error));
}


NAN_METHOD(CreateProgram) {
  Nan::HandleScope scope;

  GLuint program=glCreateProgram();
  #ifdef LOGGING
  cout<<"createProgram "<<program<<endl;
  #endif
  registerGLObj(GLOBJECT_TYPE_PROGRAM, program);
  info.GetReturnValue().Set(Nan::New<Number>(program));
}


NAN_METHOD(AttachShader) {
  Nan::HandleScope scope;

  int program = info[0]->Int32Value();
  int shader = info[1]->Int32Value();

  glAttachShader(program, shader);

  info.GetReturnValue().Set(Nan::Undefined());
}


NAN_METHOD(LinkProgram) {
  Nan::HandleScope scope;

  glLinkProgram(info[0]->Int32Value());

  info.GetReturnValue().Set(Nan::Undefined());
}


NAN_METHOD(GetProgramParameter) {
  Nan::HandleScope scope;

  int program = info[0]->Int32Value();
  int pname = info[1]->Int32Value();

  int value = 0;
  switch (pname) {
  case GL_DELETE_STATUS:
  case GL_LINK_STATUS:
  case GL_VALIDATE_STATUS:
    glGetProgramiv(program, pname, &value);
    info.GetReturnValue().Set(JS_BOOL(static_cast<bool>(value!=0)));
    break;
  case GL_ATTACHED_SHADERS:
  case GL_ACTIVE_ATTRIBUTES:
  case GL_ACTIVE_UNIFORMS:
    glGetProgramiv(program, pname, &value);
    info.GetReturnValue().Set(JS_FLOAT(static_cast<long>(value)));
    break;
  default:
    Nan::ThrowTypeError("GetProgramParameter: Invalid Enum");
  }
  //info.GetReturnValue().Set(Nan::Undefined());
}


NAN_METHOD(GetUniformLocation) {
  Nan::HandleScope scope;

  int program = info[0]->Int32Value();
  v8::String::Utf8Value name(info[1]);

  info.GetReturnValue().Set(JS_INT(glGetUniformLocation(program, *name)));
}


NAN_METHOD(ClearColor) {
  Nan::HandleScope scope;

  float red = (float) info[0]->NumberValue();
  float green = (float) info[1]->NumberValue();
  float blue = (float) info[2]->NumberValue();
  float alpha = (float) info[3]->NumberValue();

  glClearColor(red, green, blue, alpha);

  info.GetReturnValue().Set(Nan::Undefined());
}


NAN_METHOD(ClearDepth) {
  Nan::HandleScope scope;

  float depth = (float) info[0]->NumberValue();

  glClearDepth(depth);

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(Disable) {
  Nan::HandleScope scope;

  glDisable(info[0]->Int32Value());
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(Enable) {
  Nan::HandleScope scope;

  glEnable(info[0]->Int32Value());
  info.GetReturnValue().Set(Nan::Undefined());
}


NAN_METHOD(CreateTexture) {
  Nan::HandleScope scope;

  GLuint texture;
  glGenTextures(1, &texture);
  #ifdef LOGGING
  cout<<"createTexture "<<texture<<endl;
  #endif
  registerGLObj(GLOBJECT_TYPE_TEXTURE, texture);
  info.GetReturnValue().Set(Nan::New<Number>(texture));
}


NAN_METHOD(BindTexture) {
  Nan::HandleScope scope;

  int target = info[0]->Int32Value();
  int texture = info[1]->IsNull() ? 0 : info[1]->Int32Value();

  glBindTexture(target, texture);
  info.GetReturnValue().Set(Nan::Undefined());
}


NAN_METHOD(TexImage2D) {
  Nan::HandleScope scope;

  int target = info[0]->Int32Value();
  int level = info[1]->Int32Value();
  int internalformat = info[2]->Int32Value();
  int width = info[3]->Int32Value();
  int height = info[4]->Int32Value();
  int border = info[5]->Int32Value();
  int format = info[6]->Int32Value();
  int type = info[7]->Int32Value();
  void *pixels=getImageData(info[8]);

  glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);

  info.GetReturnValue().Set(Nan::Undefined());
}


NAN_METHOD(TexParameteri) {
  Nan::HandleScope scope;

  int target = info[0]->Int32Value();
  int pname = info[1]->Int32Value();
  int param = info[2]->Int32Value();

  glTexParameteri(target, pname, param);

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(TexParameterf) {
  Nan::HandleScope scope;

  int target = info[0]->Int32Value();
  int pname = info[1]->Int32Value();
  float param = (float) info[2]->NumberValue();

  glTexParameterf(target, pname, param);

  info.GetReturnValue().Set(Nan::Undefined());
}


NAN_METHOD(Clear) {
  Nan::HandleScope scope;

  glClear(info[0]->Int32Value());

  info.GetReturnValue().Set(Nan::Undefined());
}


NAN_METHOD(UseProgram) {
  Nan::HandleScope scope;

  glUseProgram(info[0]->Int32Value());

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(CreateBuffer) {
  Nan::HandleScope scope;

  GLuint buffer;
  glGenBuffers(1, &buffer);
  #ifdef LOGGING
  cout<<"createBuffer "<<buffer<<endl;
  #endif
  registerGLObj(GLOBJECT_TYPE_BUFFER, buffer);
  info.GetReturnValue().Set(Nan::New<Number>(buffer));
}

NAN_METHOD(BindBuffer) {
  Nan::HandleScope scope;

  int target = info[0]->Int32Value();
  int buffer = info[1]->Uint32Value();
  glBindBuffer(target,buffer);

  info.GetReturnValue().Set(Nan::Undefined());
}


NAN_METHOD(CreateFramebuffer) {
  Nan::HandleScope scope;

  GLuint buffer;
  glGenFramebuffers(1, &buffer);
  #ifdef LOGGING
  cout<<"createFrameBuffer "<<buffer<<endl;
  #endif
  registerGLObj(GLOBJECT_TYPE_FRAMEBUFFER, buffer);
  info.GetReturnValue().Set(Nan::New<Number>(buffer));
}


NAN_METHOD(BindFramebuffer) {
  Nan::HandleScope scope;

  int target = info[0]->Int32Value();
  int buffer = info[1]->IsNull() ? 0 : info[1]->Int32Value();

  glBindFramebuffer(target, buffer);

  info.GetReturnValue().Set(Nan::Undefined());
}


NAN_METHOD(FramebufferTexture2D) {
  Nan::HandleScope scope;

  int target = info[0]->Int32Value();
  int attachment = info[1]->Int32Value();
  int textarget = info[2]->Int32Value();
  int texture = info[3]->Int32Value();
  int level = info[4]->Int32Value();

  glFramebufferTexture2D(target, attachment, textarget, texture, level);

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(BufferData) {
  Nan::HandleScope scope;

  int target = info[0]->Int32Value();
  if(info[1]->IsObject()) {
    Local<Object> obj = Local<Object>::Cast(info[1]);
    GLenum usage = info[2]->Int32Value();

    CHECK_ARRAY_BUFFER(obj);



    int element_size = 1;
    Local<ArrayBufferView> arr = Local<ArrayBufferView>::Cast(obj);
    int size = arr->ByteLength() * element_size;
    void* data = (uint8_t*)arr->Buffer()->GetContents().Data() + arr->ByteOffset();

    glBufferData(target, size, data, usage);
  }
  else if(info[1]->IsNumber()) {
    GLsizeiptr size = info[1]->Uint32Value();
    GLenum usage = info[2]->Int32Value();
    glBufferData(target, size, NULL, usage);
  }
  info.GetReturnValue().Set(Nan::Undefined());
}


NAN_METHOD(BufferSubData) {
  Nan::HandleScope scope;

  int target = info[0]->Int32Value();
  int offset = info[1]->Int32Value();
  Local<Object> obj = Local<Object>::Cast(info[2]);

   int element_size = 1;
   Local<ArrayBufferView> arr = Local<ArrayBufferView>::Cast(obj);
   int size = arr->ByteLength() * element_size;
   void* data = (uint8_t*)arr->Buffer()->GetContents().Data() + arr->ByteOffset();

  glBufferSubData(target, offset, size, data);

  info.GetReturnValue().Set(Nan::Undefined());
}


NAN_METHOD(BlendEquation) {
  Nan::HandleScope scope;

  int mode=info[0]->Int32Value();;

  glBlendEquation(mode);

  info.GetReturnValue().Set(Nan::Undefined());
}


NAN_METHOD(BlendFunc) {
  Nan::HandleScope scope;

  int sfactor=info[0]->Int32Value();;
  int dfactor=info[1]->Int32Value();;

  glBlendFunc(sfactor,dfactor);

  info.GetReturnValue().Set(Nan::Undefined());
}


NAN_METHOD(EnableVertexAttribArray) {
  Nan::HandleScope scope;

  glEnableVertexAttribArray(info[0]->Int32Value());

  info.GetReturnValue().Set(Nan::Undefined());
}


NAN_METHOD(VertexAttribPointer) {
  Nan::HandleScope scope;

  int indx = info[0]->Int32Value();
  int size = info[1]->Int32Value();
  int type = info[2]->Int32Value();
  int normalized = info[3]->BooleanValue();
  int stride = info[4]->Int32Value();
  long offset = info[5]->Int32Value();

  //    printf("VertexAttribPointer %d %d %d %d %d %d\n", indx, size, type, normalized, stride, offset);
  glVertexAttribPointer(indx, size, type, normalized, stride, (const GLvoid *)offset);

  info.GetReturnValue().Set(Nan::Undefined());
}


NAN_METHOD(ActiveTexture) {
  Nan::HandleScope scope;

  glActiveTexture(info[0]->Int32Value());
  info.GetReturnValue().Set(Nan::Undefined());
}


NAN_METHOD(DrawElements) {
  Nan::HandleScope scope;

  int mode = info[0]->Int32Value();
  int count = info[1]->Int32Value();
  int type = info[2]->Int32Value();
  GLvoid *offset = reinterpret_cast<GLvoid*>(info[3]->Uint32Value());
  glDrawElements(mode, count, type, offset);
  info.GetReturnValue().Set(Nan::Undefined());
}


NAN_METHOD(Flush) {
  Nan::HandleScope scope;
  glFlush();
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(Finish) {
  Nan::HandleScope scope;
  glFinish();
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(VertexAttrib1f) {
  Nan::HandleScope scope;

  GLuint indx = info[0]->Int32Value();
  float x = (float) info[1]->NumberValue();

  glVertexAttrib1f(indx, x);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(VertexAttrib2f) {
  Nan::HandleScope scope;

  GLuint indx = info[0]->Int32Value();
  float x = (float) info[1]->NumberValue();
  float y = (float) info[2]->NumberValue();

  glVertexAttrib2f(indx, x, y);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(VertexAttrib3f) {
  Nan::HandleScope scope;

  GLuint indx = info[0]->Int32Value();
  float x = (float) info[1]->NumberValue();
  float y = (float) info[2]->NumberValue();
  float z = (float) info[3]->NumberValue();

  glVertexAttrib3f(indx, x, y, z);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(VertexAttrib4f) {
  Nan::HandleScope scope;

  GLuint indx = info[0]->Int32Value();
  float x = (float) info[1]->NumberValue();
  float y = (float) info[2]->NumberValue();
  float z = (float) info[3]->NumberValue();
  float w = (float) info[4]->NumberValue();

  glVertexAttrib4f(indx, x, y, z, w);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(VertexAttrib1fv) {
  Nan::HandleScope scope;

  int indx = info[0]->Int32Value();
  GLfloat *data = getArrayData<GLfloat>(info[1]);
  glVertexAttrib1fv(indx, data);

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(VertexAttrib2fv) {
  Nan::HandleScope scope;

  int indx = info[0]->Int32Value();
  GLfloat *data = getArrayData<GLfloat>(info[1]);
  glVertexAttrib2fv(indx, data);

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(VertexAttrib3fv) {
  Nan::HandleScope scope;

  int indx = info[0]->Int32Value();
  GLfloat *data = getArrayData<GLfloat>(info[1]);
  glVertexAttrib3fv(indx, data);

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(VertexAttrib4fv) {
  Nan::HandleScope scope;

  int indx = info[0]->Int32Value();
  GLfloat *data = getArrayData<GLfloat>(info[1]);
  glVertexAttrib4fv(indx, data);

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(BlendColor) {
  Nan::HandleScope scope;

  GLclampf r= (float) info[0]->NumberValue();
  GLclampf g= (float) info[1]->NumberValue();
  GLclampf b= (float) info[2]->NumberValue();
  GLclampf a= (float) info[3]->NumberValue();

  glBlendColor(r,g,b,a);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(BlendEquationSeparate) {
  Nan::HandleScope scope;

  GLenum modeRGB= info[0]->Int32Value();
  GLenum modeAlpha= info[1]->Int32Value();

  glBlendEquationSeparate(modeRGB,modeAlpha);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(BlendFuncSeparate) {
  Nan::HandleScope scope;

  GLenum srcRGB= info[0]->Int32Value();
  GLenum dstRGB= info[1]->Int32Value();
  GLenum srcAlpha= info[2]->Int32Value();
  GLenum dstAlpha= info[3]->Int32Value();

  glBlendFuncSeparate(srcRGB,dstRGB,srcAlpha,dstAlpha);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(ClearStencil) {
  Nan::HandleScope scope;

  GLint s = info[0]->Int32Value();

  glClearStencil(s);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(ColorMask) {
  Nan::HandleScope scope;

  GLboolean r = info[0]->BooleanValue();
  GLboolean g = info[1]->BooleanValue();
  GLboolean b = info[2]->BooleanValue();
  GLboolean a = info[3]->BooleanValue();

  glColorMask(r,g,b,a);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(CopyTexImage2D) {
  Nan::HandleScope scope;

  GLenum target = info[0]->Int32Value();
  GLint level = info[1]->Int32Value();
  GLenum internalformat = info[2]->Int32Value();
  GLint x = info[3]->Int32Value();
  GLint y = info[4]->Int32Value();
  GLsizei width = info[5]->Int32Value();
  GLsizei height = info[6]->Int32Value();
  GLint border = info[7]->Int32Value();

  glCopyTexImage2D( target, level, internalformat, x, y, width, height, border);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(CopyTexSubImage2D) {
  Nan::HandleScope scope;

  GLenum target = info[0]->Int32Value();
  GLint level = info[1]->Int32Value();
  GLint xoffset = info[2]->Int32Value();
  GLint yoffset = info[3]->Int32Value();
  GLint x = info[4]->Int32Value();
  GLint y = info[5]->Int32Value();
  GLsizei width = info[6]->Int32Value();
  GLsizei height = info[7]->Int32Value();

  glCopyTexSubImage2D( target, level, xoffset, yoffset, x, y, width, height);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(CullFace) {
  Nan::HandleScope scope;

  GLenum mode = info[0]->Int32Value();

  glCullFace(mode);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(DepthMask) {
  Nan::HandleScope scope;

  GLboolean flag = info[0]->BooleanValue();

  glDepthMask(flag);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(DepthRange) {
  Nan::HandleScope scope;

  GLclampf zNear = (float) info[0]->NumberValue();
  GLclampf zFar = (float) info[1]->NumberValue();

  glDepthRangef(zNear, zFar);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(DisableVertexAttribArray) {
  Nan::HandleScope scope;

  GLuint index = info[0]->Int32Value();

  glDisableVertexAttribArray(index);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(Hint) {
  Nan::HandleScope scope;

  GLenum target = info[0]->Int32Value();
  GLenum mode = info[1]->Int32Value();

  glHint(target, mode);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(IsEnabled) {
  Nan::HandleScope scope;

  GLenum cap = info[0]->Int32Value();

  bool ret=glIsEnabled(cap)!=0;
  info.GetReturnValue().Set(Nan::New<Boolean>(ret));
}

NAN_METHOD(LineWidth) {
  Nan::HandleScope scope;

  GLfloat width = (float) info[0]->NumberValue();

  glLineWidth(width);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(PolygonOffset) {
  Nan::HandleScope scope;

  GLfloat factor = (float) info[0]->NumberValue();
  GLfloat units = (float) info[1]->NumberValue();

  glPolygonOffset(factor, units);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(SampleCoverage) {
  Nan::HandleScope scope;

  GLclampf value = (float) info[0]->NumberValue();
  GLboolean invert = info[1]->BooleanValue();

  glSampleCoverage(value, invert);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(Scissor) {
  Nan::HandleScope scope;

  GLint x = info[0]->Int32Value();
  GLint y = info[1]->Int32Value();
  GLsizei width = info[2]->Int32Value();
  GLsizei height = info[3]->Int32Value();

  glScissor(x, y, width, height);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(StencilFunc) {
  Nan::HandleScope scope;

  GLenum func = info[0]->Int32Value();
  GLint ref = info[1]->Int32Value();
  GLuint mask = info[2]->Int32Value();

  glStencilFunc(func, ref, mask);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(StencilFuncSeparate) {
  Nan::HandleScope scope;

  GLenum face = info[0]->Int32Value();
  GLenum func = info[1]->Int32Value();
  GLint ref = info[2]->Int32Value();
  GLuint mask = info[3]->Int32Value();

  glStencilFuncSeparate(face, func, ref, mask);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(StencilMask) {
  Nan::HandleScope scope;

  GLuint mask = info[0]->Uint32Value();

  glStencilMask(mask);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(StencilMaskSeparate) {
  Nan::HandleScope scope;

  GLenum face = info[0]->Int32Value();
  GLuint mask = info[1]->Uint32Value();

  glStencilMaskSeparate(face, mask);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(StencilOp) {
  Nan::HandleScope scope;

  GLenum fail = info[0]->Int32Value();
  GLenum zfail = info[1]->Int32Value();
  GLenum zpass = info[2]->Int32Value();

  glStencilOp(fail, zfail, zpass);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(StencilOpSeparate) {
  Nan::HandleScope scope;

  GLenum face = info[0]->Int32Value();
  GLenum fail = info[1]->Int32Value();
  GLenum zfail = info[2]->Int32Value();
  GLenum zpass = info[3]->Int32Value();

  glStencilOpSeparate(face, fail, zfail, zpass);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(BindRenderbuffer) {
  Nan::HandleScope scope;

  GLenum target = info[0]->Int32Value();
  GLuint buffer = info[1]->IsNull() ? 0 : info[1]->Int32Value();

  glBindRenderbuffer(target, buffer);

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(CreateRenderbuffer) {
  Nan::HandleScope scope;

  GLuint renderbuffers;
  glGenRenderbuffers(1,&renderbuffers);
  #ifdef LOGGING
  cout<<"createRenderBuffer "<<renderbuffers<<endl;
  #endif
  registerGLObj(GLOBJECT_TYPE_RENDERBUFFER, renderbuffers);
  info.GetReturnValue().Set(Nan::New<Number>(renderbuffers));
}

NAN_METHOD(DeleteBuffer) {
  Nan::HandleScope scope;

  GLuint buffer = info[0]->Uint32Value();

  glDeleteBuffers(1,&buffer);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(DeleteFramebuffer) {
  Nan::HandleScope scope;

  GLuint buffer = info[0]->Uint32Value();

  glDeleteFramebuffers(1,&buffer);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(DeleteProgram) {
  Nan::HandleScope scope;

  GLuint program = info[0]->Uint32Value();

  glDeleteProgram(program);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(DeleteRenderbuffer) {
  Nan::HandleScope scope;

  GLuint renderbuffer = info[0]->Uint32Value();

  glDeleteRenderbuffers(1, &renderbuffer);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(DeleteShader) {
  Nan::HandleScope scope;

  GLuint shader = info[0]->Uint32Value();

  glDeleteShader(shader);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(DeleteTexture) {
  Nan::HandleScope scope;

  GLuint texture = info[0]->Uint32Value();

  glDeleteTextures(1,&texture);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(DetachShader) {
  Nan::HandleScope scope;

  GLuint program = info[0]->Uint32Value();
  GLuint shader = info[1]->Uint32Value();

  glDetachShader(program, shader);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(FramebufferRenderbuffer) {
  Nan::HandleScope scope;

  GLenum target = info[0]->Int32Value();
  GLenum attachment = info[1]->Int32Value();
  GLenum renderbuffertarget = info[2]->Int32Value();
  GLuint renderbuffer = info[3]->Uint32Value();

  glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(GetVertexAttribOffset) {
  Nan::HandleScope scope;

  GLuint index = info[0]->Uint32Value();
  GLenum pname = info[1]->Int32Value();
  void *ret=NULL;

  glGetVertexAttribPointerv(index, pname, &ret);
  info.GetReturnValue().Set(JS_INT(ToGLuint(ret)));
}

NAN_METHOD(IsBuffer) {
  Nan::HandleScope scope;

  info.GetReturnValue().Set(Nan::New<Boolean>(glIsBuffer(info[0]->Uint32Value())!=0));
}

NAN_METHOD(IsFramebuffer) {
  Nan::HandleScope scope;

  info.GetReturnValue().Set(JS_BOOL(glIsFramebuffer(info[0]->Uint32Value())!=0));
}

NAN_METHOD(IsProgram) {
  Nan::HandleScope scope;

  info.GetReturnValue().Set(JS_BOOL(glIsProgram(info[0]->Uint32Value())!=0));
}

NAN_METHOD(IsRenderbuffer) {
  Nan::HandleScope scope;

  info.GetReturnValue().Set(JS_BOOL(glIsRenderbuffer( info[0]->Uint32Value())!=0));
}

NAN_METHOD(IsShader) {
  Nan::HandleScope scope;

  info.GetReturnValue().Set(JS_BOOL(glIsShader(info[0]->Uint32Value())!=0));
}

NAN_METHOD(IsTexture) {
  Nan::HandleScope scope;

  info.GetReturnValue().Set(JS_BOOL(glIsTexture(info[0]->Uint32Value())!=0));
}

NAN_METHOD(RenderbufferStorage) {
  Nan::HandleScope scope;

  GLenum target = info[0]->Int32Value();
  GLenum internalformat = info[1]->Int32Value();
  GLsizei width = info[2]->Uint32Value();
  GLsizei height = info[3]->Uint32Value();

  glRenderbufferStorage(target, internalformat, width, height);
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(GetShaderSource) {
  Nan::HandleScope scope;

  int shader = info[0]->Int32Value();

  GLint len;
  glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &len);
  GLchar *source=new GLchar[len];
  glGetShaderSource(shader, len, NULL, source);

  Local<String> str = JS_STR(source);
  delete [] source;

  info.GetReturnValue().Set(str);
}

NAN_METHOD(ValidateProgram) {
  Nan::HandleScope scope;

  glValidateProgram(info[0]->Int32Value());

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(TexSubImage2D) {
  Nan::HandleScope scope;

  GLenum target = info[0]->Int32Value();
  GLint level = info[1]->Int32Value();
  GLint xoffset = info[2]->Int32Value();
  GLint yoffset = info[3]->Int32Value();
  GLsizei width = info[4]->Int32Value();
  GLsizei height = info[5]->Int32Value();
  GLenum format = info[6]->Int32Value();
  GLenum type = info[7]->Int32Value();
  void *pixels=getImageData(info[8]);

  glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(ReadPixels) {
  Nan::HandleScope scope;

  GLint x = info[0]->Int32Value();
  GLint y = info[1]->Int32Value();
  GLsizei width = info[2]->Int32Value();
  GLsizei height = info[3]->Int32Value();
  GLenum format = info[4]->Int32Value();
  GLenum type = info[5]->Int32Value();
  void *pixels=getImageData(info[6]);

  glReadPixels(x, y, width, height, format, type, pixels);

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(GetTexParameter) {
  Nan::HandleScope scope;

  GLenum target = info[0]->Int32Value();
  GLenum pname = info[1]->Int32Value();

  GLint param_value=0;
  glGetTexParameteriv(target, pname, &param_value);

  info.GetReturnValue().Set(Nan::New<Number>(param_value));
}

NAN_METHOD(GetActiveAttrib) {
  Nan::HandleScope scope;

  GLuint program = info[0]->Int32Value();
  GLuint index = info[1]->Int32Value();

  char name[1024];
  GLsizei length=0;
  GLenum type;
  GLsizei size;
  glGetActiveAttrib(program, index, 1024, &length, &size, &type, name);

  Local<Array> activeInfo = Nan::New<Array>(3);
  activeInfo->Set(JS_STR("size"), JS_INT(size));
  activeInfo->Set(JS_STR("type"), JS_INT((int)type));
  activeInfo->Set(JS_STR("name"), JS_STR(name));

  info.GetReturnValue().Set(activeInfo);
}

NAN_METHOD(GetActiveUniform) {
  Nan::HandleScope scope;

  GLuint program = info[0]->Int32Value();
  GLuint index = info[1]->Int32Value();

  char name[1024];
  GLsizei length=0;
  GLenum type;
  GLsizei size;
  glGetActiveUniform(program, index, 1024, &length, &size, &type, name);

  Local<Array> activeInfo = Nan::New<Array>(3);
  activeInfo->Set(JS_STR("size"), JS_INT(size));
  activeInfo->Set(JS_STR("type"), JS_INT((int)type));
  activeInfo->Set(JS_STR("name"), JS_STR(name));

  info.GetReturnValue().Set(activeInfo);
}

NAN_METHOD(GetAttachedShaders) {
  Nan::HandleScope scope;

  GLuint program = info[0]->Int32Value();

  GLuint shaders[1024];
  GLsizei count;
  glGetAttachedShaders(program, 1024, &count, shaders);

  Local<Array> shadersArr = Nan::New<Array>(count);
  for(int i=0;i<count;i++)
    shadersArr->Set(i, JS_INT((int)shaders[i]));

  info.GetReturnValue().Set(shadersArr);
}

NAN_METHOD(GetParameter) {
  Nan::HandleScope scope;

  GLenum name = info[0]->Int32Value();

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
    info.GetReturnValue().Set(JS_BOOL(params!=0));
    break;
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
    info.GetReturnValue().Set(JS_FLOAT(params));
    break;
  }
  case GL_RENDERER:
  case GL_SHADING_LANGUAGE_VERSION:
  case GL_VENDOR:
  case GL_VERSION:
  case GL_EXTENSIONS:
  {
    // return a string
    char *params=(char*) ::glGetString(name);

    if(params!=NULL){
      info.GetReturnValue().Set(JS_STR(params));
    }else{
      info.GetReturnValue().Set(Nan::Undefined());
    }

    break;
  }
  case GL_MAX_VIEWPORT_DIMS:
  {
    // return a int32[2]
    GLint params[2];
    ::glGetIntegerv(name, params);

    Local<Array> arr=Nan::New<Array>(2);
    arr->Set(0,JS_INT(params[0]));
    arr->Set(1,JS_INT(params[1]));
    info.GetReturnValue().Set(arr);
    break;
  }
  case GL_SCISSOR_BOX:
  case GL_VIEWPORT:
  {
    // return a int32[4]
    GLint params[4];
    ::glGetIntegerv(name, params);

    Local<Array> arr=Nan::New<Array>(4);
    arr->Set(0,JS_INT(params[0]));
    arr->Set(1,JS_INT(params[1]));
    arr->Set(2,JS_INT(params[2]));
    arr->Set(3,JS_INT(params[3]));
    info.GetReturnValue().Set(arr);
    break;
  }
  case GL_ALIASED_LINE_WIDTH_RANGE:
  case GL_ALIASED_POINT_SIZE_RANGE:
  case GL_DEPTH_RANGE:
  {
    // return a float[2]
    GLfloat params[2];
    ::glGetFloatv(name, params);
    Local<Array> arr=Nan::New<Array>(2);
    arr->Set(0,JS_FLOAT(params[0]));
    arr->Set(1,JS_FLOAT(params[1]));
    info.GetReturnValue().Set(arr);
    break;
  }
  case GL_BLEND_COLOR:
  case GL_COLOR_CLEAR_VALUE:
  {
    // return a float[4]
    GLfloat params[4];
    ::glGetFloatv(name, params);
    Local<Array> arr=Nan::New<Array>(4);
    arr->Set(0,JS_FLOAT(params[0]));
    arr->Set(1,JS_FLOAT(params[1]));
    arr->Set(2,JS_FLOAT(params[2]));
    arr->Set(3,JS_FLOAT(params[3]));
    info.GetReturnValue().Set(arr);
    break;
  }
  case GL_COLOR_WRITEMASK:
  {
    // return a boolean[4]
    GLboolean params[4];
    ::glGetBooleanv(name, params);
    Local<Array> arr=Nan::New<Array>(4);
    arr->Set(0,JS_BOOL(params[0]==1));
    arr->Set(1,JS_BOOL(params[1]==1));
    arr->Set(2,JS_BOOL(params[2]==1));
    arr->Set(3,JS_BOOL(params[3]==1));
    info.GetReturnValue().Set(arr);
    break;
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
    info.GetReturnValue().Set(JS_INT(params));
    break;
  }
  default: {
    // return a long
    GLint params;
    ::glGetIntegerv(name, &params);
    info.GetReturnValue().Set(JS_INT(params));
  }
  }

  //info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(GetBufferParameter) {
  Nan::HandleScope scope;

  GLenum target = info[0]->Int32Value();
  GLenum pname = info[1]->Int32Value();

  GLint params;
  glGetBufferParameteriv(target,pname,&params);
  info.GetReturnValue().Set(JS_INT(params));
}

NAN_METHOD(GetFramebufferAttachmentParameter) {
  Nan::HandleScope scope;

  GLenum target = info[0]->Int32Value();
  GLenum attachment = info[1]->Int32Value();
  GLenum pname = info[2]->Int32Value();

  GLint params;
  glGetFramebufferAttachmentParameteriv(target,attachment, pname,&params);
  info.GetReturnValue().Set(JS_INT(params));
}

NAN_METHOD(GetProgramInfoLog) {
  Nan::HandleScope scope;

  GLuint program = info[0]->Int32Value();
  int Len = 1024;
  char Error[1024];
  glGetProgramInfoLog(program, 1024, &Len, Error);

  info.GetReturnValue().Set(JS_STR(Error));
}

NAN_METHOD(GetRenderbufferParameter) {
  Nan::HandleScope scope;

  int target = info[0]->Int32Value();
  int pname = info[1]->Int32Value();
  int value = 0;
  glGetRenderbufferParameteriv(target,pname,&value);

  info.GetReturnValue().Set(JS_INT(value));
}

NAN_METHOD(GetUniform) {
  Nan::HandleScope scope;

  GLuint program = info[0]->Int32Value();
  GLint location = info[1]->Int32Value();
  if(location < 0 ) info.GetReturnValue().Set(Nan::Undefined());

  float data[16]; // worst case scenario is 16 floats

  glGetUniformfv(program, location, data);

  Local<Array> arr=Nan::New<Array>(16);
  for(int i=0;i<16;i++)
    arr->Set(i,JS_FLOAT(data[i]));

  info.GetReturnValue().Set(arr);
}

NAN_METHOD(GetVertexAttrib) {
  Nan::HandleScope scope;

  GLuint index = info[0]->Int32Value();
  GLuint pname = info[1]->Int32Value();

  GLint value=0;

  switch (pname) {
  case GL_VERTEX_ATTRIB_ARRAY_ENABLED:
  case GL_VERTEX_ATTRIB_ARRAY_NORMALIZED:
    glGetVertexAttribiv(index,pname,&value);
    info.GetReturnValue().Set(JS_BOOL(value!=0));
    break;
  case GL_VERTEX_ATTRIB_ARRAY_SIZE:
  case GL_VERTEX_ATTRIB_ARRAY_STRIDE:
  case GL_VERTEX_ATTRIB_ARRAY_TYPE:
    glGetVertexAttribiv(index,pname,&value);
    info.GetReturnValue().Set(JS_INT(value));
    break;
  case GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING:
    glGetVertexAttribiv(index,pname,&value);
    info.GetReturnValue().Set(JS_INT(value));
    break;
  case GL_CURRENT_VERTEX_ATTRIB: {
    float vextex_attribs[4];
    glGetVertexAttribfv(index,pname,vextex_attribs);
    Local<Array> arr=Nan::New<Array>(4);
    arr->Set(0,JS_FLOAT(vextex_attribs[0]));
    arr->Set(1,JS_FLOAT(vextex_attribs[1]));
    arr->Set(2,JS_FLOAT(vextex_attribs[2]));
    arr->Set(3,JS_FLOAT(vextex_attribs[3]));
    info.GetReturnValue().Set(arr);
    break;
  }
  default:
    Nan::ThrowError("GetVertexAttrib: Invalid Enum");
  }

  //info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(GetSupportedExtensions) {
  Nan::HandleScope scope;

  char *extensions=(char*) glGetString(GL_EXTENSIONS);

  info.GetReturnValue().Set(JS_STR(extensions));
}

// TODO GetExtension(name) return the extension name if found, should be an object...
NAN_METHOD(GetExtension) {
  Nan::HandleScope scope;

  String::Utf8Value name(info[0]);
  char *sname=*name;
  char *extensions=(char*) glGetString(GL_EXTENSIONS);
  char *ext=strcasestr(extensions, sname);

  if(ext==NULL){
      info.GetReturnValue().Set(Nan::Undefined());
  }else{
     info.GetReturnValue().Set(JS_STR(ext, (int)::strlen(sname)));
  }
}

NAN_METHOD(CheckFramebufferStatus) {
  Nan::HandleScope scope;

  GLenum target=info[0]->Int32Value();

  info.GetReturnValue().Set(JS_INT((int)glCheckFramebufferStatus(target)));
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
