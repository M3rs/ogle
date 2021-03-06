skybox = {

   faces = {
      'res/ame_nebula/purplenebula_rt.tga',
      'res/ame_nebula/purplenebula_lf.tga',
      'res/ame_nebula/purplenebula_up.tga',
      'res/ame_nebula/purplenebula_dn.tga',
      'res/ame_nebula/purplenebula_bk.tga',
      'res/ame_nebula/purplenebula_ft.tga',
   },

   shader = {
      vert = "res/sky.vert",
      frag = "res/sky.frag",
   },

   vertices = {
      -1.0, 1.0,  -1.0, -1.0, -1.0, -1.0, 1.0,  -1.0, -1.0,
      1.0,  -1.0, -1.0, 1.0,  1.0,  -1.0, -1.0, 1.0,  -1.0,
      -1.0, -1.0, 1.0,  -1.0, -1.0, -1.0, -1.0, 1.0,  -1.0,
      -1.0, 1.0,  -1.0, -1.0, 1.0,  1.0,  -1.0, -1.0, 1.0,
      1.0,  -1.0, -1.0, 1.0,  -1.0, 1.0,  1.0,  1.0,  1.0,
      1.0,  1.0,  1.0,  1.0,  1.0,  -1.0, 1.0,  -1.0, -1.0,
      -1.0, -1.0, 1.0,  -1.0, 1.0,  1.0,  1.0,  1.0,  1.0,
      1.0,  1.0,  1.0,  1.0,  -1.0, 1.0,  -1.0, -1.0, 1.0,
      -1.0, 1.0,  -1.0, 1.0,  1.0,  -1.0, 1.0,  1.0,  1.0,
      1.0,  1.0,  1.0,  -1.0, 1.0,  1.0,  -1.0, 1.0,  -1.0,
      -1.0, -1.0, -1.0, -1.0, -1.0, 1.0,  1.0,  -1.0, -1.0,
      1.0,  -1.0, -1.0, -1.0, -1.0, 1.0,  1.0,  -1.0, 1.0,
   }
}
