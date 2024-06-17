#include <Adafruit_GFX.h>
#include <LedControl.h>
#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
enum SmerPong
{
  UPPONG,
  DOWNPONG,
  NOTHINGPONG
};

#define ROWS 32 // you can change height and width of table with ROWS and COLS
#define COLS 8
#define TRUE 1
#define FALSE 0

int rotate_button = 2;
int left_button = 20;
int down_button = 4;
int right_button = 5;

int tickSleep = 0;
int tickOver = 0;

int baseSpeed = 200; // lower is faster
int modSpeed = baseSpeed;
char Table[ROWS][COLS] = {0};
int score = 300;
char GameOn = FALSE;

typedef struct
{
  char **array;
  int width, row, col;
} Shape;
Shape current;



// We are doing some Arkanoid
struct
{
  int width = 128;
  byte height = 64;
  byte playerWidth = 24;
} resolution;

struct PlayerPong
{
  int y;
  int score;
  SmerPong smerPong = NOTHINGPONG;
};
struct BallPong
{
  int x;
  int y;
  int dx;
  int dy;
};

struct GamePong
{
  PlayerPong player1;
  PlayerPong player2;
  BallPong ball;
};
bool hadRunning = false;
bool arkanoidRunning = false;
bool pongRunning = false;
bool tetrisRunning = false;
GamePong gamePong;
LedControl matrix = LedControl(5, 3, 4, 4);
const short intensity = 8;
const int ROWSHAD = 8;
const int COLSHAD = 32;
int XbyloHad = 0;
int YbyloHad = 0;
int souradniceHad[ROWSHAD][COLSHAD] = {};
int XsouradniceHad = 0;
int YsouradniceHad = 0;
int pohybXHad = 1;
int pohybYHad = 0;
int souradniceHadJidloX = -1;
int nulaX;
int nulaY;
int souradniceHadJidloY = -1;
enum SmerHad
{
  UPHAD,
  DOWNHAD,
  LEFTHAD,
  RIGHTHAD
};
enum SmerHad smerHad = RIGHTHAD;
int hadLength = 3;
void resetHad()
{
  int sektor = 0;
  int docasna = 0;
  hadRunning = false;
  for (int i = 0; i < ROWSHAD; i++)
  {
    for (int j = 0; j < COLSHAD; j++)
    {
      souradniceHad[i][j] = 0;
    }
  }
  for (int l = 0; l < 4; l++)
  {
    for (int i = 0; i < 8; i++)
    {
      for (int j = 0; j < 8; j++)
      {
        matrix.setLed(l, i, j, 0);
      }
    }
       if ((souradniceHadJidloX / 8) >= 3)
  {
    sektor = 0;
    docasna = souradniceHadJidloX - 24;
  }
  else if ((souradniceHadJidloX / 8) >= 2)
  {
    sektor = 1;
    docasna = souradniceHadJidloX - 16;
  }
  else if ((souradniceHadJidloX / 8) >= 1)
  {
    sektor = 2;
    docasna = souradniceHadJidloX - 8;
  }
  else
  {
    sektor = 3;
    docasna = souradniceHadJidloX;
  }
  matrix.setLed(sektor, souradniceHadJidloY, docasna, 0);
  }
  XbyloHad = 0;
  YbyloHad = 0;
  XsouradniceHad = 0;
  YsouradniceHad = 0;
  pohybXHad = 1;
  pohybYHad = 0;
  souradniceHadJidloX = -1;
  souradniceHadJidloY = -1;
  hadLength = 3;
  matrix.clearDisplay(0);
    matrix.clearDisplay(1);
    matrix.clearDisplay(2);
    matrix.clearDisplay(3);
    gameOver();
}
void generateJidlo()
{
  if (hadLength >= 32 * 8)
  {
    return;
  }
  int y = rand() % ROWSHAD;
  int x = rand() % COLSHAD;
  souradniceHadJidloX = x;
  souradniceHadJidloY = y;
  int sektor = 0;
  int docasna = 0;
  if ((souradniceHadJidloX / 8) >= 3)
  {
    sektor = 0;
    docasna = souradniceHadJidloX - 24;
  }
  else if ((souradniceHadJidloX / 8) >= 2)
  {
    sektor = 1;
    docasna = souradniceHadJidloX - 16;
  }
  else if ((souradniceHadJidloX / 8) >= 1)
  {
    sektor = 2;
    docasna = souradniceHadJidloX - 8;
  }
  else
  {
    sektor = 3;
    docasna = souradniceHadJidloX;
  }
  matrix.setLed(sektor, souradniceHadJidloY, docasna, 1);
}

void preteceni()
{
  if (XsouradniceHad < 0)
  {
    XsouradniceHad = ROWSHAD - 1;
  }
  if (XsouradniceHad >= ROWSHAD)
  {
    XsouradniceHad = 0;
  }
  if (YsouradniceHad < 0)
  {
    YsouradniceHad = COLSHAD - 1;
  }
  if (YsouradniceHad >= COLSHAD)
  {
    YsouradniceHad = 0;
  }
}

