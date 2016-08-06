wifi.setmode(wifi.STATION)
wifi.sta.config("desc0n","")
print(wifi.sta.getip())

srv = net.createServer(net.TCP)
srv:listen(9000,function(conn)
    conn:on("receive", function(client,request)
        client:send("ok");
        data = client:receive();

        print(data);

        client:close();
        collectgarbage();
    end)
end)
