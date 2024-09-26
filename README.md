# OLED Dino Game on ESP8266

A simple dinosaur-themed game inspired by the classic Chrome Dino game, built for a 0.96-inch OLED display and powered by an ESP8266. In this game, a dinosaur jumps over obstacles while the player's score increases. The game difficulty increases as the player progresses, and high scores are saved using EEPROM.
Features

    Dino Jump: The player controls a dinosaur that jumps to avoid both vertical and horizontal obstacles.
    Dynamic Difficulty: The speed of the obstacles increases as the player’s score increases, making the game progressively more challenging.
    High Score Saving: The highest score is saved between game sessions using the ESP8266’s EEPROM.
    Game Interface:
        Welcome Screen: Before starting the game, a "Welcome" message is displayed along with a "Play" button. Once the button is pressed, the game starts.
        Game Over Screen: Upon collision with an obstacle, a "Game Over" screen is displayed with the current and highest scores. The game can be restarted by pressing the button again.
    Vertical & Horizontal Obstacles: The player must avoid both moving horizontal and vertical obstacles, adding variety to the gameplay.

Components

    ESP8266: The microcontroller used to run the game logic.
    0.96-inch OLED Display (128x64 pixels): Used to display the game.
    Button (GPIO12): A button for jumping and game interaction.
    EEPROM: For storing the high score between resets.

Circuit Connections

    OLED Display:
        SDA -> D2 (GPIO4)
        SCL -> D1 (GPIO5)
        VCC -> 3.3V
        GND -> GND
    Button (Jump):
        One terminal connected to GPIO12.
        The other terminal connected to GND.

How to Play

    Press the "Play" button to start the game.
    Use the jump button (GPIO12) to make the dinosaur jump over obstacles.
    Try to get the highest score by avoiding both vertical and horizontal obstacles.
    If you collide with an obstacle, the game will display your score, and you can restart by pressing the button again.

Installation

    Clone this repository.

    bash

    git clone https://github.com/your-username/oled-dino-game.git

    Upload the code to your ESP8266 using the Arduino IDE or your preferred method.
    Connect the OLED display and the button as described above.
    Power the ESP8266 and enjoy the game!

Libraries Used

    Adafruit GFX Library
    Adafruit SSD1306 Library
    EEPROM Library

Future Enhancements

    Add multiple difficulty levels.
    Introduce more obstacle patterns and game mechanics.
    Improve the graphics for more immersive gameplay.
Circuit Diagram
    ![11](https://github.com/user-attachments/assets/b5b6c773-b7b6-4722-9faa-2810edc96bc4)
Output
    ![IMG_20240916_144253](https://github.com/user-attachments/assets/77b8e877-633a-4729-a118-62c3bb9fbbd0)
    ![Uploading IMG_20240916_144244.jpg…]()
License

This project is licensed under the MIT License. See the LICENSE file for more details.