enum SmerArkanoid
{
  LEFT,
  RIGHT,
  NOTHING,
  UP,
  DOWN
};
SmerArkanoid smerMenu = NOTHING;

struct PlayerArkanoid
{
  int x = 100;
  int score;
  SmerArkanoid smerArkanoid = NOTHING;
};

int zeroX1;
int zeroY1;
int arkanoidTries = 3;
struct GameArkanoid
{
  PlayerArkanoid player1;
  int destructibleBariers[8][4];
  int Bariers[8][4];
  int ball[2] = {64, 32}; // Initialize the ball position
  int dx = 1;             // Ball horizontal direction
  int dy = 1;             // Ball vertical direction
  int barierWidth = 8;
  int barierHeight = 2;
};

struct
{
  int width = 128;
  int height = 64;
  int playerWidth =  32;
} resolutionArkanoidReworked;

enum SmerArkanoidReworked
{
  LEFTArkanoidReworked,
  RIGHTArkanoidReworked,
  NOTHINGArkanoidReworked
};

struct PlayerArkanoidReworked
{
  int x = 90;
  int score;
  SmerArkanoidReworked smerArkanoidReworked = NOTHINGArkanoidReworked;
};

int zeroX1ArkanoidReworked;
int zeroY1ArkanoidReworked;

struct GameArkanoidReworked
{
  PlayerArkanoidReworked player1;
  int destructibleBariers[8][4];
  int Bariers[8][4];
  int ballArkanoidReworked[2] = {64, 32}; // Initialize the ballArkanoidReworked position
  int dx = 1;                             // BallArkanoidReworked horizontal direction
  int dy = 1;                             // BallArkanoidReworked vertical direction
  int barierWidth = 16;
  int barierHeight = 8;
};

GameArkanoidReworked gameArkanoidReworked;

void drawPlayerArkanoidReworked()
{
  display.fillRect(gameArkanoidReworked.player1.x, resolutionArkanoidReworked.height - 2, resolutionArkanoidReworked.playerWidth, 2, WHITE);
}

void resetArkanoidReworked()
{
  arkanoidTries == 0 ? arkanoidRunning = false : arkanoidTries--;
  gameArkanoidReworked.ballArkanoidReworked[0] = 64;
  gameArkanoidReworked.ballArkanoidReworked[1] = 32;
  gameArkanoidReworked.dx = 1;
  gameArkanoidReworked.dy = 1;
  // and barrier resetArkanoidReworked
  memset(gameArkanoidReworked.Bariers, 0, sizeof(gameArkanoidReworked.Bariers));
  memset(gameArkanoidReworked.destructibleBariers, 0, sizeof(gameArkanoidReworked.destructibleBariers));
  gameArkanoidReworked.player1.x = 70;
  generateBariers();
  

}

void generateBariers()
{
  randomSeed(analogRead(9));
  for (int i = 0; i < 20; i++)
  {
    int sektorY = random(0, 4);
    int sektorX = random(0, 8);
    if (gameArkanoidReworked.destructibleBariers[sektorX][sektorY] == 0)
    {
      gameArkanoidReworked.destructibleBariers[sektorX][sektorY] = 1;
    }
    else
    {
      i--;
    }
  }
  for (int i = 0; i < 6; i++)
  {
    int sektorY = random(0, 4);
    int sektorX = random(0, 8);
    if (gameArkanoidReworked.Bariers[sektorX][sektorY] == 0)
    {
      gameArkanoidReworked.Bariers[sektorX][sektorY] = 1;
    }
    else
    {
      i--;
    }
  }
}

GameArkanoid gameArkanoid;
void resetArkanoid()
{
  arkanoidTries == 0 ? arkanoidRunning = false : arkanoidTries--;
  gameArkanoid.ball[0] = 64;
  gameArkanoid.ball[1] = 32;
  gameArkanoid.dx = 1;
  gameArkanoid.dy = 1;
  // and barrier reset
  memset(gameArkanoid.Bariers, 0, sizeof(gameArkanoid.Bariers));
  memset(gameArkanoid.destructibleBariers, 0, sizeof(gameArkanoid.destructibleBariers));
  generateBariers();
}
void drawPlayerArkanoid()
{
  display.fillRect(gameArkanoid.player1.x, resolution.height - 2, resolution.playerWidth, 2, WHITE);
}

struct ResolutionPong
{
  int width = 128;
  int height = 64;
  int playerHeight = 16;
} resolutionPong;

void vyhral(int hrac)
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Hrac ");
  display.print(hrac);
  display.print(" vyhral!");
  display.display();
   initGamePong(gamePong);
   gameOver();
  delay(2000);
}

