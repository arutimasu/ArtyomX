print("Wait. . .")
mytimer = tmr.create()
mytimer:register(10000, tmr.ALARM_SINGLE, function(t)
    mytimer:unregister(); 
    print("Starting. . .\n")
    dofile("wifi.lua") 
    dofile("shell.lua") 
end)
mytimer:start()
