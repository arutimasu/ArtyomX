current_dir = ""
args = {}

function file_exists(name)
   local f=io.open(name,"r")
   if f~=nil then io.close(f) return true else return false end
end

--- Check if a file or directory exists in this path
function exists(file)
   local ok, err, code = os.rename(file, file)
   if not ok then
      if code == 13 then
         -- Permission denied, but it exists
         return true
      end
   end
   return ok, err
end

--- Check if a directory exists in this path
function isdir(path)
   -- "/" works on both Unix and Windows
   return exists(path.."/")
end

function mysplit(inputstr, sep)
  if sep == nil then
    sep = "%s"
  end
  local t = {}
  for str in string.gmatch(inputstr, "([^"..sep.."]+)") do
    table.insert(t, str)
  end
  return t
end

--telnet:open("TP-LINK_EFB4","44578572")
print("Welcome to ArtyomX 0.2")
while true do
	io.write(current_dir.."X ")
    --uart.write(0, ">")

	local cmd = io.read()
    -- when 4 chars is received.

    print(unpack(mysplit(cmd)))
    args = mysplit(cmd)
            --if data=="quit" then
    if file_exists(mysplit(cmd)[1]..".lua") then
        dofile(mysplit(cmd)[1]..".lua")
	elseif args[1] == "cd" then
		current_dir = args[2]
    else
         print("command not found")
    end

end