void initGamePong(GamePong &gamePong1)
{
  gamePong1.player1.y = resolutionPong.height / 2 - (resolutionPong.playerHeight / 2);
  gamePong1.player2.y = resolutionPong.height / 2 - (resolutionPong.playerHeight / 2);
  gamePong1.player1.score = 0;
  gamePong1.player2.score = 0;
  gamePong1.ball.x = resolutionPong.width / 2;
  gamePong1.ball.y = random(0, resolutionPong.height);
  gamePong1.ball.dx = 1;
  gamePong1.ball.dy = 1;
}

void botPong()
{
  if (gamePong.ball.y >= gamePong.player2.y + 5 && gamePong.ball.y <= gamePong.player2.y + resolutionPong.playerHeight - 5)
  {
    return;
  }
  if (gamePong.ball.y >= (gamePong.player2.y + resolutionPong.playerHeight / 2))
  {
    gamePong.player2.y += 1;
    return;
  }
  else
  {
    gamePong.player2.y -= 1;
    return;
  }
}

void printGamePong(GamePong &gamePong)
{
  display.clearDisplay();

  // Draw players
  display.fillRect(1, gamePong.player1.y, 2, resolutionPong.playerHeight, WHITE);
  display.fillRect(resolutionPong.width - 3, gamePong.player2.y, 2, resolutionPong.playerHeight, WHITE);

  // Draw ball
  display.fillCircle(gamePong.ball.x, gamePong.ball.y, 2, WHITE);

  // Draw scores
  display.setCursor(0, 0);
  display.print(gamePong.player1.score);
  display.setCursor(resolutionPong.width - 10, 0);
  display.print(gamePong.player2.score);

  display.display();
}

void mainLogicPongPong()
{
  ziskejInput();
  switch (gamePong.player1.smerPong)
  {
  case UPPONG:
    if (gamePong.player1.y > 0)
      gamePong.player1.y--;
    break;
  case DOWNPONG:
    if (gamePong.player1.y < resolutionPong.height - resolutionPong.playerHeight)
      gamePong.player1.y++;
    break;
  }
  botPong();
  gamePong.ball.x += gamePong.ball.dx;
  gamePong.ball.y += gamePong.ball.dy;

  // BallPong collision with top and botPongtom
  if (gamePong.ball.y <= 0 || gamePong.ball.y >= resolutionPong.height)
  {
    gamePong.ball.dy = -gamePong.ball.dy;
  }

  // BallPong collision with paddles
  if (gamePong.ball.x <= 3 && gamePong.ball.y >= gamePong.player1.y && gamePong.ball.y <= gamePong.player1.y + resolutionPong.playerHeight)
  {
    gamePong.ball.dx = -gamePong.ball.dx;
  }
  else if (gamePong.ball.x >= resolutionPong.width - 4 && gamePong.ball.y >= gamePong.player2.y && gamePong.ball.y <= gamePong.player2.y + resolutionPong.playerHeight)
  {
    gamePong.ball.dx = -gamePong.ball.dx;
  }

  // BallPong out of bounds
  if (gamePong.ball.x <= 0)
  {
    gamePong.player2.score++;
    gamePong.ball.x = resolutionPong.width / 2;
    gamePong.ball.y = random(0, resolutionPong.height);
    if (gamePong.player2.score >= 3)
    {
      pongRunning = false;
      vyhral(2);
    }
  }
  else if (gamePong.ball.x >= resolutionPong.width)
  {
    gamePong.player1.score++;
    gamePong.ball.x = resolutionPong.width / 2;
    gamePong.ball.y = random(0, resolutionPong.height);
    if (gamePong.player1.score  >=  3)
    {
      pongRunning = false;
      vyhral(1);
    }
  }

  printGamePong(gamePong);
}

String Menu[4] = {"Had", "Arkanoid", "Pong", "Tetris"};
int MenuPosition = 0;
void drawMenu()
{
  display.clearDisplay();
  for (int i = 0; i < 4; i++)
  {
    display.setCursor(8, i * 8);
    display.print(Menu[i]);
    if (i == MenuPosition)
    {
       display.setCursor(0, i * 8);
      display.print(">");
    }
  }
  display.display();
}

void MenuLoop()
{
  drawMenu();
  while (true)
  {
    ziskejInput();
    if (smerMenu == UP)
    {
      MenuPosition--;
      if (MenuPosition < 0)
      {
        MenuPosition = 3;
      }
      drawMenu();
      delay(100);
    }
    else if (smerMenu == DOWN)
    {
      MenuPosition++;
      if (MenuPosition > 3)
      {
        MenuPosition = 0;
      }
      drawMenu();
      delay(100);
    }

    if (smerMenu == RIGHT)
    {
      if (MenuPosition == 0)
      {
        // Had
        Had();
      }
      else if (MenuPosition == 1)
      {
        // Arkanoid
        Arkanoid();
      }
      else if (MenuPosition == 2)
      {
        // Pong
        Pong();
      }
      else if (MenuPosition == 3)
      {
        // Tetris
        Tetris();
      }
    }
    else if (smerMenu == LEFT)
    {
      break;
    }
    else if (smerMenu == NOTHING)
    {
      break;
    }
    delay(100);
  }
}

