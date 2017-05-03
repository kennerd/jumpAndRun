#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR      0x3F
#define BACKLIGHT_PIN 3
#define En_pin        2
#define Rw_pin        1
#define Rs_pin        0
#define D4_pin        4
#define D5_pin        5
#define D6_pin        6
#define D7_pin        7

char screenContent[2][16];
long debouncing_time = 15;
volatile unsigned long last_micros;
boolean showedOnce = false;
boolean isCorrectJump = false;
int levelTimeOut = 1000;
int level = 0;
int score = 0;

LiquidCrystal_I2C	lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin, BACKLIGHT_PIN, POSITIVE);

void jump() {
  if (screenContent[1][3] == 'M') {
    isCorrectJump = true;
  }
}

void debounceInterrupt() {
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
    jump();
    last_micros = micros();
  }
}

void setup() {
  attachInterrupt(0, debounceInterrupt, RISING); 
  lcd.begin (16,2);
  lcd.home();
  lcd.setBacklight(HIGH);
}

void world2Display() {
  for (int rowIdx = 0; rowIdx < 2; rowIdx++){
    for (int colIdx = 0; colIdx < 16; colIdx++){
      /* Stop flickering of level and score number */
      if(((rowIdx == 0) && ((colIdx == 0) ||
                            (colIdx == 2) || 
                            (colIdx == 12) || 
                            (colIdx == 13) ||
                            (colIdx == 14) || 
                            (colIdx == 15)))) continue;
      if (((colIdx == 2)) && ((screenContent[1][2] == ' '))) continue;
      lcd.setCursor(colIdx, rowIdx);
      lcd.print(screenContent[rowIdx][colIdx]);
    }
  }
}

void greeting() {
  //lcd.autoscroll();
  lcd.setCursor(0,0);
  lcd.print("Hello my Friend");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("This is the jump");
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("      GAME!     ");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Press the Button");
  delay(3000);
  lcd.setCursor(0,1);
  lcd.print("to avoid running");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("into enemies.");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("   Good Luck!   ");
  delay(3000);
}

void countDown() {
  for (int countDown = 5; countDown >= 0; countDown--) {
    lcd.clear();
    lcd.setCursor(6,0);
    if (countDown == 0) {
      lcd.print("GO!");
    } else {
      lcd.print('*');
      lcd.print(countDown);
      lcd.print('*');
    }
    delay(1000);
  } 
}

void initWorld() {
  memcpy(&screenContent[0], "                ", 16*sizeof(char));
  memcpy(&screenContent[1], "               M", 16*sizeof(char)); 
}

void copyScore2Display() {
  lcd.setCursor(12,0);
  lcd.print(score);
}

void increaseScore() {
  score++;
  copyScore2Display();
}

void copyLevel2Display() {
  lcd.setCursor(0,0);
  lcd.print(level);
}

void increaseLevel() {
  level++;
  copyLevel2Display();
}

void checkIfWon() {
  if (level == 9) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("* YOU HAVE WON *");
    lcd.setCursor(0,1);
    delay(1000);
    lcd.print("*  WOOP WOOP!  *");
    while (1) {} /* Game ends here */
  }
}

void spawnEnemy() {
  //if (random(0,21) != 20) {
    screenContent[1][15] = 'M';
  //} 
}

void printPlayer() {
  if (isCorrectJump) {
    lcd.setCursor(2,0);
    lcd.print('L');
    lcd.setCursor(2,1);
    lcd.print(' ');
    if((screenContent[1][3] == ' ') && (screenContent[1][2] == ' ')) {
      screenContent[1][1] = ' ';
      isCorrectJump = false;
    }
  } else {
    lcd.setCursor(2,0);
    lcd.print(' ');
    lcd.setCursor(2,1);
    lcd.print('L');
    if((screenContent[1][3] == ' ') && (screenContent[1][2] == ' ')) {
      screenContent[1][1] = ' ';
    }
  }
  
}

void advanceWorld() {
  char temp;
  for (int pos = 0; pos < 15; pos++){
    temp = screenContent[1][pos+1];
    screenContent[1][pos+1] = screenContent[1][pos];
    screenContent[1][pos] = temp;
  }
}

void autoJump() {
  if (screenContent[1][3] == 'M') {
    isCorrectJump = true;
  }
}

void checkIfDead() {
  if((screenContent[1][2] == 'M') && !isCorrectJump) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("*  YOU LOOSE   *");
    lcd.setCursor(0,1);
    delay(1000);
    lcd.print("*              *");
    while (1) {} /* Game ends here */
  }
}


void loop() {
  /* RUN ONCE */
  if (!showedOnce) {
    showedOnce = true;
    greeting();
    countDown();
    initWorld();
    copyLevel2Display();
  }
  /* ======== */
  

  levelTimeOut = levelTimeOut - 100;
  if((levelTimeOut % 20) == 0) {
    increaseScore();
  }
  
  if (levelTimeOut == 0) {
    increaseLevel();
    levelTimeOut = 1000;
  }
  
  if ((levelTimeOut % 37) == 0) {
    spawnEnemy();
  }
  
  //autoJump();

  world2Display();
  printPlayer();
  
  checkIfWon();
  checkIfDead();
  
  if ((levelTimeOut % 10) == 0) {
    advanceWorld();
  }

}

