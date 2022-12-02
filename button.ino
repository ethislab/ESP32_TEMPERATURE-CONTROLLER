/*void attachAllButtonInterrupts() {
  //buttons
  if (MODE_BUTTON_PIN > 0) {
    #ifdef BUTTON
      pinMode(MODE_BUTTON_PIN,INPUT_RESISTOR);
      attachInterrupt(digitalPinToInterrupt(MODE_BUTTON_PIN), modeButtonISR, INTERRUPT_MODE);
    #endif
  }
  if (BACK_BUTTON_PIN > 0) {
    #ifdef BUTTON
      pinMode(BACK_BUTTON_PIN,INPUT_RESISTOR);
      attachInterrupt(digitalPinToInterrupt(BACK_BUTTON_PIN), backButtonISR, INTERRUPT_MODE);
    #endif
  }
  if (UP_BUTTON_PIN > 0) {
    #ifdef BUTTON
      pinMode(UP_BUTTON_PIN,INPUT_RESISTOR);
      attachInterrupt(digitalPinToInterrupt(UP_BUTTON_PIN), upButtonISR, INTERRUPT_MODE);
    #endif
  }
  if (DOWN_BUTTON_PIN > 0) {
    #ifdef BUTTON
      pinMode(DOWN_BUTTON_PIN,INPUT_RESISTOR);
      attachInterrupt(digitalPinToInterrupt(DOWN_BUTTON_PIN), downButtonISR, INTERRUPT_MODE);
    #endif
  }


  
}*/
