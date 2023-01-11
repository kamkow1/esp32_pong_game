# esp32 pong game

## how to build

### dependencies
in order to build this project, you'll have to have ESP32 IDF installed.

[official docs](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/index.html)

- building the server:

side note:

you will need a header file in `main/` directory called `wifi_creds.h`.
this file contains your wifi's crendentials which are essential for this game to run.

an example of `wifi_creds.h`:
```c
#ifndef WIFI_CREDS_H
#define WIFI_CREDS_H

#define WIFI_PASSWORD   "my wifi's password"
#define WIFI_SSID       "my wifi's ssid"

#endif
```

```
cd game_server
idf.py build
```

## how to run
- server:
```
cd game_server
./scripts/up  # flashes the code onto an ESP32 instance
```

## TODOS:
here's a list of todos/goals for this project:
- working game client in SDL2

