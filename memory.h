#include "EEPROM.h"

void writeInt(int adresse, int val) 
{   
    //découpage de la variable val qui contient la valeur à sauvegarder en mémoire
    unsigned char faible = val & 0x00FF; //récupère les 8 bits de droite (poids faible) -> 0010 1100 
    //calcul : 1101 0111 0010 1100 & 0000 0000 1111 1111 = 0010 1100

    unsigned char fort = (val >> 8) & 0x00FF;  //décale puis récupère les 8 bits de gauche (poids fort) -> 1101 0111
    //calcul : 1101 0111 0010 1100 >> 8 = 0000 0000 1101 0111 puis le même & qu’avant

    //puis on enregistre les deux variables obtenues en mémoire
    EEPROM.write(adresse, fort) ; //on écrit les bits de poids fort en premier
    EEPROM.write(adresse+1, faible) ; //puis on écrit les bits de poids faible à la case suivante
}

int readInt(int adresse)
{
    int val = 0 ; //variable de type int, vide, qui va contenir le résultat de la lecture

    unsigned char fort = EEPROM.read(adresse);     //récupère les 8 bits de gauche (poids fort) -> 1101 0111
    unsigned char faible = EEPROM.read(adresse+1); //récupère les 8 bits de droite (poids faible) -> 0010 1100

    //assemblage des deux variable précédentes
    val = fort ;         // val vaut alors 0000 0000 1101 0111
    val = val << 8 ;     // val vaut maintenant 1101 0111 0000 0000 (décalage)
    val = val | faible ; // utilisation du masque
    // calcul : 1101 0111 0000 0000 | 0010 1100 = 1101 0111 0010 1100

    return val ; //on n’oublie pas de retourner la valeur lue !
}

/*
Enregistrement des données de configuration

600 -> mode (auto or manual) auto = false manual = true
601 -> opened (lock or open) if mode == manual
604 -> openHour      heure*60 + minutes (=minutes depuis minuit)
610 -> closeHour
616 -> MotorCloseDelay

*/

// Setters
void writeMode (bool mode) { EEPROM.write(600, (unsigned char)mode); }
void writeOpened (bool opened) { EEPROM.write(601, (unsigned char)opened); }
void writeOpenHour (int hour) { writeInt(604,hour); }
void writeCloseHour (int closeHour) { writeInt(610,closeHour); }
void writeMotorCloseDelay (int MotorOpenDelay) { writeInt(616,MotorOpenDelay); }


// Getters
bool getMode () { return EEPROM.read(600); }
bool getOpened () { return EEPROM.read(601); }
int getOpenHour () { return readInt(604); }
int getCloseHour () { return readInt(610); }
int getMotorCloseDelay () { return readInt(616); }


void logMemory () {
  // On affiche ces variables
  Serial.println(getMode());
  Serial.println(getOpened());
  Serial.println(getOpenHour());
  Serial.println(getCloseHour());
  Serial.println(getMotorCloseDelay());
}

void resetParams () {
  // On définit les différentes varialbes à enregistrer
  
  bool mode = false;
  bool opened = true;
  int openHour = 8*60;
  int closeHour = 20*60;
  int motorCloseDelay = 8500;

  // On écit ces variables
  
  writeMode(mode);
  writeOpened(opened);
  writeOpenHour(openHour);
  writeCloseHour(closeHour);
  writeMotorCloseDelay(motorCloseDelay);

}