void Had()
{
  hadRunning = true;
  generateJidlo();
  delay(33);
  while (hadRunning)
  {
    mainLogicHad();
    delay(200);
  }
}

void Pong()
{
  display.clearDisplay();
  display.display();
  pongRunning = true;
  while (pongRunning)
  {
    mainLogicPongPong();
    delay(20);
  }
}



// Funkce pro výpis pole souradniceHad

void drawBariers()
{
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      if (gameArkanoidReworked.destructibleBariers[i][j] == 1)
      {
        display.drawRect(i * 16, j * 8, 15, 7, WHITE);
      }
      if (gameArkanoidReworked.Bariers[i][j] == 1)
      {
        display.fillRect(i * 16, j * 8, 15, 7, WHITE);
      }
    }
  }
}

void drawBallArkanoidReworked()
{
  display.fillRect(gameArkanoidReworked.ballArkanoidReworked[0], gameArkanoidReworked.ballArkanoidReworked[1], 2, 2, WHITE);
}

void updateBallArkanoidReworked()
{
  gameArkanoidReworked.ballArkanoidReworked[0] += gameArkanoidReworked.dx;
  gameArkanoidReworked.ballArkanoidReworked[1] += gameArkanoidReworked.dy;

  // Check for collisions with walls
  if (gameArkanoidReworked.ballArkanoidReworked[0] <= 0 || gameArkanoidReworked.ballArkanoidReworked[0] >= resolutionArkanoidReworked.width - 2)
  {
    gameArkanoidReworked.dx = -gameArkanoidReworked.dx;
  }
  if (gameArkanoidReworked.ballArkanoidReworked[1] <= 0)
  {
    gameArkanoidReworked.dy = -gameArkanoidReworked.dy;
  }

  // Check for collisions with barriers
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      if (gameArkanoidReworked.destructibleBariers[i][j] == 1 || gameArkanoidReworked.Bariers[i][j] == 1)
      {
        int barierX = i * 16;
        int barierY = j * 8;

        // Check collision with left or right side of the barrier
        if ((gameArkanoidReworked.ballArkanoidReworked[0] == barierX + 1 || gameArkanoidReworked.ballArkanoidReworked[0] == barierX + gameArkanoidReworked.barierWidth - 1) &&
            (gameArkanoidReworked.ballArkanoidReworked[1] >= barierY + 1 && gameArkanoidReworked.ballArkanoidReworked[1] <= barierY + gameArkanoidReworked.barierHeight - 1))
        {
          gameArkanoidReworked.dx = -gameArkanoidReworked.dx;
          if (gameArkanoidReworked.destructibleBariers[i][j])
          {
            gameArkanoidReworked.destructibleBariers[i][j] = 0;
          }
        }

        // Check collision with top or bottom side of the barrier
        if ((gameArkanoidReworked.ballArkanoidReworked[1] == barierY + 1 || gameArkanoidReworked.ballArkanoidReworked[1] == barierY + gameArkanoidReworked.barierHeight - 1) &&
            (gameArkanoidReworked.ballArkanoidReworked[0] >= barierX + 1 && gameArkanoidReworked.ballArkanoidReworked[0] <= barierX + gameArkanoidReworked.barierWidth - 1))
        {
          gameArkanoidReworked.dy = -gameArkanoidReworked.dy;
          if (gameArkanoidReworked.destructibleBariers[i][j])
          {
            gameArkanoidReworked.destructibleBariers[i][j] = 0;
          }
        }

        // If the barrier is destructible, remove it
      }
    }
  }

  // Check for collisions with player
  if (gameArkanoidReworked.ballArkanoidReworked[1] >= resolutionArkanoidReworked.height - 4 && gameArkanoidReworked.ballArkanoidReworked[0] >= gameArkanoidReworked.player1.x && gameArkanoidReworked.ballArkanoidReworked[0] <= gameArkanoidReworked.player1.x + resolutionArkanoidReworked.playerWidth)
  {
    gameArkanoidReworked.dy = -gameArkanoidReworked.dy;
  }

  // Reset the gameArkanoidReworked if the ballArkanoidReworked goes below the player's paddle
  if (gameArkanoidReworked.ballArkanoidReworked[1] > resolutionArkanoidReworked.height - 2)
  {
    resetArkanoidReworked();
  }
}

