var gl = module.exports = require('../build/Release/webgl.node');
var Image = require('./image');

gl.WebGLProgram=function (_) { this._ = _; }
gl.WebGLShader=function (_) { this._ = _; }
gl.WebGLBuffer=function (_) { this._ = _; }
gl.WebGLFramebuffer=function (_) { this._ = _; }
gl.WebGLRenderbuffer=function (_) { this._ = _; }
gl.WebGLTexture=function (_) { this._ = _; }
gl.WebGLActiveInfo=function (_) { this._=_; this.size=_.size; this.type=_.type; this.name=_.name; }
gl.WebGLUniformLocation=function (_) { this._ = _; }

////////////////////////////////////////////////////////////////////////////////

var _getSupportedExtensions = gl.getSupportedExtensions;
gl.getSupportedExtensions = function getSupportedExtensions() {
  return _getSupportedExtensions().split(" ");
}

var _extension_constants = {
    OES_standard_derivatives: {
        FRAGMENT_SHADER_DERIVATIVE_HINT_OES: 35723
    },
    EXT_texture_filter_anisotropic: {
        TEXTURE_MAX_ANISOTROPY_EXT: 34046, MAX_TEXTURE_MAX_ANISOTROPY_EXT: 34047
    }
};

var _getExtension = gl.getExtension;
gl.getExtension = function getExtension(name) {
  if (!(arguments.length === 1 && typeof name === "string")) {
    throw new TypeError('Expected getExtension(string name)');
  }
  return _getExtension(name)? _extension_constants[name] || {}: null;
}

var _activeTexture = gl.activeTexture;
gl.activeTexture = function activeTexture(texture) {
  if (!(arguments.length === 1 && typeof texture === "number")) {
    throw new TypeError('Expected activeTexture(number texture)');
  }
  return _activeTexture(texture);
}

var _attachShader = gl.attachShader;
gl.attachShader = function attachShader(program, shader) {
  if (!(arguments.length === 2 && (program === null || program instanceof gl.WebGLProgram) && (shader === null || shader instanceof gl.WebGLShader))) {
    throw new TypeError('Expected attachShader(WebGLProgram program, WebGLShader shader)');
  }
  return _attachShader(program ? program._ : 0, shader ? shader._ : 0);
}

var _bindAttribLocation = gl.bindAttribLocation;
gl.bindAttribLocation = function bindAttribLocation(program, index, name) {
  if (!(arguments.length === 3 && (program === null || program instanceof gl.WebGLProgram) && typeof index === "number" && typeof name === "string")) {
    throw new TypeError('Expected bindAttribLocation(WebGLProgram program, number index, string name)');
  }
  return _bindAttribLocation(program ? program._ : 0, index, name);
}

var _bindBuffer = gl.bindBuffer;
gl.bindBuffer = function bindBuffer(target, buffer) {
  if (!(arguments.length === 2 && typeof target === "number" && (buffer === null || buffer instanceof gl.WebGLBuffer))) {
    throw new TypeError('Expected bindBuffer(number target, WebGLBuffer buffer)');
  }
  return _bindBuffer(target, buffer ? buffer._ : 0);
}

var _bindFramebuffer = gl.bindFramebuffer;
gl.bindFramebuffer = function bindFramebuffer(target, framebuffer) {
  if (!(arguments.length === 2 && typeof target === "number" && (framebuffer === null || framebuffer instanceof gl.WebGLFramebuffer))) {
    throw new TypeError('Expected bindFramebuffer(number target, WebGLFramebuffer framebuffer)');
  }
  return _bindFramebuffer(target, framebuffer ? framebuffer._ : 0);
}

var _bindRenderbuffer = gl.bindRenderbuffer;
gl.bindRenderbuffer = function bindRenderbuffer(target, renderbuffer) {
  if (!(arguments.length === 2 && typeof target === "number" && (renderbuffer === null || renderbuffer instanceof gl.WebGLRenderbuffer))) {
    throw new TypeError('Expected bindRenderbuffer(number target, WebGLRenderbuffer renderbuffer)');
  }
  return _bindRenderbuffer(target, renderbuffer ? renderbuffer._ : 0);
}

var _bindTexture = gl.bindTexture;
gl.bindTexture = function bindTexture(target, texture) {
  if (!(arguments.length === 2 && typeof target === "number" && (texture === null || texture instanceof gl.WebGLTexture))) {
    throw new TypeError('Expected bindTexture(number target, WebGLTexture texture)');
  }
  return _bindTexture(target, texture ? texture._ : 0);
}

var _blendColor = gl.blendColor;
gl.blendColor = function blendColor(red, green, blue, alpha) {
  if (!(arguments.length === 4 && typeof red === "number" && typeof green === "number" && typeof blue === "number" && typeof alpha === "number")) {
    throw new TypeError('Expected blendColor(number red, number green, number blue, number alpha)');
  }
  return _blendColor(red, green, blue, alpha);
}

var _blendEquation = gl.blendEquation;
gl.blendEquation = function blendEquation(mode) {
  if (!(arguments.length === 1 && typeof mode === "number")) {
    throw new TypeError('Expected blendEquation(number mode)');
  }
  return _blendEquation(mode);
}

var _blendEquationSeparate = gl.blendEquationSeparate;
gl.blendEquationSeparate = function blendEquationSeparate(modeRGB, modeAlpha) {
  if (!(arguments.length === 2 && typeof modeRGB === "number" && typeof modeAlpha === "number")) {
    throw new TypeError('Expected blendEquationSeparate(number modeRGB, number modeAlpha)');
  }
  return _blendEquationSeparate(modeRGB, modeAlpha);
}

var _blendFunc = gl.blendFunc;
gl.blendFunc = function blendFunc(sfactor, dfactor) {
  if (!(arguments.length === 2 && typeof sfactor === "number" && typeof dfactor === "number")) {
    throw new TypeError('Expected blendFunc(number sfactor, number dfactor)');
  }
  return _blendFunc(sfactor, dfactor);
}

var _blendFuncSeparate = gl.blendFuncSeparate;
gl.blendFuncSeparate = function blendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha) {
  if (!(arguments.length === 4 && typeof srcRGB === "number" && typeof dstRGB === "number" && typeof srcAlpha === "number" && typeof dstAlpha === "number")) {
    throw new TypeError('Expected blendFuncSeparate(number srcRGB, number dstRGB, number srcAlpha, number dstAlpha)');
  }
  return _blendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
}

