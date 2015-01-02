var fs = require('fs');

function create_symlink(srcpath, dstpath) {
  fs.symlink(srcpath, dstpath, function(err){
    console.log('Symlink', srcpath, dstpath);
    if (err) console.log('error', err);
  });
}

create_symlink('libglapi.so.0.0.0', 'mesa/lib/libglapi.so.0');
create_symlink('libglapi.so.0.0.0', 'mesa/lib/libglapi.so');

create_symlink('libOSMesa.so.8.0.0', 'mesa/lib/libOSMesa.so.8');
create_symlink('libOSMesa.so.8.0.0', 'mesa/lib/libOSMesa.so');