void mainLogicHad()
{
  ziskejInput();
  int puvodniX = pohybXHad;
  int puvodniY = pohybYHad;
  switch (smerHad)
  {
  case UPHAD:
    pohybXHad = -1;
    pohybYHad = 0;
    break;
  case DOWNHAD:
    pohybXHad = 1;
    pohybYHad = 0;
    break;
  case LEFTHAD:
    pohybXHad = 0;
    pohybYHad = -1;
    break;
  case RIGHTHAD:
    pohybXHad = 0;
    pohybYHad = 1;
    break;
  }
  if (puvodniX == -pohybXHad || puvodniY == -pohybYHad)
  {
    pohybYHad == puvodniY;
    pohybXHad == puvodniX;
  }

  if (souradniceHadJidloX == YsouradniceHad && souradniceHadJidloY == XsouradniceHad)
  {
    hadLength++;
    souradniceHadJidloX = -1;
    souradniceHadJidloY = -1;
    for (int i = 0; i < ROWSHAD; ++i)
    {
      for (int j = 0; j < COLSHAD; ++j)
      {
        if (souradniceHad[i][j] > 0)
        {
          souradniceHad[i][j]++;
        }
      }
    }
    generateJidlo();
  }

  XsouradniceHad += pohybXHad;
  YsouradniceHad += pohybYHad;
  preteceni();
  if (souradniceHad[XsouradniceHad][YsouradniceHad] > 0 && souradniceHadJidloX != YsouradniceHad && souradniceHadJidloY != XsouradniceHad)
  {
    resetHad();
    return;
  }
  souradniceHad[XsouradniceHad][YsouradniceHad] = hadLength + 1;
  for (int i = 0; i < ROWSHAD; ++i)
  {
    for (int j = 0; j < COLSHAD; ++j)
    {
      if (souradniceHad[i][j] > 0)
      {
        souradniceHad[i][j]--;
        //          Serial.print("J je ");
        //   Serial.println(j);

        int sektor = 0;
        int docasna = 0;
        if ((j / 8) >= 3)
        {
          sektor = 0;
          docasna = j - 24;
        }
        else if ((j / 8) >= 2)
        {
          sektor = 1;
          docasna = j - 16;
        }
        else if ((j / 8) >= 1)
        {
          sektor = 2;
          docasna = j - 8;
        }
        else
        {
          sektor = 3;
          docasna = j;
        }
        //     Serial.print("J jako docasna je ");
        //    Serial.println(docasna);
        //       Serial.print("sektor je ");
        //    Serial.println(sektor);
        matrix.setLed(sektor, i, docasna, souradniceHad[i][j] == 0 ? 0 : 1);

        //   switch (sektor) {
        //     case 0:
        //      matrix.setLed(sektor, i, j, souradniceHad[i][j] == 0 ? 0 : 1);
        //      break;
        //     case 1:
        //      matrix.setLed(sektor, i, j - 8, souradniceHad[i][j] == 0 ? 0 : 1);
        //      break;
        //     case 2:
        //     matrix.setLed(sektor, i, j - 16, souradniceHad[i][j] == 0 ? 0 : 1);
        //     break;
        //     case 3:
        //     matrix.setLed(sektor, i, j - 24, souradniceHad[i][j] == 0 ? 0 : 1);
        //     break;
        //   }
      }
    }
  }

  YbyloHad = YsouradniceHad;
  XbyloHad = XsouradniceHad;
}

void ziskejInput()
{
  int intA7 = analogRead(A6) - zeroX1;
  int intA2 = analogRead(A7) - zeroY1;
  if (intA2 < -80 && smerHad != DOWNHAD)
  {
    smerHad = UPHAD;
  }
  else if (intA2 > 80 && smerHad != UPHAD)
  {
    smerHad = DOWNHAD;
  }

  if (intA7 < -80 && smerHad != RIGHTHAD)
  {
    smerHad = LEFTHAD;
  }
  else if (intA7 > 80 && smerHad != LEFTHAD)
  {
    smerHad = RIGHTHAD;
  }
  if (intA7 < -80)
  {
    gamePong.player1.smerPong = UPPONG;
  }
  else if (intA7 > 80)
  {
    gamePong.player1.smerPong = DOWNPONG;
  }
  else
  {
    gamePong.player1.smerPong = NOTHINGPONG;
  }
  if (intA7 > 80)
  {
    smerMenu = DOWN;
  }
  else if (intA7 < -80)
  {
    smerMenu = UP;
  }
  else
  {
    smerMenu = NOTHING;
  }
  if (intA2 < -80)
  {
    gameArkanoid.player1.smerArkanoid = RIGHT;
    smerMenu = RIGHT;
  }
  else if (intA7 < -80)
  {
    gameArkanoid.player1.smerArkanoid = LEFT;
  }
  else
  {
    gameArkanoid.player1.smerArkanoid = NOTHING;
  }

  if (gameArkanoid.player1.smerArkanoid == RIGHT && gameArkanoid.player1.x + resolution.playerWidth < resolution.width)
  {
    gameArkanoid.player1.x += 1;
  }
  else if (gameArkanoid.player1.smerArkanoid == LEFT && gameArkanoid.player1.x > 0)
  {
    gameArkanoid.player1.x -= 1;
  }
  if (intA2 < -80)
  {
    gameArkanoidReworked.player1.smerArkanoidReworked = RIGHTArkanoidReworked;
  }
  else if (intA2 > 80)
  {
    gameArkanoidReworked.player1.smerArkanoidReworked = LEFTArkanoidReworked;
  }
  else
  {
    gameArkanoidReworked.player1.smerArkanoidReworked = NOTHINGArkanoidReworked;
  }
  if (gameArkanoidReworked.player1.smerArkanoidReworked == RIGHTArkanoidReworked && gameArkanoidReworked.player1.x + resolutionArkanoidReworked.playerWidth < resolutionArkanoidReworked.width)
  {
    gameArkanoidReworked.player1.x += 1;
  }
  else if (gameArkanoidReworked.player1.smerArkanoidReworked == LEFTArkanoidReworked && gameArkanoidReworked.player1.x > 0)
  {
    gameArkanoidReworked.player1.x -= 1;
  }
}

