#include <Arduino.h>

const int joyX = A0;
const int joyY = A1;
const int ledRight = 10;
const int ledUp = 11;
const int ledDown = 12;
const int ledLeft = 13;
const int buzzerPin = 9; 

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

int score = 0;
Direction currentChallenge;
unsigned long challengeStartTime;
const long timeLimit = 1500; 

void startNewGame();
void startNewChallenge();
void displayChallenge(Direction dir);
void allLedsOff();
void successFeedback();
void failureFeedback();
void playToneWithDelay(int frequency, int duration);

void setup() {
    
    pinMode(ledUp, OUTPUT);
    pinMode(ledDown, OUTPUT);
    pinMode(ledLeft, OUTPUT);
    pinMode(ledRight, OUTPUT);
    pinMode(buzzerPin, OUTPUT);
    Serial.begin(9600);
    randomSeed(analogRead(A2));
    allLedsOff(); 
    startNewGame();
}

void loop() {
    int xValue = analogRead(joyX);
    int yValue = analogRead(joyY);

    Direction playerMove;
    if (yValue < 200) {
        playerMove = UP;
    } else if (yValue > 800) {
        playerMove = DOWN;
    } else if (xValue < 200) {
        playerMove = LEFT;
    } else if (xValue > 700) {
        playerMove = RIGHT;
    } else {
        playerMove = (Direction)-1; 
    }

    if (playerMove == currentChallenge) {
        Serial.println("Correct!");
        score++;
        successFeedback();
        startNewChallenge();
    } 
    
    else if (playerMove != (Direction)-1) {
        Serial.println("Wrong move!");
        failureFeedback();
        Serial.println("Game Over! Final Score: " + String(score));
        startNewGame();
    }
    
    if (millis() - challengeStartTime > timeLimit) {
        Serial.println("Time's up!");
        failureFeedback();
        Serial.println("Game Over! Final Score: " + String(score));
        
        startNewGame();
    }
}

void startNewGame() {
    Serial.println("Game started! Get ready...");
    score = 0;
    delay(2000); 
    startNewChallenge();
}

void startNewChallenge() {
    allLedsOff();
    currentChallenge = (Direction)random(4); 
    displayChallenge(currentChallenge);
    challengeStartTime = millis();
    Serial.print("Challenge: ");
    switch (currentChallenge) {
        case UP:    Serial.println("UP");    break;
        case DOWN:  Serial.println("DOWN");  break;
        case LEFT:  Serial.println("LEFT");  break;
        case RIGHT: Serial.println("RIGHT"); break;
    }
    Serial.print("Current Score: ");
    Serial.println(score);
}

void displayChallenge(Direction dir) {
    switch (dir) {
        case UP:
            digitalWrite(ledUp, HIGH);
            break;
        case DOWN:
            digitalWrite(ledDown, HIGH);
            break;
        case LEFT:
            digitalWrite(ledLeft, HIGH);
            break;
        case RIGHT:
            digitalWrite(ledRight, HIGH);
            break;
    }
}

void allLedsOff() {
    digitalWrite(ledUp, LOW);
    digitalWrite(ledDown, LOW);
    digitalWrite(ledLeft, LOW);
    digitalWrite(ledRight, LOW);
}

void successFeedback() {
    for (int i = 0; i < 3; i++) {
        digitalWrite(ledUp, HIGH);
        digitalWrite(ledDown, HIGH);
        digitalWrite(ledLeft, HIGH);
        digitalWrite(ledRight, HIGH);
        playToneWithDelay(1000, 100);
        allLedsOff();
        delay(100);
    }
}

void failureFeedback() {
    allLedsOff();
    for (int i = 0; i < 5; i++) {
        digitalWrite(ledRight, HIGH);
        digitalWrite(ledLeft, HIGH);
        playToneWithDelay(500, 150);
        allLedsOff();
        delay(150);
    }
}

void playToneWithDelay(int frequency, int duration) {
    tone(buzzerPin, frequency);
    delay(duration);
    noTone(buzzerPin);
}