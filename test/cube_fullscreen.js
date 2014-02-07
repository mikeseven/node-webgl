nodejs=true;

//Read and eval library
fs=require('fs');
eval(fs.readFileSync(__dirname+ '/glMatrix-0.9.5.min.js','utf8'));

var WebGL=require('../index'),
    Image = WebGL.Image,
    document = WebGL.document(),
    ATB=document.AntTweakBar,
    log=console.log,
    alert=console.warn;

document.setTitle("cube with AntTweakBar");
requestAnimationFrame = document.requestAnimationFrame;

var twBar;
var xRot = 0;
var xSpeed = 5;
var yRot = 0;
var ySpeed = -5;
var z = -5.0;
var running = true;

document.on("resize",function(evt){
  document.createWindow(evt.width,evt.height);
  gl.viewportWidth=evt.width;
  gl.viewportHeight=evt.height;
  
  // make sure AntTweakBar is repositioned correctly and events correct
  ATB.WindowSize(evt.width,evt.height);
});


var shaders= {
    "shader-fs" : 
      [     
       "#ifdef GL_ES",
       "  precision mediump float;",
       "#endif",
       "varying vec4 vColor;",
       "void main(void) {",
       "    gl_FragColor = vColor;",
       "}"
       ].join("\n"),

       "shader-vs" : 
         [ 
          "attribute vec3 aVertexPosition;",
          "attribute vec4 aVertexColor;",
          "uniform mat4 uMVMatrix;",
          "uniform mat4 uPMatrix;",
          "varying vec4 vColor;",
          "void main(void) {",
          "    gl_Position = uPMatrix * uMVMatrix * vec4(aVertexPosition, 1.0);",
          "    vColor = aVertexColor;",
          "}"
          ].join("\n")
};

var gl;

function initGL(canvas) {
  log('init WebGL');
  
  try {
    gl = canvas.getContext("experimental-webgl");
    gl.viewportWidth = canvas.width;
    gl.viewportHeight = canvas.height;
  } catch (e) {
    alert("Could not initialise WebGL, sorry :-(");
    process.exit(-1);
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
  log('init GL shaders')
  var fragmentShader = getShader(gl, "shader-fs");
  var vertexShader = getShader(gl, "shader-vs");

  shaderProgram = gl.createProgram();
  gl.attachShader(shaderProgram, vertexShader);
  gl.attachShader(shaderProgram, fragmentShader);
  gl.linkProgram(shaderProgram);

  if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
    alert("Could not initialise shaders. Error: "+gl.getProgramInfoLog(shaderProgram));
  }

  gl.useProgram(shaderProgram);

  shaderProgram.vertexPositionAttribute = gl.getAttribLocation(shaderProgram, "aVertexPosition");
  gl.enableVertexAttribArray(shaderProgram.vertexPositionAttribute);

  shaderProgram.vertexColorAttribute = gl.getAttribLocation(shaderProgram, "aVertexColor");
  gl.enableVertexAttribArray(shaderProgram.vertexColorAttribute);

  shaderProgram.pMatrixUniform = gl.getUniformLocation(shaderProgram, "uPMatrix");
  shaderProgram.mvMatrixUniform = gl.getUniformLocation(shaderProgram, "uMVMatrix");
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

var currentlyPressedKeys = {};

document.on("keydown", function(evt){
//   console.log("[KEYDOWN]", evt);
  currentlyPressedKeys[evt.keyCode] = true;
  handleKeys();
});

document.on("keyup", function(evt){
  currentlyPressedKeys[evt.keyCode] = false;
});


function handleKeyDown(event) {
  currentlyPressedKeys[event.keyCode] = true;
}


function handleKeyUp(event) {
  currentlyPressedKeys[event.keyCode] = false;
}

function handleKeys() {
  if (currentlyPressedKeys[35]) {
    // ]
    z -= 0.05;
  }
  if (currentlyPressedKeys[51]) {
    // \
    z += 0.05;
  }
  if (currentlyPressedKeys[113]) {
    // Left cursor key
    ySpeed -= 1;
  }
  if (currentlyPressedKeys[114]) {
    // Right cursor key
    ySpeed += 1;
  }
  if (currentlyPressedKeys[111]) {
    // Up cursor key
    xSpeed -= 1;
  }
  if (currentlyPressedKeys[116]) {
    // Down cursor key
    xSpeed += 1;
  }
  if (currentlyPressedKeys[27]) {
    // Escape
    running = false;
  }
  //console.log("speed: "+xSpeed+" "+ySpeed+" "+z);
}


var cubeVertexPositionBuffer;
var cubeVertexNormalBuffer;
var cubeVerticesColorBuffer;
var cubeVertexIndexBuffer;
function initBuffers() {
  log('init GL buffers')
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
              -1.0,  1.0, -1.0
              ];
  cubeVertexPositionBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, cubeVertexPositionBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
  cubeVertexPositionBuffer.itemSize = 3;
  cubeVertexPositionBuffer.numItems = 24;

  var vertexNormals = [
                       // Front face
                       0.0,  0.0,  1.0,
                       0.0,  0.0,  1.0,
                       0.0,  0.0,  1.0,
                       0.0,  0.0,  1.0,

                       // Back face
                       0.0,  0.0, -1.0,
                       0.0,  0.0, -1.0,
                       0.0,  0.0, -1.0,
                       0.0,  0.0, -1.0,

                       // Top face
                       0.0,  1.0,  0.0,
                       0.0,  1.0,  0.0,
                       0.0,  1.0,  0.0,
                       0.0,  1.0,  0.0,

                       // Bottom face
                       0.0, -1.0,  0.0,
                       0.0, -1.0,  0.0,
                       0.0, -1.0,  0.0,
                       0.0, -1.0,  0.0,

                       // Right face
                       1.0,  0.0,  0.0,
                       1.0,  0.0,  0.0,
                       1.0,  0.0,  0.0,
                       1.0,  0.0,  0.0,

                       // Left face
                       -1.0,  0.0,  0.0,
                       -1.0,  0.0,  0.0,
                       -1.0,  0.0,  0.0,
                       -1.0,  0.0,  0.0
                       ];
  cubeVertexNormalBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, cubeVertexNormalBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertexNormals), gl.STATIC_DRAW);
  cubeVertexNormalBuffer.itemSize = 3;
  cubeVertexNormalBuffer.numItems = 24;

  var colors = [
  [ 1.0, 1.0, 1.0, 0.5 ], // Front face: white
  [ 1.0, 0.0, 0.0, 0.5 ], // Back face: red
  [ 0.0, 1.0, 0.0, 0.5 ], // Top face: green
  [ 0.0, 0.0, 1.0, 0.5 ], // Bottom face: blue
  [ 1.0, 1.0, 0.0, 0.5 ], // Right face: yellow
  [ 1.0, 0.0, 1.0, 0.5 ] // Left face: purple
  ];

  var generatedColors = [];

  for (j = 0; j < 6; j++) {
    var c = colors[j];

    for ( var i = 0; i < 4; i++) {
      generatedColors = generatedColors.concat(c);
    }
  }

  cubeVerticesColorBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, cubeVerticesColorBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(generatedColors),
      gl.STATIC_DRAW);

  var cubeVertexIndices = [
                           0, 1, 2,      0, 2, 3,    // Front face
                           4, 5, 6,      4, 6, 7,    // Back face
                           8, 9, 10,     8, 10, 11,  // Top face
                           12, 13, 14,   12, 14, 15, // Bottom face
                           16, 17, 18,   16, 18, 19, // Right face
                           20, 21, 22,   20, 22, 23  // Left face
                           ];
  cubeVertexIndexBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, cubeVertexIndexBuffer);
  gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(cubeVertexIndices), gl.STATIC_DRAW);
  cubeVertexIndexBuffer.itemSize = 1;
  cubeVertexIndexBuffer.numItems = 36;
}

