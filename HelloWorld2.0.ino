//Id like to clean it up a bit more. Get rid of the 'u' counter, its basically pointless, find  a way around the lag that affects the bullet movement when the aliens move.
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define p1Y 44
#define alienWidth 11 //assuming that the width of the alien is 10, pls confirm
#define alienHeight 6
#define trigPin 5
#define echoPin 6

static const unsigned char zero[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const unsigned char b[] = {0x00, 0x18, 0x18, 0x18, 0x00, 0x00};;
static const unsigned char p1[] = {0x80, 0x01, 0x80, 0x01, 0xd0, 0x0b, 0xf0, 0x0f};
static const unsigned char heHe[] = {0x00, 0x00, 0xe0, 0x0f, 0xf0, 0x1f, 0xe0, 0x0f, 0xc0, 0x06, 0x80, 0x02, 0x00, 0x00, 0x00, 0x00};
unsigned int alienCounter = 0;
float alienArrayX[30];
int alienArrayY[30];
unsigned int score = 0;
unsigned int k = 0;
unsigned int t = 0;
unsigned int z = 0;
unsigned int u = 0;
unsigned int cc = 0;
float speedX = -1.00;
float bSpeed = 2.00;
float p1X;
float speedIncrement = 0.10;
float distance = 200.00;
boolean changeSpeedXCondition;
boolean moveBulletCondition = false;
float bX = 0.00;
float bY = 48.00;
boolean gameOn = false;
boolean gameDone = false;
boolean gameStart = true;

U8G2_PCD8544_84X48_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);  // Nokia 5110 Display Declaration for Library

//************************************************ S E T U P ********************************************************************
void setup(void) {
  u8g2.setBitmapMode(0);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(7, INPUT);
  u8g2.begin();
  Serial.begin(9600);
  u8g2.clearBuffer();         // clear the internal memory

  for (int i = 0.00; i < 5.00; i++)
  {
    for (int j = 0.00; j < 6.00; j++)
    {
      //u8g2.drawBitmap(j * 13, i * 7, 11, 48, heHe);
      alienArrayX[k] = {(float)j * (float)13};
      alienArrayY[k] = {(float)i * (float)7};
      k++;
    }
  }        // sendBuffer() --- transfer internal memory to the display
}

//***************************************************** L O O P ***********************************************************************
void loop(void) {
 if (gameStart){startScreen(); rgbLED(0,255,0);}
  if (gameOn)
  {
    cc++;
    t++;
    moveAliens();
    movePlayer();
    if (moveBulletCondition == true) {
      moveBullet();
    }
  }
  checkUltrasonicSensor();
  if (gameDone)
  {
    restartGame();
  }
}

