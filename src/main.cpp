#include <Arduino.h>
#include <Button.h>
#include <ElegantOTA.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <BleKeyboard.h>
#include <NimBLEDevice.h>

#define LED 8

#define BUTTON_1_PIN GPIO_NUM_5
#define BUTTON_2_PIN GPIO_NUM_6
#define BUTTON_3_PIN GPIO_NUM_7
#define BUTTON_4_PIN GPIO_NUM_3
#define BUTTON_UP_PIN GPIO_NUM_9
#define BUTTON_DOWN_PIN GPIO_NUM_10

const char* ssid = "RallyOTA";

WebServer server(80);

BleKeyboard bleKeyboard;

static void onButtonUpPressCb(void *button_handle, void *usr_data) {
    Serial.println("Button up button pressed");
    bleKeyboard.press(KEY_MEDIA_VOLUME_UP);
}

static void onButtonUpReleaseCb(void *button_handle, void *usr_data) {
    Serial.println("Button up button released");
    bleKeyboard.release(KEY_MEDIA_VOLUME_UP);
}

static void onButtonDownPressCb(void *button_handle, void *usr_data) {
    Serial.println("Button down button pressed");
    bleKeyboard.press(KEY_MEDIA_VOLUME_DOWN);
}

static void onButtonDownReleaseCb(void *button_handle, void *usr_data) {
    Serial.println("Button down button released");
    bleKeyboard.release(KEY_MEDIA_VOLUME_DOWN);
}

static void onButton2PressCb(void *button_handle, void *usr_data) {
    Serial.println("Button 2 / Next track button pressed");
    bleKeyboard.press(KEY_MEDIA_NEXT_TRACK);
}

static void onButton2ReleaseCb(void *button_handle, void *usr_data) {
    Serial.println("Button 2 / Next track button released");
    bleKeyboard.release(KEY_MEDIA_NEXT_TRACK);
}

static void onButton1PressCb(void *button_handle, void *usr_data) {
    Serial.println("Button 1 / Previous track button pressed");
    bleKeyboard.press(KEY_MEDIA_PREVIOUS_TRACK);
}

static void onButton1ReleaseCb(void *button_handle, void *usr_data) {
    Serial.println("Button 1 / Previous track button released");
    bleKeyboard.release(KEY_MEDIA_PREVIOUS_TRACK);
}

static void onButton3PressCb(void *button_handle, void *usr_data) {
    Serial.println("Button 3 pressed");
    bleKeyboard.write(KEY_NUM_ASTERISK);
}

static void onButton3DoubleCb(void *button_handle, void *usr_data) {
    Serial.println("Button 3 double click");
    bleKeyboard.write(KEY_NUM_0);
}

static void onButton3LongCb(void *button_handle, void *usr_data) {
    Serial.println("Button 3 long press");
    bleKeyboard.write(KEY_NUM_1);
}

static void onButton4PressCb(void *button_handle, void *usr_data) {
    Serial.println("Button 4 pressed");
    bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
}

static void onButton4DoubleCb(void *button_handle, void *usr_data) {
    Serial.println("Button 4 double click");
    bleKeyboard.write(KEY_NUM_2);
}

static void onButton4LongCb(void *button_handle, void *usr_data) {
    Serial.println("Button 4 long press");
    bleKeyboard.write(KEY_NUM_3);
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
    if (!bleKeyboard.isConnected()) {
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
    
    bleKeyboard.setName("RallyRemote");
    bleKeyboard.begin();
    
    // workaround to fix the connection with esp32 c3
    BLEDevice::setSecurityAuth(ESP_LE_AUTH_NO_BOND);
    BLEDevice::setSecurityIOCap(ESP_IO_CAP_NONE);
    BLEDevice::setSecurityInitKey(BLE_SM_PAIR_KEY_DIST_ID | BLE_SM_PAIR_KEY_DIST_ENC);
}

void loop() {
    bluetoothStatusLog();
}
