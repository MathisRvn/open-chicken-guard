/* Code By Mathis REVENU ! */


// Librairies
#include <Wire.h>
#include "RTClib.h"
#include <avr/wdt.h>


// Correspondent à aux heures minimum et maximum => Sécurité en plus pour éviter les disfonctionnements
// Entre ces deux heures, la porte sera fermé (si le mode automatique est enclenché) (DANS TOUS LES CAS)
#define LOCK_DAY_HOUR_MORNING 7*60 // 7h
#define LOCK_DAY_HOUR_EVENING 23*60 // 22h


// Définitions de variables globales
int lapse = 500;


// Fonction permettant de "mettre en veille" l'arduino durant un certain délai
void sleepLong (int current) {

  wdt_reset();

  while (current >= 7000) {
    delay(7000);
    current = current - 7000;
    wdt_reset();
  }
  wdt_reset();
  delay(current);
  wdt_reset();
  
}


// Fichiers de code externe
#include "time.h"
#include "memory.h"
#include "motor.h"
#include "bluetooth.h"
#include "serial.h"


void setup() {

  pinMode(BLUETOOTH_INTERRUPT_PIN, INPUT);
  pinMode(MOTOR_BTN_PIN, INPUT);
  servo.attach(11);
  
  Serial.begin(9600);
  Serial.println("Device starting");
  
  rtc.begin();

  wdt_enable(WDTO_8S);

  //resetParams(); // Réinitialise les paramètres enregistrées dans la mémoire morte
  
}


void loop() {

  // Permet de débuguer en affichant certaines variables (interrupteurs...)
  logTest(); 

  // Moteur par défaut arrété
  stopMotor();

  // On analyse la situation et on fait les actions nécessaires
  if (getMode()){ // Mode manuel enclenché
    
    if (getOpened()) {
      // La porte doit être ouverte
      openDoor();
    }else{
      // La porte doit être fermé  
      closeDoor();
    }
    
  } else { // mode automatique 

    if (getTime() > LOCK_DAY_HOUR_EVENING || getTime() < LOCK_DAY_HOUR_MORNING) { // On est entre les heures de sécurité
      closeDoor();
    }else if (getTime() > LOCK_DAY_HOUR_MORNING && getTime() < LOCK_DAY_HOUR_EVENING && getTime() > getOpenHour() && getTime() < getCloseHour()) {
      openDoor();
    }else{
      closeDoor();
    }
    
  }

  // On gère le blueooth
  if (digitalRead(BLUETOOTH_INTERRUPT_PIN)) {
    lapse = 500;
    if (!bluetooth_on) {
        bluetooth_on = true;
        bluetooth.begin(9600);
    }  
    checkBluetooth();
  }else{
    lapse = 20*1000; // 20 secondes
    bluetooth_on = false;
  }

  sleepLong(lapse);
  
}
