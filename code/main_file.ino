#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BUTTON_JUMP 12  // GPIO12 (D6)
#define BUTTON_PLAY 13  // GPIO13 (D7), or another GPIO for the play button
#define JUMP_HEIGHT 25
#define GRAVITY 1.5
#define INITIAL_OBSTACLE_SPEED 2
#define OBSTACLE_SPEED_INCREMENT 0.1
#define OBSTACLE_WIDTH 10
#define OBSTACLE_HEIGHT 20

// Dinosaur properties
int dinoX = 10;
int dinoY = SCREEN_HEIGHT - 10;
int dinoHeight = 10;
int dinoWidth = 10;
bool isJumping = false;
int jumpSpeed = 0;

// Horizontal Obstacle properties
int horizontalObstacleY = SCREEN_HEIGHT /2; // Fixed height for horizontal obstacle
int horizontalObstacleWidth = SCREEN_WIDTH / 7;
int horizontalObstacleHeight = 7; // Height of the horizontal obstacle
int horizontalObstacleSpeed = INITIAL_OBSTACLE_SPEED; // Speed of the horizontal obstacle
int horizontalObstacleX = SCREEN_WIDTH; // Initial position

// Vertical Obstacle properties
int verticalObstacleX = SCREEN_WIDTH; // Initial position
int verticalObstacleHeight = SCREEN_HEIGHT / 2; // Height of the vertical obstacle
int verticalObstacleWidth = 10; // Width of the vertical obstacle
int verticalObstacleSpeed = INITIAL_OBSTACLE_SPEED; // Speed of the vertical obstacle

// Scores
int score = 0;
int highScore = 0;

enum GameState {WELCOME, PLAYING, GAME_OVER};
GameState currentState = WELCOME;

void setup() {
  Serial.begin(115200);
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  pinMode(BUTTON_JUMP, INPUT_PULLUP);  // Use internal pull-up resistor
  pinMode(BUTTON_PLAY, INPUT_PULLUP);  // Use internal pull-up resistor

  // Load high score from EEPROM
  EEPROM.begin(512);
  highScore = EEPROM.read(0);
  if (highScore < 0) highScore = 0; // Ensure highScore is not negative
}

void loop() {
  switch (currentState) {
    case WELCOME:
      showWelcomeScreen();
      break;
      
    case PLAYING:
      playGame();
      break;
      
    case GAME_OVER:
      showGameOverScreen();
      break;
  }
}

void showWelcomeScreen() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, SCREEN_HEIGHT / 4);
  display.println(F("Welcome"));
  display.setTextSize(1);
  display.setCursor(20, SCREEN_HEIGHT / 2);
  display.println(F("Press Play"));
  display.display();

  // Wait for play button press
  while (digitalRead(BUTTON_PLAY) == LOW) {
    // Do nothing, just wait for button press
  }
  
  // Move to the playing state
  currentState = PLAYING;
}

void playGame() {
  display.clearDisplay();  // Clear display

  // Read button state
  bool jumpButtonPressed = digitalRead(BUTTON_JUMP) == HIGH; // Button pressed

  // Handle dinosaur jump
  if (jumpButtonPressed && !isJumping) {
    isJumping = true;
    jumpSpeed = -JUMP_HEIGHT;
  }

  if (isJumping) {
    dinoY += jumpSpeed;
    jumpSpeed += GRAVITY;
    
    // Check if the dinosaur has landed
    if (dinoY >= SCREEN_HEIGHT - dinoHeight) {
      dinoY = SCREEN_HEIGHT - dinoHeight;
      isJumping = false;
    }
  }

  // Move horizontal obstacle
  horizontalObstacleX -= horizontalObstacleSpeed;
  if (horizontalObstacleX < -horizontalObstacleWidth) {
    horizontalObstacleX = SCREEN_WIDTH; // Reset position
    score++;  // Increase score when obstacle is passed
  }

  // Move vertical obstacle
  verticalObstacleX -= verticalObstacleSpeed;
  if (verticalObstacleX < -verticalObstacleWidth) {
    verticalObstacleX = SCREEN_WIDTH; // Reset position
    score++;  // Increase score when obstacle is passed
  }

  // Collision detection with horizontal obstacle
  if (dinoY + dinoHeight > horizontalObstacleY &&
      dinoX + dinoWidth > horizontalObstacleX &&
      dinoX < horizontalObstacleX + horizontalObstacleWidth) {
    currentState = GAME_OVER;
    return;
  }

  // Collision detection with vertical obstacle
  if (dinoX + dinoWidth > verticalObstacleX &&
      dinoX < verticalObstacleX + verticalObstacleWidth &&
      dinoY + dinoHeight > SCREEN_HEIGHT - verticalObstacleHeight) {
    currentState = GAME_OVER;
    return;
  }

  // Draw dinosaur
  display.fillRect(dinoX, dinoY, dinoWidth, dinoHeight, SSD1306_WHITE);

  // Draw horizontal obstacle
  display.fillRect(horizontalObstacleX, horizontalObstacleY, horizontalObstacleWidth, horizontalObstacleHeight, SSD1306_WHITE);

  // Draw vertical obstacle
  display.fillRect(verticalObstacleX, SCREEN_HEIGHT - verticalObstacleHeight, verticalObstacleWidth, verticalObstacleHeight, SSD1306_WHITE);

  // Display scores
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(F("Score: "));
  display.print(score);
  display.setCursor(0, 10);
  display.print(F("High Score: "));
  display.print(highScore);

  // Update display
  display.display();

  delay(30);  // Game speed
}

void showGameOverScreen() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, SCREEN_HEIGHT / 4);
  display.println(F("Game Over"));
  display.setTextSize(1);
  display.setCursor(20, SCREEN_HEIGHT / 2);
  display.println(F("Score: "));
  display.print(score);
  display.setCursor(20, SCREEN_HEIGHT / 2 + 16);
  display.println(F("High Score: "));
  display.print(highScore);
  display.setCursor(20, SCREEN_HEIGHT / 2 + 32);
  display.println(F("Press Play Again"));
  display.display();

  // Update high score if necessary
  if (score > highScore) {
    highScore = score;
    EEPROM.write(0, highScore);  // Save new high score to EEPROM
    EEPROM.commit();
  }

  // Wait for play again button press
  while (digitalRead(BUTTON_PLAY) == HIGH) {
    // Do nothing, just wait for button press
  }

  // Reset game
  horizontalObstacleX = SCREEN_WIDTH; // Reset horizontal obstacle position
  verticalObstacleX = SCREEN_WIDTH; // Reset vertical obstacle position
  horizontalObstacleSpeed = INITIAL_OBSTACLE_SPEED; // Reset obstacle speed
  verticalObstacleSpeed = INITIAL_OBSTACLE_SPEED; // Reset obstacle speed
  score = 0;
  dinoY = SCREEN_HEIGHT - dinoHeight; // Reset dinosaur position
  isJumping = false;
  jumpSpeed = 0;
  
  // Move to the playing state
  currentState = PLAYING;
}
