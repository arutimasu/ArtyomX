print("NodeMCU (esp lua) - Wifi Script")
--Задаем режим клиента для wifi и подключаемся к сети

local cfg={}
cfg.ssid="TP-LINK_EFB4"
cfg.pwd="44578572"
wifi.setmode(wifi.STATION)
print("<WIFI> Подключение к точке доступа Wifi: \""..cfg.ssid.."\".")
wifi.sta.config(cfg)
print("<WIFI> IP\t\tMASK\t\t\tGATEWAY")
print(wifi.sta.getip())