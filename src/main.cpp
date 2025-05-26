#include <Arduino.h>
#include "BleKeyboard.h"
#include "Button.h"

#define LED 8

#define BUTTON_1_PIN GPIO_NUM_5
#define BUTTON_2_PIN GPIO_NUM_6
#define BUTTON_3_PIN GPIO_NUM_7
#define BUTTON_4_PIN GPIO_NUM_3
#define BUTTON_UP_PIN GPIO_NUM_9
#define BUTTON_DOWN_PIN GPIO_NUM_10

BleKeyboard bleKeyboard;

void onButtonUpPressCb(void *button_handle, void *usr_data) {
    Serial.println("Button up button pressed");
    bleKeyboard.press(KEY_MEDIA_VOLUME_UP);
}

void onButtonUpReleaseCb(void *button_handle, void *usr_data) {
    Serial.println("Button up button released");
    bleKeyboard.release(KEY_MEDIA_VOLUME_UP);
}

void onButtonDownPressCb(void *button_handle, void *usr_data) {
    Serial.println("Button down button pressed");
    bleKeyboard.press(KEY_MEDIA_VOLUME_DOWN);
}

void onButtonDownReleaseCb(void *button_handle, void *usr_data) {
    Serial.println("Button down button released");
    bleKeyboard.release(KEY_MEDIA_VOLUME_DOWN);
}

void onButton2PressCb(void *button_handle, void *usr_data) {
    Serial.println("Button 2 / Next track button pressed");
    bleKeyboard.press(KEY_MEDIA_NEXT_TRACK);
}

void onButton2ReleaseCb(void *button_handle, void *usr_data) {
    Serial.println("Button 2 / Next track button released");
    bleKeyboard.release(KEY_MEDIA_NEXT_TRACK);
}

void onButton1PressCb(void *button_handle, void *usr_data) {
    Serial.println("Button 1 / Previous track button pressed");
    bleKeyboard.press(KEY_MEDIA_PREVIOUS_TRACK);
}

void onButton1ReleaseCb(void *button_handle, void *usr_data) {
    Serial.println("Button 1 / Previous track button released");
    bleKeyboard.release(KEY_MEDIA_PREVIOUS_TRACK);
}

void onButton3PressCb(void *button_handle, void *usr_data) {
    Serial.println("Button 3 pressed");
    bleKeyboard.write(KEY_MEDIA_STOP);
}

void onButton3DoubleCb(void *button_handle, void *usr_data) {
    Serial.println("Button 3 double click");
    bleKeyboard.write(KEY_NUM_0);
}

void onButton3LongCb(void *button_handle, void *usr_data) {
    Serial.println("Button 3 long press");
    bleKeyboard.write(KEY_NUM_1);
}


void onButton4PressCb(void *button_handle, void *usr_data) {
    Serial.println("Button 4 pressed");
    bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
}

void onButton4DoubleCb(void *button_handle, void *usr_data) {
    Serial.println("Button 4 double click");
    bleKeyboard.write(KEY_NUM_2);

}

void onButton4LongCb(void *button_handle, void *usr_data) {
    Serial.println("Button 4 long press");
    bleKeyboard.write(KEY_NUM_3);

}

Button* initButtonDownUp(gpio_num_t pin, callbackFunction pressCb, callbackFunction releaseCb) {
  Button *button = new Button(pin, false);
  button->attachPressDownEventCb(pressCb, NULL);
  button->attachPressUpEventCb(releaseCb, NULL);
  return button;
}

void initDefaultButtons() {
  initButtonDownUp(BUTTON_1_PIN, &onButton1PressCb, &onButton1ReleaseCb);
  initButtonDownUp(BUTTON_2_PIN, &onButton2PressCb, &onButton2ReleaseCb);
  initButtonDownUp(BUTTON_UP_PIN, &onButtonUpPressCb, &onButtonUpReleaseCb);
  initButtonDownUp(BUTTON_DOWN_PIN, &onButtonDownPressCb, &onButtonDownReleaseCb);

  Button* button3 = new Button(BUTTON_3_PIN, false);
  button3->attachSingleClickEventCb(&onButton3PressCb, NULL);
  button3->attachDoubleClickEventCb(&onButton3DoubleCb, NULL);
  button3->attachLongPressStartEventCb(&onButton3LongCb, NULL);

  Button* button4 = new Button(BUTTON_4_PIN, false);
  button4->attachSingleClickEventCb(&onButton4PressCb, NULL);
  button4->attachDoubleClickEventCb(&onButton4DoubleCb, NULL);
  button4->attachLongPressStartEventCb(&onButton4LongCb, NULL);
}

void setup() {
  pinMode(LED,OUTPUT);
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleKeyboard.setName("RallyRemote");
  bleKeyboard.begin();
  initDefaultButtons();
}


void loop() {
  if (!bleKeyboard.isConnected()) {
      Serial.println("Waiting for connection...");
      digitalWrite(LED,LOW);
      delay(500);
      digitalWrite(LED,HIGH);
  } else {
    digitalWrite(LED,LOW);
    delay(500);
  }
  delay(500);
}

