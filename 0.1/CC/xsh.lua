--shell.setDir("disk")

args = {}

local current_dir = ""
--args = {}

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
  if inputstr ~= nil then
  for str in string.gmatch(inputstr, "([^"..sep.."]+)") do
    table.insert(t, str)
  end
  end
  return t
end
--[[uart.setup(1, 115200, 8, uart.PARITY_NONE, uart.STOPBITS_1, 1)-- setup UART1 i.e. pin GPIO2
srv=net.createServer(net.TCP,7200)
srv:listen(2323,function(conn)
     
    conn:on("receive", function(conn,telnetdata)
        out = conn
        print(telnetdata)
        --print(unpack(mysplit(telnetdata)))
        args = mysplit(telnetdata)
        if args[1]~=nil then
        if file.exists(args[1]..".lua") then
            dofile(args[1]..".lua")
            tmr.delay(1000000)
        else
            print("command not found")
        end
        end
        --c:send("Got it\r\n")
        conn:send("Got it\r\n")
        
        --node.output(tonet, 1)  -- console also get the lua output.
    end)
     conn:on("disconnection",function(conn)
        -- not yet implemented
    end)

    conn:on("sent",function(conn)

        print("[Sent]");
        --conn:close();
        --collectgarbage();
    end)
   

end)]]
--shell.setDir("disk")
while true do

   
	io.write("xsh> ")
    --uart.write(0, ">")
    
	local cmd = io.read()
    -- when 4 chars is received.
  
    --print(unpack(mysplit(cmd)))
    args = mysplit(cmd)
            --if data=="quit" then
    if file_exists(mysplit(cmd)[1]..".lua") then
        --dofile("disk/"..mysplit(cmd)[1]..".lua")
		dofile(mysplit(cmd)[1]..".lua")
    else
         print("command not found")
    end
       
end