function initBuffers2() {
  var n =
  [
    [-1.0, 0.0, 0.0],
    [0.0, 1.0, 0.0],
    [1.0, 0.0, 0.0],
    [0.0, -1.0, 0.0],
    [0.0, 0.0, 1.0],
    [0.0, 0.0, -1.0]
  ];
  var faces =
  [
    [0, 1, 2, 3],
    [3, 2, 6, 7],
    [7, 6, 5, 4],
    [4, 5, 1, 0],
    [5, 6, 2, 1],
    [7, 4, 0, 3]
  ];
  var v=new Array(8);
  for(var i=0;i<8;i++)
    v[i]=new Array(4);
  
  var size=2;

  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

  var normals=new Array();
  var vertices=new Array();
  for (var i = 5; i >= 0; i--) {
    normals.push(n[i][0],n[i][1],n[i][2]);
    vertices.push(v[faces[i][0]][0],v[faces[i][0]][1],v[faces[i][0]][2]);
    vertices.push(v[faces[i][0]][1],v[faces[i][1]][1],v[faces[i][1]][2]);
    vertices.push(v[faces[i][0]][2],v[faces[i][2]][1],v[faces[i][2]][2]);
    vertices.push(v[faces[i][0]][3],v[faces[i][3]][1],v[faces[i][3]][2]);
  }
  
}

