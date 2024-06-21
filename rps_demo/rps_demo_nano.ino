//author: Deng, Xiang 2024 June
//Demo code of tendon driven robotic hand paper (IEEE ROMAN 2024)
//It listens to the prediction from DNN classification of human hand gesture and send motor control commands to the robotic hand to perform rock scissors paper in response.
//Credits: firmware lib and example motor code, see Waveshare's lib https://files.waveshare.com/upload/d/d8/ST_Servo.zip
//Tested MCU: ESP 32; motors used: Waveshare ST3025
//Instructions: 1. install ESP board in your aduino IDE, example reference: https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/
// 2. add the firmware library of motors from Waveshare to Arduino IDE.
// Or if the installation is not successful, see the firmware here (forked) https://github.com/dengxianga/Servo-Driver-with-ESP32.git
// 3. Upload the program to MCU and run it.
#include <SCServo.h>

SMS_STS st;

#define S_RXD 18 // uart pins to communicate with the motor driver board
#define S_TXD 19
//motor positions for extension, in the order of: 1. motor for index-middle fingers, 2. motor for the other three fingers.
// 2047 is the center position of the motor (ideally), it might have some mechanical offset.
int p0s[2] = {2047 + (int)2047 * (0.11 + 0.0662 + 0.035) / PI, 2047 - (int)2047 * (0.11 + 0.0662) / PI};
//motor positoins for flexion
int p1s[2] = {280 + p0s[0], -260 + p0s[1]};
void rock() {
  st.WritePosEx(1, p1s[0]);//flex
  st.WritePosEx(2, p1s[1]);//flex
}
void paper() {
  st.WritePosEx(1, p0s[0]);//extend
  st.WritePosEx(2, p0s[1]);//extend
}
void scissor() {
  st.WritePosEx(1, p0s[0]);//extend
  st.WritePosEx(2, p1s[1]);//flex
}

void clearSerialbuffer(){
  while (Serial.available()>0){
    Serial.read();
  }
}
void setup()
{
  Serial.begin(115200);
  Serial1.begin(1000000, SERIAL_8N1, S_RXD, S_TXD); // serial port to motor driver board
  st.pSerial = &Serial1;
  delay(1000);
  paper();
  delay(1000);
}
void loop()
{
  if (Serial.available() > 0) {
    int cmd = Serial.read() - '0';
    if (cmd == 1) {
      paper();
    }
    else if (cmd == 2) {
      scissor();
    }
    else if (cmd == 3) {
      rock();
    }
  }
}
