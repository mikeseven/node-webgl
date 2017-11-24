nodejs=true;

//Read and eval library
fs=require('fs');
mat4=require('./gl-Matrix-2.4.0.min.js').mat4;

var WebGL=require('../index'),
    Image = WebGL.Image,
    document = WebGL.document(),
    alert=console.error;
;

document.setTitle("Lesson05");
document.on("resize",function(evt){
  //console.log('resize to: ('+evt.width+", "+evt.height+")");
  gl.viewportWidth=evt.width;
  gl.viewportHeight=evt.height;
});

requestAnimationFrame = document.requestAnimationFrame;

var shaders= {
    "shader-fs" :
      [
       "#ifdef GL_ES",
       "  precision mediump float;",
       "#endif",
       "varying vec2 vTextureCoord;",
       "uniform sampler2D uSampler;",
       "void main(void) {",
       "    gl_FragColor = texture2D(uSampler, vec2(vTextureCoord.s, vTextureCoord.t));",
       "}"
       ].join("\n"),

       "shader-vs" :
         [
          "attribute vec3 aVertexPosition;",
          "attribute vec2 aTextureCoord;",
          "uniform mat4 uMVMatrix;",
          "uniform mat4 uPMatrix;",
          "varying vec2 vTextureCoord;",
          "void main(void) {",
          "    gl_Position = uPMatrix * uMVMatrix * vec4(aVertexPosition, 1.0);",
          "    vTextureCoord = aTextureCoord;",
          "}"
          ].join("\n")
};

var gl;

function initGL(canvas) {
  try {
    gl = canvas.getContext("experimental-webgl");
    gl.viewportWidth = canvas.width;
    gl.viewportHeight = canvas.height;
  } catch (e) {
  }
  if (!gl) {
    alert("Could not initialise WebGL, sorry :-(");
  }
}


function getShader(gl, id) {
  var shader;
  if(nodejs) {
    if (!shaders.hasOwnProperty(id)) return null;
    var str = shaders[id];

    if (id.match(/-fs/)) {
      shader = gl.createShader(gl.FRAGMENT_SHADER);
    } else if (id.match(/-vs/)) {
      shader = gl.createShader(gl.VERTEX_SHADER);
    } else { return null; }

  }
  else {
    var shaderScript = document.getElementById(id);
    if (!shaderScript) {
      return null;
    }

    var str = "";
    var k = shaderScript.firstChild;
    while (k) {
      if (k.nodeType == 3) {
        str += k.textContent;
      }
      k = k.nextSibling;
    }
    if (shaderScript.type == "x-shader/x-fragment") {
      shader = gl.createShader(gl.FRAGMENT_SHADER);
    } else if (shaderScript.type == "x-shader/x-vertex") {
      shader = gl.createShader(gl.VERTEX_SHADER);
    } else {
      return null;
    }
  }

  gl.shaderSource(shader, str);
  gl.compileShader(shader);

  if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
    alert(gl.getShaderInfoLog(shader));
    return null;
  }

  return shader;
}

var shaderProgram;

function initShaders() {
  var fragmentShader = getShader(gl, "shader-fs");
  var vertexShader = getShader(gl, "shader-vs");

  shaderProgram = gl.createProgram();
  gl.attachShader(shaderProgram, vertexShader);
  gl.attachShader(shaderProgram, fragmentShader);
  gl.linkProgram(shaderProgram);

  if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
    alert("Could not initialise shaders");
  }

  gl.useProgram(shaderProgram);

  shaderProgram.vertexPositionAttribute = gl.getAttribLocation(shaderProgram, "aVertexPosition");
  gl.enableVertexAttribArray(shaderProgram.vertexPositionAttribute);

  shaderProgram.textureCoordAttribute = gl.getAttribLocation(shaderProgram, "aTextureCoord");
  gl.enableVertexAttribArray(shaderProgram.textureCoordAttribute);

  shaderProgram.pMatrixUniform = gl.getUniformLocation(shaderProgram, "uPMatrix");
  shaderProgram.mvMatrixUniform = gl.getUniformLocation(shaderProgram, "uMVMatrix");
  shaderProgram.samplerUniform = gl.getUniformLocation(shaderProgram, "uSampler");
}


function handleLoadedTexture(texture) {
  gl.bindTexture(gl.TEXTURE_2D, texture);
  gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
  gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, texture.image);
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
  gl.bindTexture(gl.TEXTURE_2D, null);
}


var neheTexture;

function initTexture() {
  neheTexture = gl.createTexture();
  neheTexture.image = new Image();
  neheTexture.image.onload = function () { // [MBS] was onload() event
    console.log("Loaded image: "+neheTexture.image.src);
    console.log("size: "+neheTexture.image.width+"x"+neheTexture.image.height);
    handleLoadedTexture(neheTexture)
  };

  neheTexture.image.src = nodejs ? __dirname+"/nehe.gif" : "nehe.gif";
}


var mvMatrix = mat4.create();
var mvMatrixStack = [];
var pMatrix = mat4.create();

function mvPushMatrix() {
  var copy = mat4.create();
  mat4.set(mvMatrix, copy);
  mvMatrixStack.push(copy);
}

function mvPopMatrix() {
  if (mvMatrixStack.length == 0) {
    throw "Invalid popMatrix!";
  }
  mvMatrix = mvMatrixStack.pop();
}


function setMatrixUniforms() {
  gl.uniformMatrix4fv(shaderProgram.pMatrixUniform, false, pMatrix);
  gl.uniformMatrix4fv(shaderProgram.mvMatrixUniform, false, mvMatrix);
}


function degToRad(degrees) {
  return degrees * Math.PI / 180;
}

