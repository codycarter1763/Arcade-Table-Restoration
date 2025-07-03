# Super Winners Circle Arcade Table Restoration With Lakka

<p align="left">
  <img src="https://github.com/user-attachments/assets/8bf163d6-c494-48f8-82c2-8af24d7ceb3c" width="48%" height="48%" alt="Left Image">
  <img src="https://github.com/user-attachments/assets/155ed61f-6671-428a-b75f-5a919358d9eb" width="48%" height="48%" alt="Right Image">
</p> 

<p align="left">
  <img src="https://github.com/user-attachments/assets/f1d13faf-3c98-4a0a-b52b-2aa5d0bb6d20" width="48%" height="48%" alt="Left Image">
  <img src="https://github.com/user-attachments/assets/a1a6ffd5-c1a3-4667-8e15-ee5b5a5d6a0a" width="48%" height="48%" alt="Right Image">
</p> 

# About
Being a big fan of retro games and arcade games, I decided to take on the challenge to fully restore a defunct, abandoned 1980's Corona arcade cocktail table machine. Since a lot of essential parts were missing including a screen, game board, and other controllers, I decided to use a Raspberry Pi 3 loaded with Lakka to bring new life to this vintage machine. This upgrade gives me the ability to play thousands of old games from SNES, N64, Gameboy, NES, Amiga, MAME, and countless other consoles and computers all while keeping the original vibe. 

## History of Corona and the Winners Circle Table

<p align="left">
  <img src="https://github.com/user-attachments/assets/ec0803ab-ab80-4b7d-b36a-706b88d38838" width="48%" height="48%" alt="Left Image">
  <img src="https://github.com/user-attachments/assets/1a0b0a55-c80e-4856-bee3-ce9efa73d8e7" width="48%" height="48%" alt="Right Image">
</p> 

There is not much information out there about Corona and their arcade tables, so likely this company was very small or a local business that built their own arcade tables or rebranded generic ones. These arcade tables were used during the 80's arcade boom in bars as an alternative to standup machines. 