var _bufferData = gl.bufferData;
gl.bufferData = function bufferData(target, data, usage) {
  if (!(arguments.length === 3 && typeof target === "number" && 
      (typeof data === "object" || typeof data === "number") && typeof usage === "number")) {
    throw new TypeError('Expected bufferData(number target, ArrayBuffer data, number usage) or bufferData(number target, number size, number usage)');
  }
  return _bufferData(target, data, usage);
}

var _bufferSubData = gl.bufferSubData;
gl.bufferSubData = function bufferSubData(target, offset, data) {
  if (!(arguments.length === 3 && typeof target === "number" && typeof offset === "number" && typeof data === "object")) {
    throw new TypeError('Expected bufferSubData(number target, number offset, ArrayBuffer data)');
  }
  return _bufferSubData(target, offset, data);
}

var _checkFramebufferStatus = gl.checkFramebufferStatus;
gl.checkFramebufferStatus = function checkFramebufferStatus(target) {
  if (!(arguments.length === 1 && typeof target === "number")) {
    throw new TypeError('Expected checkFramebufferStatus(number target)');
  }
  return _checkFramebufferStatus(target);
}

var _clear = gl.clear;
gl.clear = function clear(mask) {
  if (!(arguments.length === 1 && typeof mask === "number")) {
    throw new TypeError('Expected clear(number mask)');
  }
  return _clear(mask);
}

var _clearColor = gl.clearColor;
gl.clearColor = function clearColor(red, green, blue, alpha) {
  if (!(arguments.length === 4 && typeof red === "number" && typeof green === "number" && typeof blue === "number" && typeof alpha === "number")) {
    throw new TypeError('Expected clearColor(number red, number green, number blue, number alpha)');
  }
  return _clearColor(red, green, blue, alpha);
}

var _clearDepth = gl.clearDepth;
gl.clearDepth = function clearDepth(depth) {
  if (!(arguments.length === 1 && typeof depth === "number")) {
    throw new TypeError('Expected clearDepth(number depth)');
  }
  return _clearDepth(depth);
}

var _clearStencil = gl.clearStencil;
gl.clearStencil = function clearStencil(s) {
  if (!(arguments.length === 1 && typeof s === "number")) {
    throw new TypeError('Expected clearStencil(number s)');
  }
  return _clearStencil(s);
}

var _colorMask = gl.colorMask;
gl.colorMask = function colorMask(red, green, blue, alpha) {
  if (!(arguments.length === 4 && typeof red === "boolean" && typeof green === "boolean" && typeof blue === "boolean" && typeof alpha === "boolean")) {
    throw new TypeError('Expected colorMask(boolean red, boolean green, boolean blue, boolean alpha)');
  }
  return _colorMask(red, green, blue, alpha);
}

var _compileShader = gl.compileShader;
gl.compileShader = function compileShader(shader) {
  if (!(arguments.length === 1 && (shader === null || shader instanceof gl.WebGLShader))) {
    throw new TypeError('Expected compileShader(WebGLShader shader)');
  }
  return _compileShader(shader ? shader._ : 0);
}

var _copyTexImage2D = gl.copyTexImage2D;
gl.copyTexImage2D = function copyTexImage2D(target, level, internalformat, x, y, width, height, border) {
  if (!(arguments.length === 8 && typeof target === "number" && typeof level === "number" && typeof internalformat === "number" && typeof x === "number" && typeof y === "number" && typeof width === "number" && typeof height === "number" && typeof border === "number")) {
    throw new TypeError('Expected copyTexImage2D(number target, number level, number internalformat, number x, number y, number width, number height, number border)');
  }
  return _copyTexImage2D(target, level, internalformat, x, y, width, height, border);
}

var _copyTexSubImage2D = gl.copyTexSubImage2D;
gl.copyTexSubImage2D = function copyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height) {
  if (!(arguments.length === 8 && typeof target === "number" && typeof level === "number" && typeof xoffset === "number" && typeof yoffset === "number" && typeof x === "number" && typeof y === "number" && typeof width === "number" && typeof height === "number")) {
    throw new TypeError('Expected copyTexSubImage2D(number target, number level, number xoffset, number yoffset, number x, number y, number width, number height)');
  }
  return _copyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
}

var _createBuffer = gl.createBuffer;
gl.createBuffer = function createBuffer() {
  if (!(arguments.length === 0)) {
    throw new TypeError('Expected createBuffer()');
  }
  return new gl.WebGLBuffer(_createBuffer());
}

var _createFramebuffer = gl.createFramebuffer;
gl.createFramebuffer = function () {
  if (!(arguments.length === 0)) {
    throw new TypeError('Expected createFramebuffer()');
  }
  return new gl.WebGLFramebuffer(_createFramebuffer());
}

var _createProgram = gl.createProgram;
gl.createProgram = function createProgram() {
  if (!(arguments.length === 0)) {
    throw new TypeError('Expected createProgram()');
  }
  return new gl.WebGLProgram(_createProgram());
}

var _createRenderbuffer = gl.createRenderbuffer;
gl.createRenderbuffer = function createRenderbuffer() {
  if (!(arguments.length === 0)) {
    throw new TypeError('Expected createRenderbuffer()');
  }
  return new gl.WebGLRenderbuffer(_createRenderbuffer());
}

var _createShader = gl.createShader;
gl.createShader = function createShader(type) {
  if (!(arguments.length === 1 && typeof type === "number")) {
    throw new TypeError('Expected createShader(number type)');
  }
  return new gl.WebGLShader(_createShader(type));
}

var _createTexture = gl.createTexture;
gl.createTexture = function createTexture() {
  if (!(arguments.length === 0)) {
    throw new TypeError('Expected createTexture()');
  }
  return new gl.WebGLTexture(_createTexture());
}

var _cullFace = gl.cullFace;
gl.cullFace = function cullFace(mode) {
  if (!(arguments.length === 1 && typeof mode === "number")) {
    throw new TypeError('Expected cullFace(number mode)');
  }
  return _cullFace(mode);
}

var _deleteBuffer = gl.deleteBuffer;
gl.deleteBuffer = function deleteBuffer(buffer) {
  if (!(arguments.length === 1 && (buffer === null || buffer instanceof gl.WebGLBuffer))) {
    throw new TypeError('Expected deleteBuffer(WebGLBuffer buffer)');
  }
  return _deleteBuffer(buffer ? buffer._ : 0);
}

var _deleteFramebuffer = gl.deleteFramebuffer;
gl.deleteFramebuffer = function deleteFramebuffer(framebuffer) {
  if (!(arguments.length === 1 && (framebuffer === null || framebuffer instanceof gl.WebGLFramebuffer))) {
    throw new TypeError('Expected deleteFramebuffer(WebGLFramebuffer framebuffer)');
  }
  return _deleteFramebuffer(framebuffer ? framebuffer._ : 0);
}