//*************************************************** M O V E   A L I E N S *********************************************************
void moveAliens(void)
{
  if (t == 15)
  {
    if (cc > 25)
    {
      rgbLED(0,0,255);
    }
    score++;
    alienCounter = 0;
    clearFunction();
    t = 0;
    for (int i = 0; i < k; i++)
    {
      if (alienArrayY[i] > -2) {
        alienArrayX[i] += (float)speedX;
      }
      else 
      {
        alienArrayX[i] = 50;
      }
      movePlayer();

      //u8g2.drawBitmap(alienArrayX[i], alienArrayY[i], 11, 48, heHe);
      u8g2.drawXBM(alienArrayX[i], alienArrayY[i], 16, 8, heHe);
      //      if (speedX < 0)
      //      {
      //      u8g2.drawXBM(alienArrayX[i] - (speedX + (alienWidth + 4)), alienArrayY[i], 2, 8, zero);
      //      }
      //      else
      //      {
      //        u8g2.drawXBM(alienArrayX[i] + 18, alienArrayY[i], 2, 8, zero);
      //      }


      if (moveBulletCondition == true) {
        moveBullet();
      }
      if (alienArrayX[i] > 71.00 || alienArrayX[i] < -3.00 )
      {
        Serial.print("X: ");
        Serial.println(alienArrayX[i]);
        changeSpeedXCondition = true;
      }
      if (alienArrayY[i] > 36)
      {
        gameOn = false;
        gameDone = true;
        gameOver();
      }
      if (alienArrayY[i] < 0)
      {
        alienCounter ++;
      }
    }
    if (alienCounter == k)
    {
      gameOn = false;
      newLevel();
    }
    if (changeSpeedXCondition == true)
    {
      changeSpeedXCondition = false;
      
        speedX *= -1.00;
        //Serial.println("speedX *= -1: "+String(speedX));
        if ((speedX < 7 && speedX > 0) || (speedX < 0 && speedX > -7))
      {
        speedIncrement *= -1.00;
        //Serial.println("speedIncrement *= -1: "+String(speedIncrement));
        speedX -= (float)speedIncrement;
        Serial.println("new speedX: "+String(speedX));
      }

      for (int j = 0; j < k; j++)
      {
        if (speedX > 2)
        {
         alienArrayX[j] += (float)speedX;
        }
        if (alienArrayY[j] > -2) {
          alienArrayY[j] += 1;
        }
        //Serial.println("speedX before alienArrayX[j] += speedX * 2: "+String(alienArrayX[j]));
        if (speedX > 3.00 || speedX < -3.00)
        {
          alienArrayX[j] += (float)speedX;
        }
        //Serial.println("speedX after alienArrayX[j] += speedX * 2: "+String(alienArrayX[j]));
      }
    }
  }
}

//************************************************* M O V E   P L A Y E R *************************************************************
void movePlayer(void)
{
  
  p1X = (float)analogRead(A0) / 655.00 * 76.00 - 10.00;
  //u8g2.drawBitmap(p1X, 37, 11, 48, p1);
  u8g2.drawXBM(p1X, p1Y, 16, 4, p1);
  //clear random static pixels (did this so that the char array for player1 would not be filled with 0x00s because it takes up too much space)
  // this part of the function seems to take up large processing power and is slowing down the frame rate. try to eliminate this by freeing up some space to include all of the 0x00s in the players bitmap char array or find another way to get rid of the static
  /* for (int j = p1Y; j < p1Y + 5; j++)
    {
    for (int i = p1X + 5; i < 85; i++)
    {
    u8g2.drawBitmap(i, j, 1, 1, h);
    }
    }
    for (int j = p1Y + 5; j < 49; j++)
    {
    for (int i = 0; i < 85; i++)
    {
    u8g2.drawBitmap(i, j, 1, 1, h);
    }
    }*/
  u8g2.sendBuffer();
  // part of the function that determines shooting the bullet
  if (distance < 100 && moveBulletCondition == false)
  {
    bX = (float)p1X + (float)4;
    bY = (float)p1Y - (float)5;
    moveBulletCondition = true;
    bSpeed = 1.00;
    u = 11;
    Serial.print("hi");
  }
}


//********************************************** M O V E   B U L L E T ******************************************************************
void moveBullet(void)
{
  u++;
  if (bY < -4) {
    moveBulletCondition = false;
  }
  else if (u > 10 && moveBulletCondition == true)
  {
    u = 0;
    bY -= (float)bSpeed;
    u8g2.drawXBM (bX, bY, 8, 6, b);
    u8g2.sendBuffer();
    int i = 0;
    while (i < k)
    {
      if (bY + 2 <= alienArrayY[i] + alienHeight && bY + 2 >= alienArrayY[i] && bX + 2 >= alienArrayX[i] && bX + 2 <= alienArrayX[i] + alienWidth) // to see if the bullet is situated within an alien, or in other words if the bullet has hit an alien
      {
        cleanUp(alienArrayY[i], alienArrayX[i], bY, bX);
        moveBulletCondition = false;
        bSpeed = 0.00;
        bY = 50.00;
        bX = 100.00;
        alienArrayY[i] = -10;
        alienArrayX[i] = 50;//this is to move the hit alien off of the screen, however, its location is still being calculated everysecond taking up processing space, try to find aqnother solution to the problem
        //trigger explosion tingz here
        break;
      }
      i++;
    }
  }
}

