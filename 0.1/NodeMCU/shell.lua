local current_dir = ""
args = {}

--[[function file_exists(name)
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
end]]

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
uart.setup(1, 115200, 8, uart.PARITY_NONE, uart.STOPBITS_1, 1)-- setup UART1 i.e. pin GPIO2
srv=net.createServer(net.TCP,7200)
srv:listen(2323,function(conn)
     --[[con_std = conn
        function s_output(str)
            if(con_std~=nil)
                then con_std:send(str)
            end
        end
    node.output(s_output, 0)   -- re-direct output to function s_ouput.]]
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
    --[[uart.on("data", 0,
        function(data)
            c:send(data.."\r\n")
    end, 0)]]
    --[[uart.on("data","\n",function(data) 
        print("receive from uart:", data) 
        c:send(data)
    end, 0)]]
    conn:on("sent",function(conn)

        print("[Sent]");
        --conn:close();
        --collectgarbage();
    end)
   

end)
-- a simple telnet server
--[[s=net.createServer(net.TCP)
s:listen(2323,function(c)
   con_std = c
   function s_output(str)
      if(con_std~=nil)
         then con_std:send(str)
      end
   end
   node.output(s_output, 0)   -- re-direct output to function s_ouput.
   c:on("receive",function(c,l)
        
        print(l)
        print(unpack(mysplit(l)))
        args = mysplit(l)
        
        if file.exists(args[1]..".lua") then
            dofile(args[1]..".lua")
        else
            print("command not found")
        end
        --c:send("Got it\r\n")
        
        --node.output(s_output, 0) 
      --node.input(l)           -- works like pcall(loadstring(l)) but support multiple separate line
   end)
   c:on("disconnection",function(c)
      con_std = nil
      node.output(nil)        -- un-regist the redirect output function, output goes to serial
   end)
end)]]
--telnet:open("TP-LINK_EFB4","44578572")
--while true do

   
	--io.write("> ")
    --uart.write(0, ">")
    
	--[[local cmd = io.read()
    -- when 4 chars is received.
  
    print(unpack(mysplit(cmd)))
    args = mysplit(cmd)
            --if data=="quit" then
    if file_exists(mysplit(cmd)[1]..".lua") then
        dofile(mysplit(cmd)[1]..".lua")
    else
         print("command not found")
    end]]
       
--end
