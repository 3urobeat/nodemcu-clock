# nodemcu-clock
This is a multifunctional desk clock using a ESP8266 or ESP32 and 4x20 LCD display.  
The device switches between different pages, which you can see below.  
By connecting the setup pins listed below with a switch, for example, you can enter a setup mode that spawns an AP to configure the device.  

&nbsp;

## Pages
| **Name** | **About** | **Picture** |
| -------- | --------- | ----------- |
| Clock    | Displays current time, date and weekday. Automatically geolocates your timezone. Uses NTP for syncing time and ip-api for geolocating. | TBA |
| Weather  | Displays city name, current weather, temp, sunrise and sunset time. Automatically geolocates your coords and city but can be overwritten in config. Uses openweathermap.org (API key required!). | ![Picture](https://raw.githubusercontent.com/HerrEurobeat/nodemcu-clock/main/.github/img/page-weather.jpg) |
| News     | Displays 4 current headlines from different news outlets, switching to the next one on page reset. Page switch on event (headline finished scrolling through) or after time supported. Uses newsapi.org (API key required!). | TBA |
| Spotify  | Displays your current Spotify playback title, artist and progress. | TBA |

&nbsp;

## Display Connection Pins
Connect VCC & GND to the respective pins of your devkit board.  

**ESP8266:**
- SDA: D2  
- SCL: D1  
- setupMode switch: D0 -> GND  

**ESP32:**
- SDA: 21  
- SCL: 22  
- setupMode switch: 19 -> GND  

&nbsp;

## Compiling and flashing firmware
TBA

&nbsp;

## Configuration & Setup Mode
TBA