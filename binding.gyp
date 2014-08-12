{
  'variables': {
    'platform': '<(OS)',
  },
  'conditions': [
    # Replace gyp platform with node platform, blech
    ['platform == "mac"', {'variables': {'platform': 'darwin'}}],
    ['platform == "win"', {'variables': {'platform': 'win32'}}],
  ],
  'targets': [
    {
      'target_name': 'webgl',
      'defines': [
        'VERSION=0.4.3'
      ],
      'sources': [ 
          'src/bindings.cc',
          'src/image.cc',
          'src/webgl.cc',
      ],
      'include_dirs': [
        "<!(node -e \"require('nan')\")",
        '<(module_root_dir)/deps/include',
      ],
      'conditions': [
        ['OS=="mac"', {
          'libraries': ['-lGLEW','-lfreeimage','-framework OpenGL']}],
        ['OS=="linux"', {'libraries': ['-lfreeimage','-lGLEW','-lGL']}],
        ['OS=="win"',
          {
            'include_dirs': [
              './deps/glew/include',
              './deps/FreeImage/include'
              ],
            'library_dirs': [
              './deps/glew/windows/lib/<(target_arch)',
              './deps/FreeImage/windows/lib/<(target_arch)'
              ],
            'libraries': [
              'glew32.lib', 
              'opengl32.lib',
              'FreeImage.lib'
              ],
            'defines' : [
              'WIN32_LEAN_AND_MEAN',
              'VC_EXTRALEAN'
            ],
            'cflags' : [
              '/O2','/Oy','/GL','/GF','/Gm-','/EHsc','/MT','/GS','/Gy','/GR-','/Gd','/wd"4530"','/wd"4251"' 
            ],
            'ldflags' : [
              '/OPT:REF','/OPT:ICF','/LTCG'
            ]
          }
        ],
      ],
    }
  ]
}