var _deleteProgram = gl.deleteProgram;
gl.deleteProgram = function deleteProgram(program) {
  if (!(arguments.length === 1 && (program === null || program instanceof gl.WebGLProgram))) {
    throw new TypeError('Expected deleteProgram(WebGLProgram program)');
  }
  return _deleteProgram(program ? program._ : 0);
}

var _deleteRenderbuffer = gl.deleteRenderbuffer;
gl.deleteRenderbuffer = function deleteRenderbuffer(renderbuffer) {
  if (!(arguments.length === 1 && (renderbuffer === null || renderbuffer instanceof gl.WebGLRenderbuffer))) {
    throw new TypeError('Expected deleteRenderbuffer(WebGLRenderbuffer renderbuffer)');
  }
  return _deleteRenderbuffer(renderbuffer ? renderbuffer._ : 0);
}

var _deleteShader = gl.deleteShader;
gl.deleteShader = function deleteShader(shader) {
  if (!(arguments.length === 1 && (shader === null || shader instanceof gl.WebGLShader))) {
    throw new TypeError('Expected deleteShader(WebGLShader shader)');
  }
  return _deleteShader(shader ? shader._ : 0);
}

var _deleteTexture = gl.deleteTexture;
gl.deleteTexture = function deleteTexture(texture) {
  if (!(arguments.length === 1 && (texture === null || texture instanceof gl.WebGLTexture))) {
    throw new TypeError('Expected deleteTexture(WebGLTexture texture)');
  }
  return _deleteTexture(texture ? texture._ : 0);
}

var _depthFunc = gl.depthFunc;
gl.depthFunc = function depthFunc(func) {
  if (!(arguments.length === 1 && typeof func === "number")) {
    throw new TypeError('Expected depthFunc(number func)');
  }
  return _depthFunc(func);
}

var _depthMask = gl.depthMask;
gl.depthMask = function depthMask(flag) {
  if (!(arguments.length === 1 && typeof flag === "boolean")) {
    throw new TypeError('Expected depthMask(boolean flag)');
  }
  return _depthMask(flag);
}

var _depthRange = gl.depthRange;
gl.depthRange = function depthRange(zNear, zFar) {
  if (!(arguments.length === 2 && typeof zNear === "number" && typeof zFar === "number")) {
    throw new TypeError('Expected depthRange(number zNear, number zFar)');
  }
  return _depthRange(zNear, zFar);
}

var _detachShader = gl.detachShader;
gl.detachShader = function detachShader(program, shader) {
  if (!(arguments.length === 2 && (program === null || program instanceof gl.WebGLProgram) && (shader === null || shader instanceof gl.WebGLShader))) {
    throw new TypeError('Expected detachShader(WebGLProgram program, WebGLShader shader)');
  }
  return _detachShader(program ? program._ : 0, shader ? shader._ : 0);
}

var _disable = gl.disable;
gl.disable = function disable(cap) {
  if (!(arguments.length === 1 && typeof cap === "number")) {
    throw new TypeError('Expected disable(number cap)');
  }
  return _disable(cap);
}

var _disableVertexAttribArray = gl.disableVertexAttribArray;
gl.disableVertexAttribArray = function disableVertexAttribArray(index) {
  if (!(arguments.length === 1 && typeof index === "number")) {
    throw new TypeError('Expected disableVertexAttribArray(number index)');
  }
  return _disableVertexAttribArray(index);
}

var _drawArrays = gl.drawArrays;
gl.drawArrays = function drawArrays(mode, first, count) {
  if (!(arguments.length === 3 && typeof mode === "number" && typeof first === "number" && typeof count === "number")) {
    throw new TypeError('Expected drawArrays(number mode, number first, number count)');
  }
  return _drawArrays(mode, first, count);
}

var _drawElements = gl.drawElements;
gl.drawElements = function drawElements(mode, count, type, offset) {
  if (!(arguments.length === 4 && typeof mode === "number" && typeof count === "number" && typeof type === "number" && typeof offset === "number")) {
    throw new TypeError('Expected drawElements(number mode, number count, number type, number offset)');
  }
  return _drawElements(mode, count, type, offset);
}

var _enable = gl.enable;
gl.enable = function enable(cap) {
  if (!(arguments.length === 1 && typeof cap === "number")) {
    throw new TypeError('Expected enable(number cap)');
  }
  return _enable(cap);
}

var _enableVertexAttribArray = gl.enableVertexAttribArray;
gl.enableVertexAttribArray = function enableVertexAttribArray(index) {
  if (!(arguments.length === 1 && typeof index === "number")) {
    throw new TypeError('Expected enableVertexAttribArray(number index)');
  }
  return _enableVertexAttribArray(index);
}

var _finish = gl.finish;
gl.finish = function finish() {
  if (!(arguments.length === 0)) {
    throw new TypeError('Expected finish()');
  }
  return _finish();
}

var _flush = gl.flush;
gl.flush = function flush() {
  if (!(arguments.length === 0)) {
    throw new TypeError('Expected flush()');
  }
  return _flush();
}

var _framebufferRenderbuffer = gl.framebufferRenderbuffer;
gl.framebufferRenderbuffer = function framebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer) {
  if (!(arguments.length === 4 && typeof target === "number" && typeof attachment === "number" && typeof renderbuffertarget === "number" && (renderbuffer === null || renderbuffer instanceof gl.WebGLRenderbuffer))) {
    throw new TypeError('Expected framebufferRenderbuffer(number target, number attachment, number renderbuffertarget, WebGLRenderbuffer renderbuffer)');
  }
  return _framebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer ? renderbuffer._ : 0);
}

var _framebufferTexture2D = gl.framebufferTexture2D;
gl.framebufferTexture2D = function framebufferTexture2D(target, attachment, textarget, texture, level) {
  if (!(arguments.length === 5 && typeof target === "number" && typeof attachment === "number" && typeof textarget === "number" && (texture === null || texture instanceof gl.WebGLTexture) && typeof level === "number")) {
    throw new TypeError('Expected framebufferTexture2D(number target, number attachment, number textarget, WebGLTexture texture, number level)');
  }
  return _framebufferTexture2D(target, attachment, textarget, texture ? texture._ : 0, level);
}

