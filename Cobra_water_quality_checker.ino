#include <OneWire.h>
#include <LiquidCrystal.h>

#define TEMP_SENSOR_PIN A0 // 18B20 temperature sensor connected to analog pin A0
#define WATER_SENSOR_PIN A1 // Water sensor connected to analog pin A1
#define TURBIDITY_SENSOR_PIN A2 // Turbidity sensor connected to analog pin A2

float calibration_value = 21.34;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;

// Initialize the OneWire library for the temperature sensor
OneWire oneWire(TEMP_SENSOR_PIN);

// Initialize the LiquidCrystal library for the LCD screen
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

void setup() {
  // Set up the LCD screen
  lcd.begin(16, 2);
  
  // Display the initial message on the LCD screen
  lcd.print("Temp:   Water:  ");
  lcd.setCursor(0, 1);
  lcd.print("Turbidity:       ");
}

void loop() {
  // Read the temperature from the 18B20 sensor
  float tempC;
  byte data[12];
  oneWire.reset();
  oneWire.write(0x55, 1);
  oneWire.read_bytes(data, 12);
  tempC = ((data[1] << 8) | data[0]) * 0.0625;
  
  // Read the water sensor value
  int waterValue = analogRead(WATER_SENSOR_PIN);
  
  // Read the turbidity sensor value
  int turbidityValue = analogRead(TURBIDITY_SENSOR_PIN);

  // Reading the Ph value
  for(int i=0;i<10;i++) 
 { 
 buffer_arr[i]=analogRead(A0);
 delay(30);
 }
 for(int i=0;i<9;i++)
 {
 for(int j=i+1;j<10;j++)
 {
 if(buffer_arr[i]>buffer_arr[j])
 {
 temp=buffer_arr[i];
 buffer_arr[i]=buffer_arr[j];
 buffer_arr[j]=temp;
 }
 }
 }
 avgval=0;
 for(int i=2;i<8;i++)
 avgval+=buffer_arr[i];
 float volt=(float)avgval*5.0/1024/6;
 float ph_act = -5.70 * volt + calibration_value;
 
  // Display the readings on the LCD screen
  lcd.setCursor(5, 0);
  lcd.print(tempC);
  lcd.print(" C ");
  lcd.setCursor(12, 0);
  lcd.print(waterValue);
  lcd.setCursor(12, 1);
  lcd.print(turbidityValue);

  lcd.print("pH Val:");
  lcd.print(ph_act);
  
  delay(1000); // Delay for one second before taking the next reading
}
