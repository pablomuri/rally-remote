#include <Arduino.h>
#include <Button.h>
#include <ElegantOTA.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <BleGamepad.h>

#define LED 8

#define BUTTON_1_PIN GPIO_NUM_5
#define BUTTON_2_PIN GPIO_NUM_6
#define BUTTON_3_PIN GPIO_NUM_7
#define BUTTON_4_PIN GPIO_NUM_3
#define BUTTON_UP_PIN GPIO_NUM_9
#define BUTTON_DOWN_PIN GPIO_NUM_10

const char* ssid = "RallyOTA";

WebServer server(80);

BleGamepad bleGamepad("RallyRemote");

void pressGamepadButton(uint8_t button) {
    bleGamepad.press(button);
    bleGamepad.sendReport();
}

void releaseGamepadButton(uint8_t button) {
    bleGamepad.release(button);
    bleGamepad.sendReport();
}

void pressAndReleaseGamepadButton(uint8_t button, uint16_t delayMs = 100) {
    bleGamepad.press(button);
    bleGamepad.sendReport();
    delay(delayMs);
    bleGamepad.release(button);
    bleGamepad.sendReport();
}

static void onButtonUpPressCb(void *button_handle, void *usr_data) {
    Serial.println("Button up pressed - Gamepad BUTTON_9");
    pressGamepadButton(BUTTON_9);
}

static void onButtonUpReleaseCb(void *button_handle, void *usr_data) {
    Serial.println("Button up released - Gamepad BUTTON_9");
    releaseGamepadButton(BUTTON_9);
}

static void onButtonDownPressCb(void *button_handle, void *usr_data) {
    Serial.println("Button down pressed - Gamepad BUTTON_10");
    pressGamepadButton(BUTTON_10);
}

static void onButtonDownReleaseCb(void *button_handle, void *usr_data) {
    Serial.println("Button down released - Gamepad BUTTON_10");
    releaseGamepadButton(BUTTON_10);
}

static void onButton2PressCb(void *button_handle, void *usr_data) {
    Serial.println("Button 2 pressed - Gamepad BUTTON_6");
    pressGamepadButton(BUTTON_6);
}

static void onButton2ReleaseCb(void *button_handle, void *usr_data) {
    Serial.println("Button 2 released - Gamepad BUTTON_6");
    releaseGamepadButton(BUTTON_6);
}

static void onButton1PressCb(void *button_handle, void *usr_data) {
    Serial.println("Button 1 pressed - Gamepad BUTTON_7");
    pressGamepadButton(BUTTON_7);
}

static void onButton1ReleaseCb(void *button_handle, void *usr_data) {
    Serial.println("Button 1 released - Gamepad BUTTON_7");
    releaseGamepadButton(BUTTON_7);
}

static void onButton3PressCb(void *button_handle, void *usr_data) {
    Serial.println("Button 3 pressed - Gamepad BUTTON_1");
    pressAndReleaseGamepadButton(BUTTON_1);
}

static void onButton3DoubleCb(void *button_handle, void *usr_data) {
    Serial.println("Button 3 double click - Gamepad BUTTON_2");
    pressAndReleaseGamepadButton(BUTTON_2);
}

static void onButton3LongCb(void *button_handle, void *usr_data) {
    Serial.println("Button 3 long press - Gamepad BUTTON_3");
    pressAndReleaseGamepadButton(BUTTON_3);
}

static void onButton4PressCb(void *button_handle, void *usr_data) {
    Serial.println("Button 4 pressed - Gamepad BUTTON_8");
    pressAndReleaseGamepadButton(BUTTON_8);
}

static void onButton4DoubleCb(void *button_handle, void *usr_data) {
    Serial.println("Button 4 double click - Gamepad BUTTON_4");
    pressAndReleaseGamepadButton(BUTTON_4);
}

static void onButton4LongCb(void *button_handle, void *usr_data) {
    Serial.println("Button 4 long press - Gamepad BUTTON_5");
    pressAndReleaseGamepadButton(BUTTON_5);
}

Button *initButtonDownUp(gpio_num_t pin, callbackFunction pressCb, callbackFunction releaseCb) {
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

    Button *button3 = new Button(BUTTON_3_PIN, false);
    button3->attachSingleClickEventCb(&onButton3PressCb, NULL);
    button3->attachDoubleClickEventCb(&onButton3DoubleCb, NULL);
    button3->attachLongPressStartEventCb(&onButton3LongCb, NULL);

    Button *button4 = new Button(BUTTON_4_PIN, false);
    button4->attachSingleClickEventCb(&onButton4PressCb, NULL);
    button4->attachDoubleClickEventCb(&onButton4DoubleCb, NULL);
    button4->attachLongPressStartEventCb(&onButton4LongCb, NULL);
}

void bluetoothStatusLog() {
    if (!bleGamepad.isConnected()) {
        Serial.println("Waiting for connection...");
        digitalWrite(LED, LOW);
        delay(500);
        digitalWrite(LED, HIGH);
    } else {
        digitalWrite(LED, LOW);
        delay(500);
    }
    delay(500);
}

void updateOtaLoop(){
    pinMode(BUTTON_1_PIN, INPUT_PULLUP);
    pinMode(BUTTON_2_PIN, INPUT_PULLUP);
    if (digitalRead(BUTTON_1_PIN) == LOW && digitalRead(BUTTON_2_PIN) == LOW) {
        WiFi.mode(WIFI_AP);
        WiFi.softAP(ssid);

        Serial.print("AP IP address: ");
        Serial.println(WiFi.softAPIP());
        
        server.on("/", []() {
            server.sendHeader("Location", "/update");
            server.send(302, "text/plain", "");
        });

        ElegantOTA.begin(&server);
        server.begin();
        Serial.println("HTTP server started");

        while (true) {
            server.handleClient();
            ElegantOTA.loop();
        }
    }    
}

void setup() {
    pinMode(LED, OUTPUT);
    Serial.begin(115200);
    Serial.println("Starting Rally Remote!");

    updateOtaLoop();

    initDefaultButtons();
    
    BleGamepadConfiguration bleGamepadConfig;
    bleGamepadConfig.setAutoReport(false); // Disable auto-report to have manual control
    bleGamepadConfig.setButtonCount(16); // Set button count explicitly (default is 16, but needed when auto-report is disabled)
    bleGamepad.begin(&bleGamepadConfig);
}

void loop() {
    bluetoothStatusLog();
}