var _frontFace = gl.frontFace;
gl.frontFace = function frontFace(mode) {
  if (!(arguments.length === 1 && typeof mode === "number")) {
    throw new TypeError('Expected frontFace(number mode)');
  }
  return _frontFace(mode);
}

var _generateMipmap = gl.generateMipmap;
gl.generateMipmap = function generateMipmap(target) {
  if (!(arguments.length === 1 && typeof target === "number")) {
    throw new TypeError('Expected generateMipmap(number target)');
  }
  return _generateMipmap(target);
}

var _getActiveAttrib = gl.getActiveAttrib;
gl.getActiveAttrib = function getActiveAttrib(program, index) {
  if (!(arguments.length === 2 && (program === null || program instanceof gl.WebGLProgram) && typeof index === "number")) {
    throw new TypeError('Expected getActiveAttrib(WebGLProgram program, number index)');
  }
  return new gl.WebGLActiveInfo(_getActiveAttrib(program ? program._ : 0, index));
}

var _getActiveUniform = gl.getActiveUniform;
gl.getActiveUniform = function getActiveUniform(program, index) {
  if (!(arguments.length === 2 && (program === null || program instanceof gl.WebGLProgram) && typeof index === "number")) {
    throw new TypeError('Expected getActiveUniform(WebGLProgram program, number index)');
  }
  return new gl.WebGLActiveInfo(_getActiveUniform(program ? program._ : 0, index));
}

var _getAttachedShaders = gl.getAttachedShaders;
gl.getAttachedShaders = function getAttachedShaders(program) {
  if (!(arguments.length === 1 && (program === null || program instanceof gl.WebGLProgram))) {
    throw new TypeError('Expected getAttachedShaders(WebGLProgram program)');
  }
  return _getAttachedShaders(program ? program._ : 0);
}

var _getAttribLocation = gl.getAttribLocation;
gl.getAttribLocation = function getAttribLocation(program, name) {
  if (!(arguments.length === 2 && (program === null || program instanceof gl.WebGLProgram) && typeof name === "string")) {
    throw new TypeError('Expected getAttribLocation(WebGLProgram program, string name)');
  }
  return _getAttribLocation(program ? program._ : 0, name);
}

var _getParameter = gl.getParameter;
gl.getParameter = function getParameter(pname) {
  if (!(arguments.length === 1 && typeof pname === "number")) {
    throw new TypeError('Expected getParameter(number pname)');
  }
  return _getParameter(pname);
}

var _getBufferParameter = gl.getBufferParameter;
gl.getBufferParameter = function getBufferParameter(target, pname) {
  if (!(arguments.length === 2 && typeof target === "number" && typeof pname === "number")) {
    throw new TypeError('Expected getBufferParameter(number target, number pname)');
  }
  return _getBufferParameter(target, pname);
}

var _getError = gl.getError;
gl.getError = function getError() {
  if (!(arguments.length === 0)) {
    throw new TypeError('Expected getError()');
  }
  return _getError();
}

var _getFramebufferAttachmentParameter = gl.getFramebufferAttachmentParameter;
gl.getFramebufferAttachmentParameter = function getFramebufferAttachmentParameter(target, attachment, pname) {
  if (!(arguments.length === 3 && typeof target === "number" && typeof attachment === "number" && typeof pname === "number")) {
    throw new TypeError('Expected getFramebufferAttachmentParameter(number target, number attachment, number pname)');
  }
  return _getFramebufferAttachmentParameter(target, attachment, pname);
}

var _getProgramParameter = gl.getProgramParameter;
gl.getProgramParameter = function getProgramParameter(program, pname) {
  if (!(arguments.length === 2 && (program === null || program instanceof gl.WebGLProgram) && typeof pname === "number")) {
    throw new TypeError('Expected getProgramParameter(WebGLProgram program, number pname)');
  }
  return _getProgramParameter(program ? program._ : 0, pname);
}

var _getProgramInfoLog = gl.getProgramInfoLog;
gl.getProgramInfoLog = function getProgramInfoLog(program) {
  if (!(arguments.length === 1 && (program === null || program instanceof gl.WebGLProgram))) {
    throw new TypeError('Expected getProgramInfoLog(WebGLProgram program)');
  }
  return _getProgramInfoLog(program ? program._ : 0);
}

var _getRenderbufferParameter = gl.getRenderbufferParameter;
gl.getRenderbufferParameter = function getRenderbufferParameter(target, pname) {
  if (!(arguments.length === 2 && typeof target === "number" && typeof pname === "number")) {
    throw new TypeError('Expected getRenderbufferParameter(number target, number pname)');
  }
  return _getRenderbufferParameter(target, pname);
}

var _getShaderParameter = gl.getShaderParameter;
gl.getShaderParameter = function getShaderParameter(shader, pname) {
  if (!(arguments.length === 2 && (shader === null || shader instanceof gl.WebGLShader) && typeof pname === "number")) {
    throw new TypeError('Expected getShaderParameter(WebGLShader shader, number pname)');
  }
  return _getShaderParameter(shader ? shader._ : 0, pname);
}

var _getShaderInfoLog = gl.getShaderInfoLog;
gl.getShaderInfoLog = function getShaderInfoLog(shader) {
  if (!(arguments.length === 1 && (shader === null || shader instanceof gl.WebGLShader))) {
    throw new TypeError('Expected getShaderInfoLog(WebGLShader shader)');
  }
  return _getShaderInfoLog(shader ? shader._ : 0);
}

var _getShaderSource = gl.getShaderSource;
gl.getShaderSource = function getShaderSource(shader) {
  if (!(arguments.length === 1 && (shader === null || shader instanceof gl.WebGLShader))) {
    throw new TypeError('Expected getShaderSource(WebGLShader shader)');
  }
  return _getShaderSource(shader ? shader._ : 0);
}

var _getTexParameter = gl.getTexParameter;
gl.getTexParameter = function getTexParameter(target, pname) {
  if (!(arguments.length === 2 && typeof target === "number" && typeof pname === "number")) {
    throw new TypeError('Expected getTexParameter(number target, number pname)');
  }
  return _getTexParameter(target, pname);
}

var _getUniform = gl.getUniform;
gl.getUniform = function getUniform(program, location) {
  if (!(arguments.length === 2 && (program === null || program instanceof gl.WebGLProgram) && (location === null || location instanceof gl.WebGLUniformLocation))) {
    throw new TypeError('Expected getUniform(WebGLProgram program, WebGLUniformLocation location)');
  }
  return _getUniform(program ? program._ : 0, location ? location._ : -1);
}