void drawBall()
{
  display.fillRect(gameArkanoid.ball[0], gameArkanoid.ball[1], 2, 2, WHITE);
}

void updateBall()
{
  gameArkanoid.ball[0] += gameArkanoid.dx;
  gameArkanoid.ball[1] += gameArkanoid.dy;

  // Check for collisions with walls
  if (gameArkanoid.ball[0] <= 0 || gameArkanoid.ball[0] >= resolution.width - 2)
  {
    gameArkanoid.dx = -gameArkanoid.dx;
  }
  if (gameArkanoid.ball[1] <= 0)
  {
    gameArkanoid.dy = -gameArkanoid.dy;
  }

  // Check for collisions with barriers
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      if (gameArkanoid.destructibleBariers[i][j] == 1 || gameArkanoid.Bariers[i][j] == 1)
      {
        int barierX = i * 16;
        int barierY = j * 8;

        // Check collision with left or right side of the barrier
        if ((gameArkanoid.ball[0] == barierX + 1 || gameArkanoid.ball[0] == barierX + gameArkanoid.barierWidth - 1) &&
            (gameArkanoid.ball[1] >= barierY + 1 && gameArkanoid.ball[1] <= barierY + gameArkanoid.barierHeight - 1))
        {
          gameArkanoid.dx = -gameArkanoid.dx;
          if (gameArkanoid.destructibleBariers[i][j])
          {
            gameArkanoid.destructibleBariers[i][j] = 0;
          }
        }

        // Check collision with top or bottom side of the barrier
        if ((gameArkanoid.ball[1] == barierY + 1 || gameArkanoid.ball[1] == barierY + gameArkanoid.barierHeight - 1) &&
            (gameArkanoid.ball[0] >= barierX + 1 && gameArkanoid.ball[0] <= barierX + gameArkanoid.barierWidth - 1))
        {
          gameArkanoid.dy = -gameArkanoid.dy;
          if (gameArkanoid.destructibleBariers[i][j])
          {
            gameArkanoid.destructibleBariers[i][j] = 0;
          }
        }

        // If the barrier is destructible, remove it
      }
    }
  }

  // Check for collisions with player
  if (gameArkanoid.ball[1] >= resolution.height - 4 && gameArkanoid.ball[0] >= gameArkanoid.player1.x && gameArkanoid.ball[0] <= gameArkanoid.player1.x + resolution.playerWidth)
  {
    gameArkanoid.dy = -gameArkanoid.dy;
  }

  // Reset the gameArkanoid if the ball goes below the player's paddle
  if (gameArkanoid.ball[1] > resolution.height - 2)
  {
    resetArkanoid();
  }
}
void setup()
{

  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);

  zeroX1 = analogRead(A6);
  zeroY1 = analogRead(A7);

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  generateBariers();
  initGamePong(gamePong);
  matrix.shutdown(0, false);
  matrix.setIntensity(0, intensity);
  matrix.clearDisplay(0);
  matrix.shutdown(1, false);
  matrix.setIntensity(1, intensity);
  matrix.clearDisplay(1);
  matrix.shutdown(2, false);
  matrix.setIntensity(2, intensity);
  matrix.clearDisplay(2);
  matrix.shutdown(3, false);
  matrix.setIntensity(3, intensity);
  matrix.clearDisplay(3);
  
}

void mainLogic()
{
  ziskejInput();
  updateBall();
}

void Arkanoid()
{
  arkanoidRunning = true;
  arkanoidTries == 3;
  while (arkanoidRunning)
  {
    display.clearDisplay();
    drawBariers();
    drawPlayerArkanoidReworked();
    drawBallArkanoidReworked();
    display.display();
    mainLogicArkanoidReworked();
    delay(1000 / 60);
  }
  gameOver();
  arkanoidTries == 3;
}
void loop()
{
  MenuLoop();
  delay(20);
}

void mainLogicArkanoidReworked()
{
  ziskejInput();
  updateBallArkanoidReworked();
}