var cubeVertexPositionBuffer;
var cubeVertexTextureCoordBuffer;
var cubeVertexIndexBuffer;

function initBuffers() {
  cubeVertexPositionBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, cubeVertexPositionBuffer);
  vertices = [
              // Front face
              -1.0, -1.0,  1.0,
              1.0, -1.0,  1.0,
              1.0,  1.0,  1.0,
              -1.0,  1.0,  1.0,

              // Back face
              -1.0, -1.0, -1.0,
              -1.0,  1.0, -1.0,
              1.0,  1.0, -1.0,
              1.0, -1.0, -1.0,

              // Top face
              -1.0,  1.0, -1.0,
              -1.0,  1.0,  1.0,
              1.0,  1.0,  1.0,
              1.0,  1.0, -1.0,

              // Bottom face
              -1.0, -1.0, -1.0,
              1.0, -1.0, -1.0,
              1.0, -1.0,  1.0,
              -1.0, -1.0,  1.0,

              // Right face
              1.0, -1.0, -1.0,
              1.0,  1.0, -1.0,
              1.0,  1.0,  1.0,
              1.0, -1.0,  1.0,

              // Left face
              -1.0, -1.0, -1.0,
              -1.0, -1.0,  1.0,
              -1.0,  1.0,  1.0,
              -1.0,  1.0, -1.0,
              ];
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
  cubeVertexPositionBuffer.itemSize = 3;
  cubeVertexPositionBuffer.numItems = 24;

  cubeVertexTextureCoordBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, cubeVertexTextureCoordBuffer);
  var textureCoords = [
                       // Front face
                       0.0, 0.0,
                       1.0, 0.0,
                       1.0, 1.0,
                       0.0, 1.0,

                       // Back face
                       1.0, 0.0,
                       1.0, 1.0,
                       0.0, 1.0,
                       0.0, 0.0,

                       // Top face
                       0.0, 1.0,
                       0.0, 0.0,
                       1.0, 0.0,
                       1.0, 1.0,

                       // Bottom face
                       1.0, 1.0,
                       0.0, 1.0,
                       0.0, 0.0,
                       1.0, 0.0,

                       // Right face
                       1.0, 0.0,
                       1.0, 1.0,
                       0.0, 1.0,
                       0.0, 0.0,

                       // Left face
                       0.0, 0.0,
                       1.0, 0.0,
                       1.0, 1.0,
                       0.0, 1.0,
                       ];
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(textureCoords), gl.STATIC_DRAW);
  cubeVertexTextureCoordBuffer.itemSize = 2;
  cubeVertexTextureCoordBuffer.numItems = 24;

  cubeVertexIndexBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, cubeVertexIndexBuffer);
  var cubeVertexIndices = [
                           0, 1, 2,      0, 2, 3,    // Front face
                           4, 5, 6,      4, 6, 7,    // Back face
                           8, 9, 10,     8, 10, 11,  // Top face
                           12, 13, 14,   12, 14, 15, // Bottom face
                           16, 17, 18,   16, 18, 19, // Right face
                           20, 21, 22,   20, 22, 23  // Left face
                           ];
  gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(cubeVertexIndices), gl.STATIC_DRAW);
  cubeVertexIndexBuffer.itemSize = 1;
  cubeVertexIndexBuffer.numItems = 36;
}


var xRot = 0;
var yRot = 0;
var zRot = 0;

function drawScene() {
  gl.viewport(0, 0, gl.viewportWidth, gl.viewportHeight);
  gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

  mat4.perspective(pMatrix, 45, gl.viewportWidth / gl.viewportHeight, 0.1, 100.0);

  mat4.identity(mvMatrix);

  mat4.translate(mvMatrix, mvMatrix, [0.0, 0.0, -5.0]);

  mat4.rotate(mvMatrix, mvMatrix, degToRad(xRot), [1, 0, 0]);
  mat4.rotate(mvMatrix, mvMatrix, degToRad(yRot), [0, 1, 0]);
  mat4.rotate(mvMatrix, mvMatrix, degToRad(zRot), [0, 0, 1]);

  gl.bindBuffer(gl.ARRAY_BUFFER, cubeVertexPositionBuffer);
  gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, cubeVertexPositionBuffer.itemSize, gl.FLOAT, false, 0, 0);

  gl.bindBuffer(gl.ARRAY_BUFFER, cubeVertexTextureCoordBuffer);
  gl.vertexAttribPointer(shaderProgram.textureCoordAttribute, cubeVertexTextureCoordBuffer.itemSize, gl.FLOAT, false, 0, 0);

  gl.activeTexture(gl.TEXTURE0);
  gl.bindTexture(gl.TEXTURE_2D, neheTexture);
  gl.uniform1i(shaderProgram.samplerUniform, 0);

  gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, cubeVertexIndexBuffer);
  setMatrixUniforms();
  gl.drawElements(gl.TRIANGLES, cubeVertexIndexBuffer.numItems, gl.UNSIGNED_SHORT, 0);
}


var lastTime = 0;

function animate() {
  var timeNow = new Date().getTime();
  if (lastTime != 0) {
    var elapsed = timeNow - lastTime;

    xRot += (90 * elapsed) / 1000.0;
    yRot += (90 * elapsed) / 1000.0;
    zRot += (90 * elapsed) / 1000.0;
  }
  lastTime = timeNow;
}


function tick() {
  drawScene();
  animate();

  requestAnimationFrame(tick);
}


function webGLStart() {
  var canvas = document.createElement("lesson05-canvas");
  initGL(canvas);
  initShaders();
  initBuffers();
  initTexture();

  gl.clearColor(0.0, 0.0, 0.0, 1.0);
  gl.enable(gl.DEPTH_TEST);

  tick();
}

webGLStart();