var _getUniformLocation = gl.getUniformLocation;
gl.getUniformLocation = function getUniformLocation(program, name) {
  if (!(arguments.length === 2 && (program === null || program instanceof gl.WebGLProgram) && typeof name === "string")) {
    throw new TypeError('Expected getUniformLocation(WebGLProgram program, string name)');
  }
  var loc = _getUniformLocation(program ? program._ : 0, name);
  return loc === -1? null : new gl.WebGLUniformLocation(loc);
}

var _getVertexAttrib = gl.getVertexAttrib;
gl.getVertexAttrib = function getVertexAttrib(index, pname) {
  if (!(arguments.length === 2 && typeof index === "number" && typeof pname === "number")) {
    throw new TypeError('Expected getVertexAttrib(number index, number pname)');
  }
  return _getVertexAttrib(index, pname);
}

var _getVertexAttribOffset = gl.getVertexAttribOffset;
gl.getVertexAttribOffset = function getVertexAttribOffset(index, pname) {
  if (!(arguments.length === 2 && typeof index === "number" && typeof pname === "number")) {
    throw new TypeError('Expected getVertexAttribOffset(number index, number pname)');
  }
  
  if(pname === gl.CURRENT_VERTEX_ATTRIB) {
    var buf=_getVertexAttribOffset(index, pname);
    return new Float32Array(buf);
  }
  return _getVertexAttribOffset(index, pname);
}

var _hint = gl.hint;
gl.hint = function hint(target, mode) {
  if (!(arguments.length === 2 && typeof target === "number" && typeof mode === "number")) {
    throw new TypeError('Expected hint(number target, number mode)');
  }
  return _hint(target, mode);
}

var _isBuffer = gl.isBuffer;
gl.isBuffer = function isBuffer(buffer) {
  if (!(arguments.length === 1 && (buffer === null || buffer instanceof gl.WebGLBuffer))) {
    throw new TypeError('Expected isBuffer(WebGLBuffer buffer)');
  }
  return _isBuffer(buffer ? buffer._ : 0);
}

var _isEnabled = gl.isEnabled;
gl.isEnabled = function isEnabled(cap) {
  if (!(arguments.length === 1 && typeof cap === "number")) {
    throw new TypeError('Expected isEnabled(number cap)');
  }
  return _isEnabled(cap);
}

var _isFramebuffer = gl.isFramebuffer;
gl.isFramebuffer = function isFramebuffer(framebuffer) {
  if (!(arguments.length === 1 && (framebuffer === null || framebuffer instanceof gl.WebGLFramebuffer))) {
    throw new TypeError('Expected isFramebuffer(WebGLFramebuffer framebuffer)');
  }
  return _isFramebuffer(framebuffer ? framebuffer._ : 0);
}

var _isProgram = gl.isProgram;
gl.isProgram = function isProgram(program) {
  if (!(arguments.length === 1 && (program === null || program instanceof gl.WebGLProgram))) {
    throw new TypeError('Expected isProgram(WebGLProgram program)');
  }
  return _isProgram(program ? program._ : 0);
}

var _isRenderbuffer = gl.isRenderbuffer;
gl.isRenderbuffer = function isRenderbuffer(renderbuffer) {
  if (!(arguments.length === 1 && (renderbuffer === null || renderbuffer instanceof gl.WebGLRenderbuffer))) {
    throw new TypeError('Expected isRenderbuffer(WebGLRenderbuffer renderbuffer)');
  }
  return _isRenderbuffer(renderbuffer ? renderbuffer._ : 0);
}

var _isShader = gl.isShader;
gl.isShader = function isShader(shader) {
  if (!(arguments.length === 1 && (shader === null || shader instanceof gl.WebGLShader))) {
    throw new TypeError('Expected isShader(WebGLShader shader)');
  }
  return _isShader(shader ? shader._ : 0);
}

var _isTexture = gl.isTexture;
gl.isTexture = function isTexture(texture) {
  if (!(arguments.length === 1 && (texture === null || texture instanceof gl.WebGLTexture))) {
    throw new TypeError('Expected isTexture(WebGLTexture texture)');
  }
  return _isTexture(texture ? texture._ : 0);
}

var _lineWidth = gl.lineWidth;
gl.lineWidth = function lineWidth(width) {
  if (!(arguments.length === 1 && typeof width === "number")) {
    throw new TypeError('Expected lineWidth(number width)');
  }
  return _lineWidth(width);
}

var _linkProgram = gl.linkProgram;
gl.linkProgram = function linkProgram(program) {
  if (!(arguments.length === 1 && (program === null || program instanceof gl.WebGLProgram))) {
    throw new TypeError('Expected linkProgram(WebGLProgram program)');
  }
  return _linkProgram(program ? program._ : 0);
}

var _pixelStorei = gl.pixelStorei;
gl.pixelStorei = function pixelStorei(pname, param) {
  if (!(arguments.length === 2 && typeof pname === "number" && (typeof param === "number") || typeof param === "boolean")) {
    throw new TypeError('Expected pixelStorei(number pname, number param)');
  }
  
  if(typeof param === "boolean")
    param= param ? 1 : 0;
  return _pixelStorei(pname, param);
}

var _polygonOffset = gl.polygonOffset;
gl.polygonOffset = function polygonOffset(factor, units) {
  if (!(arguments.length === 2 && typeof factor === "number" && typeof units === "number")) {
    throw new TypeError('Expected polygonOffset(number factor, number units)');
  }
  return _polygonOffset(factor, units);
}

var _readPixels = gl.readPixels;
gl.readPixels = function readPixels(x, y, width, height, format, type, pixels) {
  if (!(arguments.length === 7 && typeof x === "number" && typeof y === "number" && typeof width === "number" && typeof height === "number" && typeof format === "number" && typeof type === "number" && typeof pixels === "object")) {
    throw new TypeError('Expected readPixels(number x, number y, number width, number height, number format, number type, ArrayBufferView pixels)');
  }
  return _readPixels(x, y, width, height, format, type, pixels);
}

var _renderbufferStorage = gl.renderbufferStorage;
gl.renderbufferStorage = function renderbufferStorage(target, internalformat, width, height) {
  if (!(arguments.length === 4 && typeof target === "number" && typeof internalformat === "number" && typeof width === "number" && typeof height === "number")) {
    throw new TypeError('Expected renderbufferStorage(number target, number internalformat, number width, number height)');
  }
  return _renderbufferStorage(target, internalformat, width, height);
}

var _sampleCoverage = gl.sampleCoverage;
gl.sampleCoverage = function sampleCoverage(value, invert) {
  if (!(arguments.length === 2 && typeof value === "number" && typeof invert === "boolean")) {
    throw new TypeError('Expected sampleCoverage(number value, boolean invert)');
  }
  return _sampleCoverage(value, invert);
}

