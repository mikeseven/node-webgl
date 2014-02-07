This is a Node.JS port of WebGL for desktops: windows, linux, mac

It started as an extension of creationix/node-webgl and their great example com.creationix.minimason that 
you can find in examples/wavefront.js. However, it is now quite different and support different platform 
bindings, mainly GLFW instead of SDL.

Dependencies
============
- node-glfw, which depends on GLEW, GLFW as well as AntTweakBar. See node-glfw for installation instructions.
- freeimage is used to load/save a variety of image formats.
- node-gyp if not already available in your distribution

Installation
============
node-gyp rebuild

Installation for Windows 7
==========================
Beware of the Node.JS distribution you use. The default Node.JS is 32-bit and this means that modules 
will be compiled by node-gyp with 32-bit settings, which often leads to compilation errors especially
on 64-bit systems.

So for Windows 7 64-bit, instead of downloading the default Node.JS windows installer, select 'Other release files'.
This will show you an ftp site for the latest release. Go into x64 folder and download that distribution.

Usage
=====
examples/   contains examples from other the web
test/       contains lessons from www.learningwebgl.com and other tests

simply type: node test/lesson02.js

Enjoy!

Limitations
===========
WebGL is based on OpenGL ES, a restriction of OpenGL found on desktops, for embedded systems.
Because this module wraps OpenGL, it is possible to do things that may not work on web browsers. 
Please read http://www.khronos.org/webgl/wiki_1_15/index.php/WebGL_and_OpenGL_Differences 
to learn about the differences.

- shaders
Remember to add this on top of your fragment shaders:
<pre>
#ifdef GL_ES
precision highp float;
#endif
</pre>

- loading external scripts
If your code uses external libraries, you can load them like this. No code change to external scripts ;-)
<pre>
fs=require('fs');
eval(fs.readFileSync(__dirname+ '/glMatrix-0.9.5.min.js','utf8'));
</pre>

- frame rate
requestAnimationFrame(callback) works as in the browser. After the callback is done drawing, the proccess will wait
until VSync for swapping buffers. If you want to do useful work meanwhile, you can use a web worker.
For benchmarking purposes, or for purely off-screen rendering, you can disable VSync by setting
canvas.vsync to false. Don't disable vsync for regular, real time use, it causes tearing.

For a portable, high resolution time function (other than the time provided in the requestAnimationFrame callback),
use this snippet at the beginning:
<pre>
var performance = (function(){
    var hrtime = typeof process !== "undefined" && process.hrtime;
    var init = hrtime && hrtime() || Date.now();
    return ((typeof performance !== "undefined" && performance)
        || (hrtime && {now: function(){
            var hr = hrtime();
            return (hr[0]-init[0]) * 1e3 + (hr[1]-init[1]) * 1e-6;
    }}) || {now: function(){return Date.now() - init}});
})();
// and then:
performance.now()  // Gives the running time in milliseconds
</pre>