//***************************************** C H E C K   U L T R A S O N I C   S E N S O R **************************************************
void checkUltrasonicSensor(void)
{
  z++;
  digitalWrite(trigPin, LOW);
  if (z >= 3 && z < 15)
  {
    digitalWrite(trigPin, HIGH);
  }
  else if (z == 15)
  {
    digitalWrite(trigPin, LOW);
    distance = pulseIn(echoPin, HIGH) * 0.034 / 2;
    Serial.println(distance);
    z = 0;
  }

}

void cleanUp(int alienY, int alienX, int bulletY, float bulletX)
{
  rgbLED(255,0,0);
  u8g2.drawXBM(alienX, alienY, 16, 6, zero);
  u8g2.drawXBM(bulletX, bulletY, 8, 6, zero);
}

void clearFunction(void)
{
  u8g2.clear();
  u8g2.drawXBM(p1X, p1Y, 16, 4, p1);
  int i = 0;
  do 
  {
    u8g2.drawXBM(alienArrayX[i], alienArrayY[i], 16, 8, heHe);
    i++;
  } while (i < k)
  u8g2.sendBuffer();
}
void gameOver(void)
{ 
  gameOn = false;
  bY = 48.00;
  bX = 0.00;
  u8g2.clear();
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(0, 10, "Game Over!"); // write something to the internal memory
  u8g2.setFont(u8g2_font_synchronizer_nbp_tf);
  u8g2.drawStr(0,22,"Final Score: ");
  u8g2.setFont(u8g2_font_profont10_tn);
  u8g2.setCursor(0,30);
  u8g2.print(score);
  u8g2.sendBuffer();          // transfer internal memory to the display
  gameDone = true;
  delay(1000);

}

void restartGame(void)
{
  if (distance < 20.00 && distance > 0.00)
  {
    u8g2.clear();
    u8g2.clearBuffer();
    int v = 0;
    gameDone = false;
    gameOn = true;
    t = 0;
    z = 0;
    u = 0;
    speedX = -1.00;
    bSpeed = 2.00;
    speedIncrement = 0.20;
    distance = 200.00;
    boolean moveBulletCondition = false;
    float bX = 0.00;
    float bY = 48.00;
    for (int i = 0.00; i < 5.00; i++)
    {
      for (int j = 0.00; j < 6.00; j++)
      {
        alienArrayX[v] = {(float)j * (float)13};
        alienArrayY[v] = {(float)i * (float)7};
        v++;
      }
    }
  }
}
void newLevel(void)
{
  rgbLED(0,255,0);
  score += 70;
  gameOn = true;
  int v = 0;
  for (int i = 0.00; i < 5.00; i++)
  {
    for (int j = 0.00; j < 6.00; j++)
    {
      alienArrayX[v] = {(float)j * (float)13};
      alienArrayY[v] = {(float)i * (float)7};
      v++;
    }
  }
}

void startScreen(void)
{
  u8g2.setFont(u8g2_font_courB08_tf);
  u8g2.drawStr(0, 10, "Space Invaders");
  u8g2.setFont(u8g2_font_inb16_mr);
  u8g2.drawStr(0, 35, "W. K");
  u8g2.sendBuffer();
   if (distance < 20.00 && distance > 0.00)
  {
    rgbLED(0,0,255);
    gameStart = false;
    gameOn = true;
    u8g2.clear();
    u8g2.sendBuffer();
  }
}
void rgbLED(int red, int green, int blue)
{
    cc = 0;
  analogWrite(15, red);
  analogWrite(16, green);
  analogWrite(17, blue);
  
}