var _scissor = gl.scissor;
gl.scissor = function scissor(x, y, width, height) {
  if (!(arguments.length === 4 && typeof x === "number" && typeof y === "number" && typeof width === "number" && typeof height === "number")) {
    throw new TypeError('Expected scissor(number x, number y, number width, number height)');
  }
  return _scissor(x, y, width, height);
}

var _shaderSource = gl.shaderSource;
gl.shaderSource = function shaderSource(shader, source) {
  if (!(arguments.length === 2 && (shader === null || shader instanceof gl.WebGLShader) && typeof source === "string")) {
    throw new TypeError('Expected shaderSource(WebGLShader shader, string source)');
  }
  return _shaderSource(shader ? shader._ : 0, source);
}

var _stencilFunc = gl.stencilFunc;
gl.stencilFunc = function stencilFunc(func, ref, mask) {
  if (!(arguments.length === 3 && typeof func === "number" && typeof ref === "number" && typeof mask === "number")) {
    throw new TypeError('Expected stencilFunc(number func, number ref, number mask)');
  }
  return _stencilFunc(func, ref, mask);
}

var _stencilFuncSeparate = gl.stencilFuncSeparate;
gl.stencilFuncSeparate = function stencilFuncSeparate(face, func, ref, mask) {
  if (!(arguments.length === 4 && typeof face === "number" && typeof func === "number" && typeof ref === "number" && typeof mask === "number")) {
    throw new TypeError('Expected stencilFuncSeparate(number face, number func, number ref, number mask)');
  }
  return _stencilFuncSeparate(face, func, ref, mask);
}

var _stencilMask = gl.stencilMask;
gl.stencilMask = function stencilMask(mask) {
  if (!(arguments.length === 1 && typeof mask === "number")) {
    throw new TypeError('Expected stencilMask(number mask)');
  }
  return _stencilMask(mask);
}

var _stencilMaskSeparate = gl.stencilMaskSeparate;
gl.stencilMaskSeparate = function stencilMaskSeparate(face, mask) {
  if (!(arguments.length === 2 && typeof face === "number" && typeof mask === "number")) {
    throw new TypeError('Expected stencilMaskSeparate(number face, number mask)');
  }
  return _stencilMaskSeparate(face, mask);
}

var _stencilOp = gl.stencilOp;
gl.stencilOp = function stencilOp(fail, zfail, zpass) {
  if (!(arguments.length === 3 && typeof fail === "number" && typeof zfail === "number" && typeof zpass === "number")) {
    throw new TypeError('Expected stencilOp(number fail, number zfail, number zpass)');
  }
  return _stencilOp(fail, zfail, zpass);
}

var _stencilOpSeparate = gl.stencilOpSeparate;
gl.stencilOpSeparate = function stencilOpSeparate(face, fail, zfail, zpass) {
  if (!(arguments.length === 4 && typeof face === "number" && typeof fail === "number" && typeof zfail === "number" && typeof zpass === "number")) {
    throw new TypeError('Expected stencilOpSeparate(number face, number fail, number zfail, number zpass)');
  }
  return _stencilOpSeparate(face, fail, zfail, zpass);
}

var _texImage2D = gl.texImage2D;
gl.texImage2D = function texImage2D(target, level, internalformat, width, height, border, format, type, pixels) {
  if(!(arguments.length == 9 || arguments.length == 6)) {
    throw new TypeError('Expected texImage2D(number target, number level, number internalformat, number width, number height, number border, number format, number type, ArrayBufferView pixels) \
        or texImage2D(number target, number level, number internalformat, number format, number type, Image pixels)');
  }

  if (arguments.length == 6) {
    // width is now format, height is now type, and border is now pixels
    if(!(
        typeof target === "number" && 
        typeof level === "number" && typeof internalformat === "number" && 
        typeof width === "number" && typeof height === "number" && 
        (border==null || border instanceof Image))) {
      throw new TypeError('Expected texImage2D(number target, number level, number internalformat, number format, number type, Image pixels)');
    }
    pixels=border;
    type=height;
    format=width;
    return _texImage2D(target, level, internalformat, pixels.width, pixels.height, 0, format, type, pixels);
  }
  else if (arguments.length == 9) {
    if(!(typeof target === "number" && 
        typeof level === "number" && typeof internalformat === "number" && 
        typeof width === "number" && typeof height === "number" && typeof border === "number" && 
        typeof format === "number" && typeof type === "number" && 
        (pixels==null || typeof pixels === "object"))) {
      throw new TypeError('Expected texImage2D(number target, number level, number internalformat, number width, number height, number border, number format, number type, ArrayBufferView pixels)');
    }
    return _texImage2D(target, level, internalformat, width, height, border, format, type, pixels);
  }
}

var _texParameterf = gl.texParameterf;
gl.texParameterf = function texParameterf(target, pname, param) {
  if (!(arguments.length === 3 && typeof target === "number" && typeof pname === "number" && typeof param === "number")) {
    throw new TypeError('Expected texParameterf(number target, number pname, number param)');
  }
  return _texParameterf(target, pname, param);
}

var _texParameteri = gl.texParameteri;
gl.texParameteri = function texParameteri(target, pname, param) {
  if (!(arguments.length === 3 && typeof target === "number" && typeof pname === "number" && typeof param === "number")) {
    throw new TypeError('Expected texParameteri(number target, number pname, number param)');
  }
  return _texParameteri(target, pname, param);
}

var _texSubImage2D = gl.texSubImage2D;
gl.texSubImage2D = function texSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels) {
  if (!(arguments.length === 9 && typeof target === "number" && typeof level === "number" && 
      typeof xoffset === "number" && typeof yoffset === "number" && 
      typeof width === "number" && typeof height === "number" && 
      typeof format === "number" && typeof type === "number" && 
      (pixels==null || typeof pixels === "object"))) {
    throw new TypeError('Expected texSubImage2D(number target, number level, number xoffset, number yoffset, number width, number height, number format, number type, ArrayBufferView pixels)');
  }
  return _texSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
}

var _uniform1f = gl.uniform1f;
gl.uniform1f = function uniform1f(location, x) {
  if (!(arguments.length === 2 && (location === null || location instanceof gl.WebGLUniformLocation) && (typeof x === "number" || typeof x === "boolean"))) {
    throw new TypeError('Expected uniform1f(WebGLUniformLocation location, number x)');
  }
  return _uniform1f(location ? location._ : -1, x);
}

