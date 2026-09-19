-- Simple Lua build helper for XML_Editor
local function run(cmd)
  print("-> ", cmd)
  local res = os.execute(cmd)
  if res ~= 0 then
    error("Command failed: " .. cmd)
  end
end

print("Building XML_Editor using qmake + make")
run("qmake XML-Editor.pro")
run("make -j4")
print("Build complete")
