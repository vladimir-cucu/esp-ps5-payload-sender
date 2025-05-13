# ESP8266 PlayStation 5 Payload Sender

### Proof of concept

Send `kstuff-toggle` to the PS5 via a web page hosted at `http://10.1.1.1` from an ESP8266 access point (SSID: `PS5_WEB_AP`, password: `password`). Can be opened from the ESP Host FPKG on the PS5.

No need for any PC-side tools. Can be extended to support a custom payload list.

Works on any PS5 firmware where `elfldr` is running — including Lua/BD-JB setups.