function drawScene() {
  gl.viewport(0, 0, gl.viewportWidth, gl.viewportHeight);
  gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

  mat4.perspective(45, gl.viewportWidth / gl.viewportHeight, 0.1, 100.0, pMatrix);

  mat4.identity(mvMatrix);

  mat4.translate(mvMatrix, [0.0, 0.0, z]);

  mat4.rotate(mvMatrix, degToRad(xRot), [1, 0, 0]);
  mat4.rotate(mvMatrix, degToRad(yRot), [0, 1, 0]);

  gl.bindBuffer(gl.ARRAY_BUFFER, cubeVertexPositionBuffer);
  gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, cubeVertexPositionBuffer.itemSize, gl.FLOAT, false, 0, 0);

  gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);
  gl.enable(gl.BLEND);
  gl.disable(gl.DEPTH_TEST);

  gl.bindBuffer(gl.ARRAY_BUFFER, cubeVerticesColorBuffer);
  gl.vertexAttribPointer(shaderProgram.vertexColorAttribute, 4, gl.FLOAT, false, 0, 0);
  
  gl.enable(gl.CULL_FACE);
  gl.cullFace(gl.FRONT);
  gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, cubeVertexIndexBuffer);
  setMatrixUniforms();
  gl.drawElements(gl.TRIANGLES, cubeVertexIndexBuffer.numItems, gl.UNSIGNED_SHORT, 0);

  gl.cullFace(gl.BACK);
  gl.drawElements(gl.TRIANGLES, cubeVertexIndexBuffer.numItems, gl.UNSIGNED_SHORT, 0);
  
  // cleanup GL state
  gl.bindBuffer(gl.ARRAY_BUFFER, null);
  gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, null);
}


var lastTime = 0;
var fps=0;

function animate(timeNow) {
  if(!timeNow) return; // first time, timeNow may be undefined
  
  if (lastTime != 0) {
    var elapsed = timeNow - lastTime;
    fps = Math.round(1000/elapsed);
    
    xRot += (xSpeed * elapsed) / 1000.0;
    yRot += (ySpeed * elapsed) / 1000.0;
  }
  lastTime = timeNow;
}

/* Before calling AntTweakBar or any other library that could use programs,
 * one must make sure to disable the VertexAttribArray used by the current
 * program otherwise this may have some unpredictable consequences aka
 * wrong vertex attrib arrays being used by another program!
 */
function enableProgram() {
  gl.useProgram(shaderProgram);
  gl.enableVertexAttribArray(shaderProgram.vertexPositionAttribute);
  gl.enableVertexAttribArray(shaderProgram.vertexColorAttribute);
}

function disableProgram() {
  gl.disableVertexAttribArray(shaderProgram.vertexPositionAttribute);
  gl.disableVertexAttribArray(shaderProgram.vertexColorAttribute);
  gl.useProgram(null);
}

function drawATB() {
  disableProgram();
  ATB.Draw();
  enableProgram();
}

function tick(timeNow) {
  drawScene(timeNow);
  animate(timeNow);

  drawATB();
    
  gl.finish(); // for timing
  if(running)
    requestAnimationFrame(tick);
}

function initAntTweakBar(canvas) {
  log('init AntTweakBar');
  
  ATB.Init();
  ATB.Define(" GLOBAL help='This example shows how to integrate AntTweakBar with GLFW and OpenGL.' "); // Message added to the help bar.
  ATB.WindowSize(canvas.width, canvas.height);

  twBar=new ATB.NewBar("Cube");
  twBar.AddVar("z", ATB.TYPE_FLOAT, {
    getter: function(data){ return z},
    setter: function(val,data) { z=val; },
  },
  " label='z' min=-5 max=5 step=0.1 keyIncr=s keyDecr=S help='Eye distance' ");

  twBar.AddVar("Orientation", ATB.TYPE_QUAT4F, {
    getter: function(data){ 
      var a=degToRad(xRot)*0.5, b=degToRad(yRot)*0.5;
      var x1=Math.sin(a),y1=0,z1=0,w1=Math.cos(a);
      var x2=0,y2=Math.sin(b),z2=0,w2=Math.cos(b);
      return [w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2,
              w1 * y2 + y1 * w2 + z1 * x2 - x1 * z2,
              w1 * z2 + z1 * w2 + x1 * y2 - y1 * x2,
              w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2]; 
      },
  },
  " label=Orientation opened=true group=Rotation help='Orientation (degree)' ");

  /*twBar.AddVar("yRot", ATB.TYPE_FLOAT, {
    getter: function(data){ return yRot; },
  },
  " label='yRot' precision=1 group=Rotation help='y rot (degree)' ");*/

  twBar.AddVar("xSpeed", ATB.TYPE_FLOAT, {
    getter: function(data){ return xSpeed; },
    setter: function(val,data){ xSpeed=val; },
  },
  " label='xSpeed' group=Rotation help='x speed' ");

  twBar.AddVar("ySpeed", ATB.TYPE_FLOAT, {
    getter: function(data){ return ySpeed; },
    setter: function(val,data){ ySpeed=val; },
  },
  " label='ySpeed' group=Rotation help='y speed' ");

  twBar.AddSeparator("misc");
  twBar.AddVar("fps", ATB.TYPE_INT32, {
    getter: function(data){ return fps; },
  },
  " label='fps' help='frames per second' ");

  //twBar.AddButton("toto",speedup,"dummy value"," label='misc' ");

}

function webGLStart() {
  var canvas = document.createElement("cube-canvas");
  // requestFullscreen is not possible yet, so do this instead
  canvas.fullscreen = true;
  initGL(canvas);
  initShaders();
  initBuffers();
  //initBuffers2();
  initAntTweakBar(canvas);

  gl.clearColor(0, 0, 0, 1);
  gl.enable(gl.DEPTH_TEST);

  tick();
}

webGLStart();