const Shape ShapesArray[7] = {
    {(char *[]){(char[]){0, 1, 1}, (char[]){1, 1, 0}, (char[]){0, 0, 0}}, 3},                               // S shape
    {(char *[]){(char[]){1, 1, 0}, (char[]){0, 1, 1}, (char[]){0, 0, 0}}, 3},                               // Z shape
    {(char *[]){(char[]){0, 1, 0}, (char[]){1, 1, 1}, (char[]){0, 0, 0}}, 3},                               // T shape
    {(char *[]){(char[]){0, 0, 1}, (char[]){1, 1, 1}, (char[]){0, 0, 0}}, 3},                               // L shape
    {(char *[]){(char[]){1, 0, 0}, (char[]){1, 1, 1}, (char[]){0, 0, 0}}, 3},                               // flipped L shape
    {(char *[]){(char[]){1, 1}, (char[]){1, 1}}, 2},                                                        // square shape
    {(char *[]){(char[]){0, 0, 0, 0}, (char[]){1, 1, 1, 1}, (char[]){0, 0, 0, 0}, (char[]){0, 0, 0, 0}}, 4} // long bar shape
                                                                                                            // you can add any shape like it's done above. Don't be naughty.
};

Shape CopyShape(Shape shape)
{
  Shape new_shape = shape;
  char **copyshape = shape.array;
  new_shape.array = (char **)malloc(new_shape.width * sizeof(char *));
  int i, j;
  for (i = 0; i < new_shape.width; i++)
  {
    new_shape.array[i] = (char *)malloc(new_shape.width * sizeof(char));
    for (j = 0; j < new_shape.width; j++)
    {
      new_shape.array[i][j] = copyshape[i][j];
    }
  }
  return new_shape;
}

void DeleteShape(Shape shape)
{
  int i;
  for (i = 0; i < shape.width; i++)
  {
    free(shape.array[i]);
  }
  free(shape.array);
}

int CheckPosition(Shape shape)
{ // Check the position of the copied shape
  char **array = shape.array;
  int i, j;
  for (i = 0; i < shape.width; i++)
  {
    for (j = 0; j < shape.width; j++)
    {
      if ((shape.col + j < 0 || shape.col + j >= COLS || shape.row + i >= ROWS))
      {                  // Out of borders
        if (array[i][j]) // but is it just a phantom?
          return FALSE;
      }
      else if (Table[shape.row + i][shape.col + j] && array[i][j])
        return FALSE;
    }
  }
  return TRUE;
}
char getInput() {

 int intA1 = analogRead(A6) - zeroX1;
  int intA2 = analogRead(A7) -zeroY1;

  if (intA1 > 300) {

    return 'u';
    
  }
  if (intA1 < -300) {

    return 'w';
    
  }
  if (intA2 > 300) {
 
    return 'd';
   
  }
  if (intA2 < -200) {

    return 'a';
    
  }
  return 'e';
}
void SetNewRandomShape()
{ // updates [current] with new shape
   randomSeed(analogRead(A0));
  Shape new_shape = CopyShape(ShapesArray[random(0, 6)]);

  new_shape.col = 2;
  new_shape.row = 0;
  DeleteShape(current);
  current = new_shape;
  if (!CheckPosition(current))
  {
    GameOn = FALSE;
  }
}

void RotateShape(Shape shape)
{ // rotates clockwise
  Shape temp = CopyShape(shape);
  int i, j, k, width;
  width = shape.width;
  for (i = 0; i < width; i++)
  {
    for (j = 0, k = width - 1; j < width; j++, k--)
    {
      shape.array[i][j] = temp.array[k][i];
    }
  }
  DeleteShape(temp);
}


void Tetris() {
  tetrisRunning = true;
  while (tetrisRunning) {
  randomSeed(analogRead(A3));
    score = 0;
    int c;
    SetNewRandomShape();
    PrintTable();


while(!GameOn) {

    delay(1);

    if (tickOver == 0) {
      splashScreen();
    }
    else {

      gameOver();
      tickOver--;
    }

if (getInput() != 'e') {
    GameOn = TRUE;
  }
  }
  
  while(GameOn){
    delay(1);
    modSpeed = (int)(baseSpeed / (1+((score/2)*0.001)));
    tickSleep++;
    c = getInput();

    if (tickSleep > modSpeed/1.5){
          if (c != 'e' ) {
      ManipulateCurrent(c);
      tickSleep = tickSleep + modSpeed/2;
    }

      tickSleep=0;
      ManipulateCurrent('s');
    };//time difference in microsec accuracy
  }

ResetGame();
  delay(1000);
  GameOn = FALSE;
  tetrisRunning = FALSE;
  }
}


void WriteToTable()
{
  int i, j;
  for (i = 0; i < current.width; i++)
  {
    for (j = 0; j < current.width; j++)
    {
      if (current.array[i][j])
        Table[current.row + i][current.col + j] = current.array[i][j];
    }
  }
}

