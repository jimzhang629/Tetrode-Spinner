#include <Stepper.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <LiquidCrystal.h>

int lcd_key = 0;
int adc_key_in = 0;
#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5

// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0); // read the value from the sensor
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 use this threshold
 if (adc_key_in < 50) return btnRIGHT;
 if (adc_key_in < 250) return btnUP;
 if (adc_key_in < 450) return btnDOWN;
 if (adc_key_in < 650) return btnLEFT;
 if (adc_key_in < 850) return btnSELECT;
 return btnNONE; // when all others fail, return this…
}

// Define LCD pins
LiquidCrystal lcd(8,9,4,5,6,7);


// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);
// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)

int steps_per_rev = 200; //how many steps it has
int rev_right = 0;
int rev_left = 0;

 
void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  
  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  myMotor->setSpeed(800);  // set rpm

  lcd.begin(16,2); // 16 columns, 2 rows
}

void loop() {
  lcd_key = read_LCD_buttons(); // read the buttons
  switch (lcd_key) // depending on which button was pushed, we perform an action
  {
    
  case btnRIGHT:
  {
  
  lcd.setCursor(0,1);
  lcd.print("RIGHT ");
  
  while(lcd_key != btnUP and lcd_key!= btnDOWN and lcd_key != btnLEFT){ // if btn isn't up, down, or left, then spin a revolution right
    myMotor->step(200, FORWARD, DOUBLE); //step the number of revolutions you want
    rev_right = rev_right + 1; // increment the counter
    lcd.setCursor(7,1);
    lcd.print(rev_right); 
    lcd_key = read_LCD_buttons(); //check for if the btn has changed
  }
  myMotor->release();// release all power from the motor 
  break;
  
  }
  
  case btnLEFT:
  {
  lcd.setCursor(0,0);
  lcd.print("LEFT ");
  while(lcd_key != btnUP and lcd_key != btnDOWN and lcd_key != btnRIGHT){ //same code but for left revolution
    myMotor->step(200, BACKWARD, DOUBLE);
    rev_left = rev_left + 1;
    lcd.setCursor(7,0);
    lcd.print(rev_left);
    lcd_key = read_LCD_buttons();
  }
  myMotor->release();
  break;
  }
  
}
}
