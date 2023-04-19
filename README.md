# nodemcu-clock
This is a multifunctional desk clock using a ESP8266 or ESP32 and 4x20 LCD display.  
The device switches between different pages, which you can see below.  
By connecting the setup pins listed below with a switch, for example, you can enter a setup mode that spawns an AP to configure the device.  

&nbsp;

## Pages
| **Name** | **About** | **Demo** |
| -------- | --------- | ----------- |
| Clock    | Displays current time, date and weekday. Automatically geolocates your timezone. Uses NTP for syncing time and ip-api for geolocating. | ![Demo](https://raw.githubusercontent.com/HerrEurobeat/nodemcu-clock/main/.github/img/page-clock.gif) |
| Weather  | Displays city name, current weather, temp, sunrise and sunset time. Automatically geolocates your coords and city but can be overwritten in config. Uses openweathermap.org (API key required!). | ![Demo](https://raw.githubusercontent.com/HerrEurobeat/nodemcu-clock/main/.github/img/page-weather.gif) |
| News     | Displays 4 current headlines from different news outlets, switching to the next one on page reset. Page switch on event (headline finished scrolling through) or after time supported. Uses newsapi.org (API key required!). | ![Demo](https://raw.githubusercontent.com/HerrEurobeat/nodemcu-clock/main/.github/img/page-news.gif) |
| Spotify  | Displays your current Spotify playback title, artist and progress. | ![Demo](https://raw.githubusercontent.com/HerrEurobeat/nodemcu-clock/main/.github/img/page-spotify.gif) |

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
There are two methods of configuring your nodemcu-clock.  
You can either use the aforementioned setupMode at runtime or by editing the `config.cpp` file at compile-time.  

<details>
    <summary><b>Setup Mode (click to expand)</b></summary>

    - Bridge the setupMode pins listed above (for example with a switch which you can easily toggle)  
    - Connect power to the board or reset it  
    - The board should power up and display "Entering Setup" instead of "Connecting"  
    - Follow the instructions on the screen:  
        - Open the WiFi settings on your phone and find the "nodemcu-clock setup wifi"  
        - Connect to it with the password "setuppassword136"  
        - Open your browser and open the webpage "192.168.1.1"  
        - When done making changes, click "Save" at the bottom  
        - Unbridge the setupMode pins again and reset the device  
    
    Continue reading below to learn what needs to be configured.
</details>

<details>
    <summary><b>Compile-time Config (click to expand)</b></summary>

    - Open the `config/config.cpp` file in a text editor  
    - When done making changes, save the file, recompile the firmware and flash it onto the device. See [Compiling and flashing firmware](https://github.com/HerrEurobeat/nodemcu-clock/#compiling-and-flashing-firmware) for more information.

    Note: Changes made here will only be applied if no settings have already been saved onto the device by a previous start with compile-time config or by using the setup mode.  
    To circumvent this you can uncomment the `CLOCK_IGNOREFS` build flag in `platformio.ini`. Recompile and flash the firmware again.  
    The internal storage will now be ignored and only the compile-time config will be used.
</details>

&nbsp;

**What needs to be configured?**  
- You always need to have at least one wifi network configured.  
- Depending on which pages you use you need to have the API keys they require configured (see [Pages](https://github.com/HerrEurobeat/nodemcu-clock/#pages) for more information)

Everything else can be left at default.

**How do I get the API keys?**  
TBA