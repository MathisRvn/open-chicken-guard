void logTest() {
  
  Serial.print("------------------------------------- LOG INFOS -------------------------------------");
  Serial.println();
  
  logMemory();
  
  Serial.println();
  Serial.print("Time : ");
  Serial.print(getTime());
  Serial.print(" - ");
  Serial.print(getTime() / 60);
  Serial.print("h");
  Serial.print(getTime() % 60);
  Serial.println();
  
  Serial.print("Bluetooth : ");
  Serial.print(digitalRead(BLUETOOTH_INTERRUPT_PIN));
  Serial.println();
  
  Serial.print("Motor btn : ");
  Serial.print(digitalRead(MOTOR_BTN_PIN));
  Serial.println();
  
}
