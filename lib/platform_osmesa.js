var WebGL = require('./webgl');

module.exports = function () {

    if (process.platform !== 'win32')
        process.on('SIGINT', function () { process.exit(0); });

    var events;
    var platform;
    var window;

    platform = {
        type: "nodeOSMesa",
        createElement: function (name, width, height) {
            if (name.indexOf('canvas') >= 0) {
              console.log("Creating window "+width+" x "+height);
              this.osmesaContext = WebGL.CreateOffscreenContext(WebGL.OSMESA_RGBA);
              this.canvas = this;
              WebGL.canvas = this;

              bytes_per_pixel = 4;
              this.buffer = new Uint8Array(width * height * bytes_per_pixel);
              WebGL.MakeOffscreenCurrent(this.osmesaContext, this.buffer,
                                         width, height);

              this.width = width;
              this.height = height;

              return this;
            }
            return null;
        },
        addEventListener: function (name, callback) {
        },
        getContext: function (name) {
            return WebGL;
        },
        requestAnimationFrame: function() {
        }
    };

    return platform;
};

