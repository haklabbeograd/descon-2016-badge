tmr.alarm(0, 5000, 1, function()
   if wifi.sta.getip() == nil then
      print("Connecting to AP...\n")
   else
      ip, nm, gw=wifi.sta.getip()
      print("Ip Addr: ",ip)
      tmr.stop(0)
   end
end)

srv=net.createServer(net.TCP)
srv:listen(80,function(conn)
  conn:on("receive",function(conn,payload)
    print(payload)
  end)
  conn:on("sent",function(conn) conn:close() end)
end)
