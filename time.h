// Définition de l'horloge temps réel
RTC_DS3231 rtc; // rtc pour real time clock
DateTime now;

void updateTime (int t) {
  // January 21, 2014 at 3am you would call:
  now = rtc.now();
  rtc.adjust(DateTime(now.year(), now.month(), now.day(), (int)(t / 60), t % 60, 0));  
}

// Quelques fonctions de bases
int getTime () { 
  now = rtc.now();
  return (now.hour() * 60) + now.minute();
}