void RemoveFullRowsAndUpdateScore()
{
  int i, j, sum, count = 0;
  for (i = 0; i < ROWS; i++)
  {
    sum = 0;
    for (j = 0; j < COLS; j++)
    {
      sum += Table[i][j];
    }
    if (sum == COLS)
    {
      count++;
      int l, k;
      for (k = i; k >= 1; k--)
        for (l = 0; l < COLS; l++)
          Table[k][l] = Table[k - 1][l];
      for (l = 0; l < COLS; l++)
        Table[k][l] = 0;
    }
  }
  score += 100 * count;
}

void PrintTable()
{
  char Buffer[ROWS][COLS] = {0};
  int i, j;
  for (i = 0; i < current.width; i++)
  {
    for (j = 0; j < current.width; j++)
    {
      if (current.array[i][j])
        Buffer[current.row + i][current.col + j] = current.array[i][j];
    }
  }

  for (i = 0; i < 32; i++)
  {
    for (j = 0; j < COLS; j++)
    {
      matrix.setLed((3 - (i / 8)), j, i % 8, (Table[i][j] + Buffer[i][j]));
    }
  }
}

void ManipulateCurrent(int action)
{
  Shape temp = CopyShape(current);
  switch (action)
  {
  case 's':
    temp.row++; // move down
    if (CheckPosition(temp))
      current.row++;
    else
    {
      WriteToTable();
      RemoveFullRowsAndUpdateScore();
      SetNewRandomShape();
    }
    delay(350);
    break;
  case 'd':
    temp.col++; // move right
    if (CheckPosition(temp))
      current.col++;
    break;
  case 'a':
    temp.col--; // move left
    if (CheckPosition(temp))
      current.col--;
    break;
  case 'w':
    RotateShape(temp); // rotate clockwise
    if (CheckPosition(temp))
      RotateShape(current);
      delay(40);
    break;
  case 'u':
    while (true)
    {
      temp.row = temp.row + 2;
      if (CheckPosition(temp))
      {
        current.row++;
        temp.row--;
      }
      else
      {
        ManipulateCurrent('s');
        break;
      }
    }
    break;
  }
  DeleteShape(temp);
  PrintTable();
}

void splashScreen()
{

  int left[] = {
      B01010100,
      B01010100,
      B01111100,
      B00000000,
      B01000000,
      B01111100,
      B01000000,
      B00000000,
  };
  int lmid[] = {
      B01101100,
      B01010000,
      B01111100,
      B00000000,
      B01000000,
      B01111100,
      B01000000,
      B00000000,
  };
  int rmid[] = {
      B01011100,
      B01010100,
      B01110100,
      B00000000,
      B00000000,
      B01111100,
      B00000000,
      B00000000,
  };
  int right[] = {
      B01001111,
      B01100000,
      B00101100,
      B00001100,
      B01000000,
      B01101100,
      B01000110,
      B00000000,
  };

  for (int rownum = 0; rownum < 8; rownum++)
  {
    matrix.setRow(0, rownum, right[rownum]);
    matrix.setRow(1, rownum, rmid[rownum]);
    matrix.setRow(2, rownum, lmid[rownum]);
    matrix.setRow(3, rownum, left[rownum]);
  }
}

void gameOver()
{
     matrix.clearDisplay(0);
    matrix.clearDisplay(1);
    matrix.clearDisplay(2);
    matrix.clearDisplay(3);

  int left[] = {
      B11111000,
      B10100000,
      B11111000,
      B00000000,
      B10111000,
      B10101000,
      B10001000,
      B11111000,
  };
  int lmid[] = {
      B01010100,
      B01010100,
      B01111100,
      B00000000,
      B01111100,
      B00100000,
      B00100000,
      B01111100,
  };
  int rmid[] = {
      B01111000,
      B00000100,
      B01111000,
      B00000000,
      B01111100,
      B01000100,
      B01000100,
      B01111100,
  };
  int right[] = {
      B01110100,
      B01011000,
      B01010000,
      B01111100,
      B00000000,
      B01010100,
      B01010100,
      B01111100,
  };

  for (int rownum = 0; rownum < 8; rownum++)
  {
    matrix.setRow(0, rownum, right[rownum]);
    matrix.setRow(1, rownum, rmid[rownum]);
    matrix.setRow(2, rownum, lmid[rownum]);
    matrix.setRow(3, rownum, left[rownum]);
  }
  delay(5000);
   matrix.clearDisplay(0);
    matrix.clearDisplay(1);
    matrix.clearDisplay(2);
    matrix.clearDisplay(3);
}

void ResetGame() {
    // Vymaže obsah herního pole
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            Table[i][j] = 0;
        }
    }

    // Nastaví počáteční hodnoty
    score = 0;
    GameOn = FALSE;
    tickOver = 0;
    baseSpeed = 600; // Nebo můžete nastavit libovolnou výchozí rychlost
    modSpeed = baseSpeed;
    matrix.clearDisplay(0);
    matrix.clearDisplay(1);
    matrix.clearDisplay(2);
    matrix.clearDisplay(3);
    gameOver();
}