var _uniform1fv = gl.uniform1fv;
gl.uniform1fv = function uniform1fv(location, v) {
  if (!(arguments.length === 2 && (location === null || location instanceof gl.WebGLUniformLocation) && typeof v === "object")) {
    throw new TypeError('Expected uniform1fv(WebGLUniformLocation location, FloatArray v)');
  }
  return _uniform1fv(location ? location._ : -1, v);
}

var _uniform1i = gl.uniform1i;
gl.uniform1i = function uniform1i(location, x) {
  if (!(arguments.length === 2 && (location === null || location instanceof gl.WebGLUniformLocation) && (typeof x === "number" || typeof x ==="boolean"))) {
    throw new TypeError('Expected uniform1i(WebGLUniformLocation location, number x)');
  }
  if(typeof x === "boolean")
    x= x ? 1 : 0;
  return _uniform1i(location ? location._ : -1, x);
}

var _uniform1iv = gl.uniform1iv;
gl.uniform1iv = function uniform1iv(location, v) {
  if (!(arguments.length === 2 && (location === null || location instanceof gl.WebGLUniformLocation) && typeof v === "object")) {
    throw new TypeError('Expected uniform1iv(WebGLUniformLocation location, Int32Array v)');
  }
  return _uniform1iv(location ? location._ : -1, v);
}

var _uniform2f = gl.uniform2f;
gl.uniform2f = function uniform2f(location, x, y) {
  if (!(arguments.length === 3 && (location === null || location instanceof gl.WebGLUniformLocation) && typeof x === "number" && typeof y === "number")) {
    throw new TypeError('Expected uniform2f(WebGLUniformLocation location, number x, number y)');
  }
  return _uniform2f(location ? location._ : -1, x, y);
}

var _uniform2fv = gl.uniform2fv;
gl.uniform2fv = function uniform2fv(location, v) {
  if (!(arguments.length === 2 && (location === null || location instanceof gl.WebGLUniformLocation) && typeof v === "object")) {
    throw new TypeError('Expected uniform2fv(WebGLUniformLocation location, FloatArray v)');
  }
  return _uniform2fv(location ? location._ : -1, v);
}

var _uniform2i = gl.uniform2i;
gl.uniform2i = function uniform2i(location, x, y) {
  if (!(arguments.length === 3 && (location === null || location instanceof gl.WebGLUniformLocation) && typeof x === "number" && typeof y === "number")) {
    throw new TypeError('Expected uniform2i(WebGLUniformLocation location, number x, number y)');
  }
  return _uniform2i(location ? location._ : -1, x, y);
}

var _uniform2iv = gl.uniform2iv;
gl.uniform2iv = function uniform2iv(location, v) {
  if (!(arguments.length === 2 && (location === null || location instanceof gl.WebGLUniformLocation) && typeof v === "object")) {
    throw new TypeError('Expected uniform2iv(WebGLUniformLocation location, Int32Array v)');
  }
  return _uniform2iv(location ? location._ : -1, v);
}

var _uniform3f = gl.uniform3f;
gl.uniform3f = function uniform3f(location, x, y, z) {
  if (!(arguments.length === 4 && (location === null || location instanceof gl.WebGLUniformLocation) && typeof x === "number" && typeof y === "number" && typeof z === "number")) {
    throw new TypeError('Expected uniform3f(WebGLUniformLocation location, number x, number y, number z)');
  }
  return _uniform3f(location ? location._ : -1, x, y, z);
}

var _uniform3fv = gl.uniform3fv;
gl.uniform3fv = function uniform3fv(location, v) {
  if (!(arguments.length === 2 && (location === null || location instanceof gl.WebGLUniformLocation) && typeof v === "object")) {
    throw new TypeError('Expected uniform3fv(WebGLUniformLocation location, FloatArray v)');
  }
  return _uniform3fv(location ? location._ : -1, v);
}

var _uniform3i = gl.uniform3i;
gl.uniform3i = function uniform3i(location, x, y, z) {
  if (!(arguments.length === 4 && (location === null || location instanceof gl.WebGLUniformLocation) && typeof x === "number" && typeof y === "number" && typeof z === "number")) {
    throw new TypeError('Expected uniform3i(WebGLUniformLocation location, number x, number y, number z)');
  }
  return _uniform3i(location ? location._ : -1, x, y, z);
}

var _uniform3iv = gl.uniform3iv;
gl.uniform3iv = function uniform3iv(location, x) {
  if (!(arguments.length === 2 && (location === null || location instanceof gl.WebGLUniformLocation) && typeof x === "object")) {
    throw new TypeError('Expected uniform3iv(WebGLUniformLocation location, Int32Array x)');
  }
  return _uniform3iv(location ? location._ : -1, x);
}

var _uniform4f = gl.uniform4f;
gl.uniform4f = function uniform4f(location, x, y, z, w) {
  if (!(arguments.length === 5 && (location === null || location instanceof gl.WebGLUniformLocation) && typeof x === "number" && typeof y === "number" && typeof z === "number" && typeof w === "number")) {
    throw new TypeError('Expected uniform4f(WebGLUniformLocation location, number x, number y, number z, number w)');
  }
  return _uniform4f(location ? location._ : -1, x, y, z, w);
}

var _uniform4fv = gl.uniform4fv;
gl.uniform4fv = function uniform4fv(location, v) {
  if (!(arguments.length === 2 && (location === null || location instanceof gl.WebGLUniformLocation) && typeof v === "object")) {
    throw new TypeError('Expected uniform4fv(WebGLUniformLocation location, FloatArray v)');
  }
  return _uniform4fv(location ? location._ : -1, v);
}

var _uniform4i = gl.uniform4i;
gl.uniform4i = function uniform4i(location, x, y, z, w) {
  if (!(arguments.length === 5 && (location === null || location instanceof gl.WebGLUniformLocation) && typeof x === "number" && typeof y === "number" && typeof z === "number" && typeof w === "number")) {
    throw new TypeError('Expected uniform4i(WebGLUniformLocation location, number x, number y, number z, number w)');
  }
  return _uniform4i(location ? location._ : -1, x, y, z, w);
}

var _uniform4iv = gl.uniform4iv;
gl.uniform4iv = function uniform4iv(location, x) {
  if (!(arguments.length === 2 && (location === null || location instanceof gl.WebGLUniformLocation) && typeof x === "object")) {
    throw new TypeError('Expected uniform4iv(WebGLUniformLocation location, Int32Array x)');
  }
  return _uniform4iv(location ? location._ : -1, x);
}

