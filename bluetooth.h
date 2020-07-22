#include <SoftwareSerial.h>

#define BLUETOOTH_INTERRUPT_PIN 13

bool bluetooth_on = false;
 
SoftwareSerial bluetooth(4, 5); // RX | TX

void checkBluetooth () {
  
  String bluetoothMsg = "";
  while (bluetooth.available())
  {
    delay(3);
    char c = bluetooth.read();
    bluetoothMsg += c;
  }
  if (bluetoothMsg.length() > 0) {
    Serial.println(bluetoothMsg);
    String elems[11] = {"", "", "", "", "", "", "", "", "", "", ""};

    int arrIndex = 0;
    for (int i = 0; i < bluetoothMsg.length(); i++) {
      if (bluetoothMsg[i] == ';') {
        arrIndex += 1;
      }else{
        elems[arrIndex] += bluetoothMsg[i];
      }
    }

    // On a recu un message

    /*
     Différents types de messages

      -> newconnection
      <- c;auto_or_manual;open_or_close;open_hour;close_hour;close_motor_delay

      -> newparams;auto_or_manual;open_or_close;open_hour;close_hour;close_motor_delay
      <- Paramètres enregistré

      -> resetparams
      <- c;auto_or_manual;open_or_close;open_hour;close_hour;close_motor_delay

      -> testclosemotor;close_motor_delay
      <- 
     
    */
    Serial.println(bluetoothMsg);
    if (elems[0] == "newconnection") {
      
      String str = "";
      
      str += "c;";
      
      str += (String)getMode();
      str += ';';
      
      str += (String)getOpened();
      str += ';';

      str += (String)getOpenHour();
      str += ';';

      str += (String)getCloseHour();
      str += ';';

      str += (String)getMotorCloseDelay();
      
      
      Serial.println(str);
      bluetooth.print(str);
      
    } else if (elems[0] == "newparams") {

      if (elems[1] == "true") {
        writeMode(true);  
      }else{
        writeMode(false);
      }

      if (elems[2] == "true") {
        writeOpened(true);  
      }else{
        writeOpened(false);
      }

      if (elems[4] != "0") {writeOpenHour(atoi(elems[3].c_str()));}
      
      if (elems[7] != "0") {writeCloseHour(atoi(elems[4].c_str()));}

      writeMotorCloseDelay(atoi(elems[5].c_str()));
      
      bluetooth.print("Paramètres enregistrés");

    } else if (elems[0] == "resetparams") {
      resetParams();
      
      String str = "";
      
      str += "c;";
      
      str += (String)getMode();
      str += ';';
      
      str += (String)getOpened();
      str += ';';

      str += (String)getOpenHour();
      str += ';';

      str += (String)getCloseHour();
      str += ';';

      str += (String)getMotorCloseDelay();
      
      
      Serial.println(str);
      bluetooth.print(str);

    } else if (elems[0] == "testclosemotor") {
      openDoor();
      writeMotorCloseDelay(atoi(elems[1].c_str()));
      closeDoor();
    }
    
    
  }

 
  // Permet la configuration du module hc-06 depuis le port série avec les commandes AT
  if (Serial.available())
    bluetooth.write(Serial.read());
 
}
