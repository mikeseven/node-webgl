var GLFW = require('node-glfw');
var WebGL = require('./webgl');

module.exports = function () {

    if (process.platform !== 'win32')
        process.on('SIGINT', function () { process.exit(0); });
    //process.on('exit', function () { console.log('exiting app'); });

    var events;
    var platform;
    var window;
    
    Object.defineProperty(GLFW, 'events', {
        get: function () {
            if (events) return events;
            events = new (require('events').EventEmitter);

            var _emit = events.emit;
            events.emit = function () {
              var args = Array.prototype.slice.call(arguments);
              var evt = args[1]; // args[1] is the event, args[0] is the type of event

              if(evt) evt.preventDefault = function () { };
              if(evt) evt.stopPropagation = function () { };
              if (evt && evt.type === 'resize' && platform) {
                platform.width = platform.drawingBufferWidth = platform.clientWidth = evt.width;
                platform.height = platform.drawingBufferWidth = platform.clientHeight = evt.height;
              }
              _emit.apply(this, args);
            };
            return events;
        }
    });

    GLFW.Init();
    //  GLFW.events.on('event', console.dir);
    GLFW.events.on('quit', function () { process.exit(0); });
    // GLFW.events.on("keydown", function (evt) {
    //   if (evt.keyCode === 'C'.charCodeAt(0) && evt.ctrlKey) { process.exit(0); }// Control+C
    //   if (evt.keyCode === 27) process.exit(0);  // ESC
    // });

    platform = {
        type: "nodeGLFW",
        _title: "GL window",
        setTitle: function (title) {
            this.title = title;
        },
        setIcon: function () { },
        flip: function() {
            //GLFW.SwapBuffers(window);
        },
        getElementById: function (name) {
            return null; //this;
        },
        createElement: function (name) {
            if (name.indexOf('canvas') >= 0) {
                return this;
            }
            return null;
        },
        createWindow: function (width, height, attribs) {
            var resizeListeners = [], rl = GLFW.events.listeners('resize');
            for (var l = 0, ln = rl.length; l < ln; ++l)
                resizeListeners[l] = rl[l];
            GLFW.events.removeAllListeners('resize');

            GLFW.WindowHint(GLFW.RESIZABLE, 1);
            
            attribs = attribs || {};
            // ? true:false:default
            GLFW.WindowHint(GLFW.DEPTH_BITS, attribs.depth!==undefined? attribs.depth?16:0:16);
            GLFW.WindowHint(GLFW.STENCIL_BITS, attribs.stencil!==undefined? attribs.stencil?8:0:0);
            GLFW.WindowHint(GLFW.SAMPLES, attribs.antialias!==undefined? attribs.antialias?4:0:4);
            // For more info about what to implement,
            // see http://www.glfw.org/docs/latest/window.html#window_hints
            // and https://www.khronos.org/registry/webgl/specs/1.0/#5.2

            // we use OpenGL 2.1, GLSL 1.20. Comment this for now as this is for GLSL 1.50
            //GLFW.WindowHint(GLFW.OPENGL_FORWARD_COMPAT, 1);
            //GLFW.WindowHint(GLFW.CONTEXT_VERSION_MAJOR, 3);
            //GLFW.WindowHint(GLFW.CONTEXT_VERSION_MINOR, 2);
            //GLFW.WindowHint(GLFW.OPENGL_PROFILE, GLFW.OPENGL_CORE_PROFILE);

            var monitor_index = -1;
            if(this.fullscreen){
                // GLFW doesn't allow yet to change fullscreen or window decoration
                // without re-creating the window
                // TODO: experiment with shared windows and with lost context event
                
                var monitor, monitors = GLFW.GetMonitors();
                for(var i=0; i<monitors.length; i++){
                    if(this.monitor_name !== undefined){
                        if(monitor[i].name == this.monitor_name){
                            monitor = monitors[i];
                        }
                    }else if(monitors[i].is_primary){
                        monitor = monitors[i];
                    }
                }
                monitor_index = monitor.index;
                width = monitor.width;
                height = monitor.height;
            }
            console.log(width, height, this.title, monitor_index);
            window = GLFW.CreateWindow(width, height, this.title, monitor_index);
            
            if (!window) {
                GLFW.Terminate();
                throw "Can't initialize GL surface";
            }

            GLFW.MakeContextCurrent(window);
            
            // make sure GLEW is initialized
            WebGL.Init();

            this.vsync = true;
            
            if(!this.fullscreen)
                for (var l = 0, ln = resizeListeners.length; l < ln; ++l)
                    GLFW.events.addListener('resize', resizeListeners[l]);

            var size = GLFW.GetWindowSize(window);
            this.width = this.drawingBufferWidth = this.clientWidth = size.width;
            this.height = this.drawingBufferHeight = this.clientHeight = size.height;
        },
        getContext: function (name, attribs) {
            if (!window){
                this.createWindow(this.width || 800, this.height || 800, attribs);
                this.canvas = this;
                WebGL.canvas = this;
            }
            return WebGL;
        },
        on: function (name, callback) {
            GLFW.events.on(name, callback);
        },
        addEventListener: function (name, callback) {
            GLFW.events.on(name, callback);
        },
        removeEventListener: function (name, callback) {
            GLFW.events.removeListener(name, callback);
        },
        requestAnimationFrame: function (callback) {
            setImmediate(function(){
                callback(GLFW.GetTime()*1000.0);
                GLFW.SwapBuffers(window);
                GLFW.PollEvents();
            });
        },
    };

    Object.defineProperty(platform, 'AntTweakBar', {
        get: function (cb) {
            return new GLFW.AntTweakBar();
        }
    });

    Object.defineProperty(platform, 'onkeydown', {
        set: function (cb) {
            this.on('keydown', cb);
        }
    });

    Object.defineProperty(platform, 'onkeyup', {
        set: function (cb) {
            this.on('keyup', cb);
        }
    });

    Object.defineProperty(platform, 'vsync', {
        set: function (vsync) {
            this._vsync = vsync;
            GLFW.SwapInterval(vsync? 1:0);
        },
        get: function (){
            return this._vsync;
        }
    });

    Object.defineProperty(platform, 'body', {
        get: function (){
            return this;
        }
    });
    
    Object.defineProperty(platform, 'title', {
        get: function (){
            return this._title;
        },
        set: function (t){
            this._title = t;
            GLFW.SetWindowTitle(window, t);
        }
    });
            
    
    return platform;
};

