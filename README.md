This is a Node.JS port of WebGL for desktops: windows, linux, mac

@creationix kicked off this effort and provided a great example for node knockout which is the
basis for this library.  Tim's implementation required SDL, but we've paired it away and now you
can use whatever framework that creates a webgl context!

Examples for usage with various libraries like `node-sdl` or `node-glfw` live under the `lib`
directory prefixed with `platform_x.js` to use them, ensure you have the module you'd like to
use installed.

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
requestAnimationFrame(callback [, delay]) works as in the browser. 
If delay is specified, it is the requested delay in milliseconds between animation frames 
e.g. 16 will provide 1000 / 16 = 62 fps at best, which is the default value if delay is undefined. 
If delay = 0, then the fastest possible framerate on your machine is used.

The timestamp now uses the high-resolution timer in your machine (not new Date()). This provides a much more precise
framerate as well as much better timing for animations.
