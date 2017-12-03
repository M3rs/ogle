-- script loaded at start
function load(script)
   print('lua: loading '..script)
   dofile('res/scripts/'..script..'.lua')
end

scripts = {
   "skybox",
   "flashlight",
}

for k,v in pairs(scripts) do
   load(v)
end

print('lua: startup script...')