var _uniformMatrix2fv = gl.uniformMatrix2fv;
gl.uniformMatrix2fv = function uniformMatrix2fv(location, transpose, value) {
  if (!(arguments.length === 3 && (location === null || location instanceof gl.WebGLUniformLocation) && typeof transpose === "boolean" && typeof value === "object")) {
    throw new TypeError('Expected uniformMatrix2fv(WebGLUniformLocation location, boolean transpose, FloatArray value)');
  }
  return _uniformMatrix2fv(location ? location._ : -1, transpose, value);
}

var _uniformMatrix3fv = gl.uniformMatrix3fv;
gl.uniformMatrix3fv = function uniformMatrix3fv(location, transpose, value) {
  if (!(arguments.length === 3 && (location === null || location instanceof gl.WebGLUniformLocation) && typeof transpose === "boolean" && typeof value === "object")) {
    throw new TypeError('Expected uniformMatrix3fv(WebGLUniformLocation location, boolean transpose, FloatArray value)');
  }
  return _uniformMatrix3fv(location ? location._ : -1, transpose, value);
}

var _uniformMatrix4fv = gl.uniformMatrix4fv;
gl.uniformMatrix4fv = function uniformMatrix4fv(location, transpose, value) {
  if (!(arguments.length === 3 && (location === null || location instanceof gl.WebGLUniformLocation) && typeof transpose === "boolean" && typeof value === "object")) {
    throw new TypeError('Expected uniformMatrix4fv(WebGLUniformLocation location, boolean transpose, FloatArray value)');
  }
  return _uniformMatrix4fv(location ? location._ : -1, transpose, value);
}

var _useProgram = gl.useProgram;
gl.useProgram = function useProgram(program) {
  if (!(arguments.length === 1 && (program === null || program instanceof gl.WebGLProgram))) {
    throw new TypeError('Expected useProgram(WebGLProgram program)');
  }
  return _useProgram(program ? program._ : 0);
}

var _validateProgram = gl.validateProgram;
gl.validateProgram = function validateProgram(program) {
  if (!(arguments.length === 1 && (program === null || program instanceof gl.WebGLProgram))) {
    throw new TypeError('Expected validateProgram(WebGLProgram program)');
  }
  return _validateProgram(program ? program._ : 0);
}

var _vertexAttrib1f = gl.vertexAttrib1f;
gl.vertexAttrib1f = function vertexAttrib1f(indx, x) {
  if (!(arguments.length === 2 && typeof indx === "number" && typeof x === "number")) {
    throw new TypeError('Expected vertexAttrib1f(number indx, number x)');
  }
  return _vertexAttrib1f(indx, x);
}

var _vertexAttrib1fv = gl.vertexAttrib1fv;
gl.vertexAttrib1fv = function vertexAttrib1fv(indx, values) {
  if (!(arguments.length === 2 && typeof indx === "number" && typeof values === "object")) {
    throw new TypeError('Expected vertexAttrib1fv(number indx, FloatArray values)');
  }
  return _vertexAttrib1fv(indx, values);
}

var _vertexAttrib2f = gl.vertexAttrib2f;
gl.vertexAttrib2f = function vertexAttrib2f(indx, x, y) {
  if (!(arguments.length === 3 && typeof indx === "number" && typeof x === "number" && typeof y === "number")) {
    throw new TypeError('Expected vertexAttrib2f(number indx, number x, number y)');
  }
  return _vertexAttrib2f(indx, x, y);
}

var _vertexAttrib2fv = gl.vertexAttrib2fv;
gl.vertexAttrib2fv = function vertexAttrib2fv(indx, values) {
  if (!(arguments.length === 2 && typeof indx === "number" && typeof values === "object")) {
    throw new TypeError('Expected vertexAttrib2fv(number indx, FloatArray values)');
  }
  return _vertexAttrib2fv(indx, values);
}

var _vertexAttrib3f = gl.vertexAttrib3f;
gl.vertexAttrib3f = function vertexAttrib3f(indx, x, y, z) {
  if (!(arguments.length === 4 && typeof indx === "number" && typeof x === "number" && typeof y === "number" && typeof z === "number")) {
    throw new TypeError('Expected vertexAttrib3f(number indx, number x, number y, number z)');
  }
  return _vertexAttrib3f(indx, x, y, z);
}

var _vertexAttrib3fv = gl.vertexAttrib3fv;
gl.vertexAttrib3fv = function vertexAttrib3fv(indx, values) {
  if (!(arguments.length === 2 && typeof indx === "number" && typeof values === "object")) {
    throw new TypeError('Expected vertexAttrib3fv(number indx, FloatArray values)');
  }
  return _vertexAttrib3fv(indx, values);
}

var _vertexAttrib4f = gl.vertexAttrib4f;
gl.vertexAttrib4f = function vertexAttrib4f(indx, x, y, z, w) {
  if (!(arguments.length === 5 && typeof indx === "number" && typeof x === "number" && typeof y === "number" && typeof z === "number" && typeof w === "number")) {
    throw new TypeError('Expected vertexAttrib4f(number indx, number x, number y, number z, number w)');
  }
  return _vertexAttrib4f(indx, x, y, z, w);
}

var _vertexAttrib4fv = gl.vertexAttrib4fv;
gl.vertexAttrib4fv = function vertexAttrib4fv(indx, values) {
  if (!(arguments.length === 2 && typeof indx === "number" && typeof values === "object")) {
    throw new TypeError('Expected vertexAttrib4fv(number indx, FloatArray values)');
  }
  return _vertexAttrib4fv(indx, values);
}

var _vertexAttribPointer = gl.vertexAttribPointer;
gl.vertexAttribPointer = function vertexAttribPointer(indx, size, type, normalized, stride, offset) {
  if (!(arguments.length === 6 && typeof indx === "number" && typeof size === "number" && typeof type === "number" && (typeof normalized === "boolean" || typeof normalized === "number") && typeof stride === "number" && typeof offset === "number")) {
    throw new TypeError('Expected vertexAttribPointer(number indx, number size, number type, boolean normalized, number stride, number offset)');
  }
  return _vertexAttribPointer(indx, size, type, normalized, stride, offset);
}

var _viewport = gl.viewport;
gl.viewport = function viewport(x, y, width, height) {
  if (!(arguments.length === 4 && typeof x === "number" && typeof y === "number" && typeof width === "number" && typeof height === "number")) {
    throw new TypeError('Expected viewport(number x, number y, number width, number height)');
  }
  return _viewport(x, y, width, height);
}

