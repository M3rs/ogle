print('TODO: Fix light')

light = {
   -- for flashlight
   constant = 1.0,
   linear = 0.09,
   quadratic = 0.032,

   position = glmvec3(0.0, 0.0, 0.0),
   direction = glmvec3(0.0, 0.0, 0.0),
   
   cutOff = math.cos(math.rad(12.5)),
   outerCutOff = math.cos(math.rad(17.5)),

   ambient = glmvec3(0.6, 0.6, 0.6),
   diffuse = glmvec3(0.9, 0.9, 0.9),
   specular = glmvec3(1.0, 1.0, 1.0),
}
