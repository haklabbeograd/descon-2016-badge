-- init.lua --


-- Global Variables (Modify for your network)
ssid = "strawberrybush"
pass = ""

-- Configure Wireless Internet
wifi.setmode(wifi.STATION)
-- print('set mode=STATION (mode='..wifi.getmode()..')\n')
-- print('MAC Address: ',wifi.sta.getmac())
-- print('Chip ID: ',node.chipid())
-- print('Heap Size: ',node.heap(),'\n')
-- wifi config start
wifi.sta.config(ssid,pass)
-- wifi config end

-- Run the main file
dofile("main.lua")