Winners Circle was a horse racing game based off the original board released by Corona in 1981 where you bet on seven horses over three races to win the most cash. Check out the video of some original gameplay [here](https://www.youtube.com/watch?v=5p2Suuga1zI).

# How It Started
This arcade table has been sitting in a garage in the middle of humid hot Louisiana and been through the 2016 floods. So when I recieved the table it was in very poor conditions, even with the missing parts it needed some serious TLC. My first steps was just to open up the machine, clean out dust and dirt, and see what I was working with and see what I can salvage for the final product. I also removed some heavy transformers from the unit that werent being used.  

<p align="left">
  <img src="https://github.com/user-attachments/assets/7bff3182-4f9b-4c27-88b1-3b5b6bf3308f" width="48%" height="48%" alt="Left Image">
  <img src="https://github.com/user-attachments/assets/07059f9b-6bab-4302-8882-26e521a1670c" width="48%" height="48%" alt="Right Image">
</p> 

<p align="left">
  <img src="https://github.com/user-attachments/assets/5e66cc23-d40a-497c-9095-7e4a998d9d55" width="48%" height="48%" alt="Left Image">
  <img src="https://github.com/user-attachments/assets/3cf2e0b5-3b89-4a91-83e4-2dab22fc64a6" width="48%" height="48%" alt="Right Image">
</p> 

<p align="left">
  <img src="https://github.com/user-attachments/assets/e8954414-425b-437f-850f-d04cd2dccb6e" width="48%" height="48%" alt="Left Image">
  <img src="https://github.com/user-attachments/assets/e1e153ee-cc45-4dac-b726-879dfacaf947" width="48%" height="48%" alt="Right Image">
</p> 

# Rust Removal and Veneer + Wood Repair
The metal parts of the arcade table were very rusted and took some time to remove most of it without sacrificing the original paint. I used WD-40, wirebrush, emery cloth, and sandpaper to remove the rust. While for some pieces the chrome was partly removed, the new brushed metal finish already looks a lot better than when I got started. 

For the veneer and wood, I used mineral oil to wipe to restore the original shine. As you can see in the right photo, where the left side has mineral oil and the right side does not. It made a huge difference in the finish quality. 
<p align="left">
  <img src="https://github.com/user-attachments/assets/e7ae5ce8-9ac2-4c14-8031-97e53b604219" width="48%" height="48%" alt="Left Image">
  <img src="https://github.com/user-attachments/assets/67b76196-5941-4a8b-bdd0-507564f13faf" width="48%" height="48%" alt="Right Image">
</p> 

# Retrofitting a Raspberry Pi
For the restoration, I decided to go with Lakka, a linux distribution built on top of RetroArch and LibreELEC for compatibility for many retro consoles and good performance on limited hardware. Lakka also has built-in wired and Bluetooth controller support so you can easily plug up multiple retro controllers for a unique experience. Also, since the Raspberry Pi 3 is small, it was easy to fit and secure inside the table. Check out this link [here](https://www.lakka.tv/get/). for installation instructions if you want to run Lakka yourself. 

<p align="left">
  <img src="https://github.com/user-attachments/assets/552ee541-4e04-4469-b929-9d9fde4a5842" width="48%" height="48%" alt="Left Image">
  <img src="https://github.com/user-attachments/assets/f34f82bb-2819-4abc-b0f4-382edfc993f5" width="48%" height="48%" alt="Right Image">
</p> 

# Retrofitting a Monitor
For the monitor, I used an old Dell monitor that I salvaged from a friend of mine for this build. It is just the right size to fill the space where the original CRT screen was and makes for a lightweight addition. For mounting, I used 2x4 pieces of wood cut to length to mount onto the CRT screen mounting holes. I then used velcro to secure the screen on for easy removal. 

Since the screen has HDMI, this allows me to hook up any console or computer to the aracde table.

![IMG_4855](https://github.com/user-attachments/assets/9e74c321-6e4f-4541-88be-851fa443a24b)

# Adding Neopixel RGB Lights and Salvaging Original Hardware
## The Idea
For the restoration, I decided to take another step to making this aracde table truly special. I added individually-addressable LED strips to the perimeter as an underglow with an ESP32 for custom effects. As I wanted to keep everything looking as original as possible, I used the eight original buttons and number dials on the side of the unit to control RGB presets. 

## RGB Strip
The RGB strip uses the WS2812 RGB LEDs that use a common data bus to communicate and determine how to behave. The way this works is the ESP 32 sends out a stream of 24 bit data in the form of. Where each bit can be a 1 or 0.

![Green](https://img.shields.io/badge/GGGGGGGG-00FF00?style=flat&logo=)
![Red](https://img.shields.io/badge/RRRRRRRR-FF0000?style=flat&logo=)
![Blue](https://img.shields.io/badge/BBBBBBBB-0000FF?style=flat&logo=)

The LED will read the first 24 bits of data that it recieves and stores it in its data latch, and then sends out the remaining data to the next LED and so on. 

For example, say that you have three RGB LEDs and you want to light up the third one red only. The controller would need to send out three packets of 24 bit data with this included data. 

### Packet 1
![Green](https://img.shields.io/badge/00000000-00FF00?style=flat&logo=)
![Red](https://img.shields.io/badge/00000000-FF0000?style=flat&logo=)
![Blue](https://img.shields.io/badge/00000000-0000FF?style=flat&logo=)

### Packet 2
![Green](https://img.shields.io/badge/00000000-00FF00?style=flat&logo=)
![Red](https://img.shields.io/badge/00000000-FF0000?style=flat&logo=)
![Blue](https://img.shields.io/badge/00000000-0000FF?style=flat&logo=)

### Packet 3
![Green](https://img.shields.io/badge/00000000-00FF00?style=flat&logo=)
![Red](https://img.shields.io/badge/11111111-FF0000?style=flat&logo=)
![Blue](https://img.shields.io/badge/00000000-0000FF?style=flat&logo=)

The controller will send out the three packets of data as a chain. 
- RGB 1 will store the first 24 bits of all 0's and carry
- RGB 2 will store the second 24 bits of all 0's and carry
- RGB 3 will store the third 24 bits of ![Green](https://img.shields.io/badge/00000000-00FF00?style=flat&logo=)
![Red](https://img.shields.io/badge/11111111-FF0000?style=flat&logo=)
![Blue](https://img.shields.io/badge/00000000-0000FF?style=flat&logo=) (or G=0, R=255, B=0 in decimal) to light up red

As you'd probably imagine, the complexity ramps up quickly if you want to program more vibrant effects. Though, companies and people like like Adafruit came up with a library to simplify the process called   [NeoPixel](https://learn.adafruit.com/adafruit-neopixel-uberguide/the-magic-of-neopixels). 

This simplifies the process of programming to just calling 

strip.setPixelColor(index, byte red, byte green, byte blue); 

To set color for a certain LED. 



Other alternatives include [FastLED](https://fastled.io/). , but for my project, NeoPixel worked just fine. 

## Web UI to Change RGB Presets
Since the ESP32 has onboard WiFi and Bluetooth capabilities, I decided to create an HTML web page from scratch and have it displayed to your device when you want to change what preset will display on the RGB strip when the physical buttons are pushed. The HTML webpage also displays what presets were set on a previous gaming session and will display it on its respective button. 

I gave the UI a retro Pacman theme with custom fonts and animations for a truly unique project. I used pico.css framework for customzied widgets and menu animations and this link [here](https://codepen.io/cobra_winfrey/pen/pLKevR) for the basis of the Pacman animation. 

Keep in mind the UI is not laggy in real life, I just used a gif for the repository. 
![Recording 2025-07-03 131957](https://github.com/user-attachments/assets/2e48048f-a456-48af-8825-bf7015630cec)


## Number Dial
The number dials on the side of the machine comprise of a solenoid valve that you apply a short pulse to advance the number. The display itself is fully mechanical and is self-carrying. To control solenoids, these required 24 volts DC pulse to flip to the next number. So, I connected a relay board that works off of 5V logic, added a UF4007 diode to the solenoid to prevent current and voltage spikes from damaging the relay, and added a boost converter to convert 5 volts to 24 volts. Only one of the three worked so I decided to make use of it by incrementing when an RGB preset button is pressed.</br>

<p align="left">
  <img src="https://github.com/user-attachments/assets/445550db-177d-403d-bed7-c8be23f4c321" width="48%" height="48%" alt="Left Image">
  <img src="https://github.com/user-attachments/assets/195b06f4-b655-4fa3-9f0a-410649f924b6" width="48%" height="48%" alt="Right Image">
</p> 

![image](https://github.com/user-attachments/assets/70c0a905-1198-4e68-b87a-f6b80c97f8cf)

# Software for ESP32
I used VSCode and PlatformIO for development of the HTML file and Arduino code. PlatformIO allows me to easily debug and test new changes though their included frameworks. The code is very extensive so check out the source code in my repository to see explanantions and content. 

## How To Upload Code Using PlatformIO and SPIFFS
SPIFFS is a file system that allows you to upload HTML, CSS, JS, config files, and more that you need for your application. For this project, this came in handy to develop an HTML webpage seperate from the Arduino backend and allowed for easy expansion of features.

**Keep in mind that if you upload this repository code, don't move the files in the "data" folder and "src" around or all of the font and framework dependencies won't be uploaded.**
![image](https://github.com/user-attachments/assets/c8dac4bf-71d6-4278-a590-4e75580e2cd6)

## PlatformIO
1. Open VSCode
2. Open extensions
3. Install PlatformIO IDE ![image](https://github.com/user-attachments/assets/254ccaa4-827c-44ac-aa7d-65e2a29db3a7)

4. Open this once installed ![image](https://github.com/user-attachments/assets/12fb879c-ca25-41cb-b6fe-9f07cc224a6d)

5. Create a new project, name it, choose your board, and set your framework accordingly if using C++ or Arduino
6. Check your platformio.ini file for configuration, it is dependent on your project, but this is mine ![image](https://github.com/user-attachments/assets/0820e3c7-e33f-4383-8ffe-e24732c4acb2)
7. Start coding!

## SPIFFS
SPIFFS is already included in PlatformIO and only required a few steps to start uploading.
1. Create a folder in your project folder called "data", this will be where your HTML and font files will be stored
2. Open PlatformIO icon
3. Look for PROJECT TASKS
4. Scroll down and click Build Filesystem Image
5. After building, click Upload Filesystem Image
6. Files in the data folder should be uploaded to the board!

## Uploading Arduino Code
1. After using SPIFFS to upload the "data" folder, open your main.cpp file in "src"
2. Click this drop down, build, then upload ![image](https://github.com/user-attachments/assets/5d1274b5-e20a-481f-ba23-9940cf450547)
3. All files should be uploaded and working!

## Accessing HTML Webpage
1. Open up your wifi settings and connect to the ssid set in the main.cpp file
2. Enter your password
3. Open up a browser and type in "http://IP-Address-In-Serial-Monitor"
4. Wait for it to load and you should be able to see the animations, plus saved RGB presets from previous gaming sessions that were stored in the ESP32 non-volatile flash.

# Closing
Hope you learned something from this repository and may my example help with inspiration for your project!
