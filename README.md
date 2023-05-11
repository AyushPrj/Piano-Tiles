# Piano-Tiles game in Embedded C
Piano Tiles is a fun arcade style game that challenges your reflexes and musical skills. The game consists of a grid of colorful tiles that scroll down the screen with each color representing a different note. Everytime a colorful tile is correctly played, the corresponding note is produced on the speaker. Your goal is to tap only the colorful tiles as they appear and in doing so, play the entire song. There is also a scoring mechanism that lets you know how you did.

Components used:
1) A MSP2202 TFT LCD to show the player the note we want them to play. 
2) 8 Push-buttons to collect user input.
3) A SOC1602A OLED to display instructions and score. 
5) A microcontroller (STM32F091RCT)
6) Many resistors and capacitors.
7) A Speaker

Internal Peripherals used:
1) GPIO
2) Timers
3) SPI
4) DAC
