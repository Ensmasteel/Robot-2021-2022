// This sketch code is based on the RPLIDAR driver library provided by RoboPeak
#include <RPLidar.h>
#include <Arduino.h>
#include <LIDAR.h>


LIDAR lid;


void setup() {
  Serial.begin(115200);
  delay(100);
  analogWrite(3, 255);
  lid.init_com(Serial);
  lid.begin(Serial1);
  lid.init(Serial1);

  
}

void loop() {
  
  for(int i = 0;i < 500;i++){
    lid.scan();
  };
  lid.filter_objects();
  lid.calcul_speed();
  //lid.show_objects_filtered();
  //lid.plot();
  lid.plot_biggest();
  lid.plot_closest();
}