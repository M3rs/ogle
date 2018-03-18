-- for a 
skybox = {

   -- faces = {
   --    'res/ame_iceflats/iceflats_rt.tga',
   --    'res/ame_iceflats/iceflats_lf.tga',
   --    'res/ame_iceflats/iceflats_up.tga',
   --    'res/ame_iceflats/iceflats_dn.tga',
   --    'res/ame_iceflats/iceflats_bk.tga',
   --    'res/ame_iceflats/iceflats_ft.tga',
   -- },
   faces = {
      'res/ame_iceflats/iceflats_rt.tga',
      'res/ame_iceflats/iceflats_lf.tga',
      'res/ame_iceflats/iceflats_dn.tga',
      'res/ame_iceflats/iceflats_up.tga',
      'res/ame_iceflats/iceflats_bk.tga',
      'res/ame_iceflats/iceflats_ft.tga',
   },

   shader = {
      vert = "res/sky.vert",
      frag = "res/sky.frag",
   },
   
   vertices = {
     -1.0,  1.0, -1.0,
     -1.0, -1.0, -1.0,
      1.0, -1.0, -1.0,
      1.0, -1.0, -1.0,
      1.0,  1.0, -1.0,
     -1.0,  1.0, -1.0,

     -1.0, -1.0,  1.0,
     -1.0, -1.0, -1.0,
     -1.0,  1.0, -1.0,
     -1.0,  1.0, -1.0,
     -1.0,  1.0,  1.0,
     -1.0, -1.0,  1.0,

      1.0,  -1.0, -1.0,
      1.0,  -1.0, 1.0,
      1.0,  1.0,  1.0,
      1.0,  1.0,  1.0,
      1.0,  1.0,  -1.0,
      1.0,  -1.0, -1.0,

      -1.0, -1.0, 1.0,
     -1.0, 1.0,  1.0,
      1.0,  1.0,  1.0,
      1.0,  1.0,  1.0,
      1.0,  -1.0, 1.0,
     -1.0, -1.0, 1.0,

     -1.0,  1.0, -1.0,
      1.0,  1.0, -1.0,
      1.0,  1.0,  1.0,
      1.0,  1.0,  1.0,
     -1.0,  1.0,  1.0,
     -1.0,  1.0, -1.0,

    -1.0, -1.0, -1.0,
    -1.0, -1.0,  1.0,
     1.0, -1.0, -1.0,
     1.0, -1.0, -1.0,
    -1.0, -1.0,  1.0,
     1.0, -1.0,  1.0,
   }
}
