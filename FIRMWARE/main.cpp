#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

const int batteryPin = 34;
const int trigPin = 5;
const int echoPin = 18;

bool lowBatteryWarning = false;
bool obstaclePreviouslyDetected = false;
bool exploreModeEnabled = false;
bool recordingEnabled = true;
bool returnHomeRequested = false;
bool returnHomeComplete = false;
bool homeReached = false;

volatile long leftEncoderCount = 0;
volatile long rightEncoderCount = 0;

unsigned long exploreStartTime = 0;
unsigned long totalExploreTime = 0;
unsigned long missionStartTime = 0;
unsigned long missionDuration = 0;


enum Mode {
    MANUAL_MODE,
    EXPLORE_MODE,
    RETURN_HOME_MODE
};

enum State {
    STARTUP_STATE,
    NORMAL_STATE,
    OBSTACLE_RECOVERY_STATE,
    CRITICAL_BATTERY_STATE
};

Mode currentMode = MANUAL_MODE;
State currentState = STARTUP_STATE;

float batteryPercent = 100;
float pitch = 0;
float roll = 0;
float obstacleDistance = 0;
float distanceTravelled = 0;
float speed = 0;
float currentSpeed = 0;
float totalDistanceTravelled = 0;
float estimatedRange = 0;
float wheelDiameterCM = 6.5;
float wheelCircumferenceCM = wheelDiameterCM * PI;
float totalDistanceExplored = 0;

int obstacleCount = 0;
int recoveryAttempts = 0;

String getModeName() {
    switch (currentMode) {
        case MANUAL_MODE:
            return "MANUAL";
        case EXPLORE_MODE:
            return "EXPLORE";
        case RETURN_HOME_MODE:
            return "RETURN_HOME";
    }
    return "UNKNOWN";
}

String getStateName() {
    switch (currentState) {
        case STARTUP_STATE:
            return "STARTUP";
        case NORMAL_STATE:
            return "NORMAL";
        case OBSTACLE_RECOVERY_STATE:
            return "OBSTACLE_RECOVERY";
        case CRITICAL_BATTERY_STATE:
            return "CRITICAL_BATTERY";
    }
    return "UNKNOWN";
}

struct MovementStep {
    String action;
    float value;
};

MovementStep pathHistory[500];

int pathIndex = 0;

void setup() {
    Serial.begin(115200);
    while (!mpu.begin()) {
        Serial.println("MPU6050 NOT FOUND");

        while (1)
        {
            delay(10);
        }
    }
    Serial.println("MPU6050 READY");
    // configure motor/output pins
    pinMode(2, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    // configure buzzer pin
    pinMode(15, OUTPUT);

    tone(15, 1000);
    delay(300);

    noTone(15);
    delay(1000);

    digitalWrite(2, LOW);
    digitalWrite(4, LOW);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);

    currentState = NORMAL_STATE;

    Serial.println("ROVER STARTUP COMPLETE");
}

void leftEncoderISR() {
    leftEncoderCount++;
}

void rightEncoderISR() {
    rightEncoderCount++;
}

void changeMode(Mode newMode) {
    if(currentMode == newMode) return;
    currentMode = newMode;
    Serial.print("MODE CHANGED TO: ");
    switch (currentMode) {
        case MANUAL_MODE:
            Serial.println("MANUAL");
            break;
        case EXPLORE_MODE:
            Serial.println("EXPLORE");
            break;
        case RETURN_HOME_MODE:
            Serial.println("RETURN_HOME");
            break;
    }
}

void changeState(State newState) {
    if(currentState == newState) return;
    currentState = newState;
    Serial.print("STATE CHANGED TO: ");
    switch (currentState) {
        case NORMAL_STATE:
            Serial.println("NORMAL");
            break;
        case OBSTACLE_RECOVERY_STATE:
            Serial.println("OBSTACLE_RECOVERY");
            break;
        case CRITICAL_BATTERY_STATE:
            Serial.println("CRITICAL_BATTERY");
            break;
    }
}

void logMovement(String action, float value) {
    if(!recordingEnabled) return;
    if (pathIndex < 500) {
        pathHistory[pathIndex].action = action;
        pathHistory[pathIndex].value = value;
        pathIndex++;
    }
}

void driveLeftSide(int speed) {

}

void driveRightSide(int speed) {

}

void stopMotors() {

}

void returnHome() {
    Serial.println("RETURNING HOME");
    recordingEnabled = false;
    for (int i = pathIndex - 1; i >= 0; i--) {
        String action = pathHistory[i].action;
        float value = pathHistory[i].value;

        if (action == "FORWARD") {
            moveBackward(value);
        }
        else if (action == "BACKWARD") {
            moveForward(value);
        }
        else if (action == "LEFT") {
            turnRight(value);
        }
        else if (action == "RIGHT") {
            turnLeft(value);
        }

        delay(1000);
    }
    recordingEnabled = true;
    Serial.println("ARRIVED HOME");
}

void handleExploreMode() {
    if (!exploreModeEnabled) return;

    if (currentState == OBSTACLE_RECOVERY_STATE) {
        Serial.println("EXPLORE: OBSTACLE FOUND");
        Serial.println("STOPPING");
        delay(500);

        Serial.println("REVERSING");
        delay(1000);

        Serial.println("Scanning New Direction");
        delay(1000);

        Serial.println("Path Found");

        currentState = NORMAL_STATE;
    }
}

void readIMU() {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    pitch = atan2(-a.acceleration.x, sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z)) * 180.0 / PI;
    roll = atan2(a.acceleration.y, a.acceleration.z) * 180.0 / PI;
}

void readUltrasonic() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    obstacleDistance = (duration * 0.034) / 2;

    if (obstacleDistance < 20) {
        if (!obstaclePreviouslyDetected) {
            obstacleCount++;
            obstaclePreviouslyDetected = true;
        }
        currentState = OBSTACLE_RECOVERY_STATE;
    }
    else {
        obstaclePreviouslyDetected = false;

        if(currentState == OBSTACLE_RECOVERY_STATE)
            currentState = NORMAL_STATE;
    }
}

void readBattery() {
    int adcValue = analogRead(batteryPin);
    batteryPercent = map(adcValue, 0, 4095, 0, 100);

    if (batteryPercent < 30) {
        lowBatteryWarning = true;
    } else {
        lowBatteryWarning = false;
    }

    if (batteryPercent < 15) {
        currentState = CRITICAL_BATTERY_STATE;
    }
    else {
        currentState = NORMAL_STATE;
    }
}

void checkSerialCommands() {
    if (Serial.available()) {
        char cmd = Serial.read();
        if (cmd == 'e') {
            exploreModeEnabled = !exploreModeEnabled;
            if (exploreModeEnabled) {
                currentMode = EXPLORE_MODE;
                Serial.println("EXPLORE MODE ENABLED");
            }
            else {
                currentMode = MANUAL_MODE;
                Serial.println("EXPLORE MODE DISABLED");
            }
        }
    if (cmd == 'f')
        moveForward(100);
    if (cmd == 'b')
        moveBackward(50);
    if (cmd == 'l')
        turnLeft(90);
    if (cmd == 'r')
        turnRight(90);
    if (cmd == 's')
        stopRover();
    }
}

void printTelemetry() {
    Serial.println("=============");

    Serial.print("MODE: ");
    Serial.println(getModeName());

    Serial.print("STATE: ");
    Serial.println(getStateName());

    Serial.print("BATTERY: ");
    Serial.print(batteryPercent);
    Serial.println("%");

    Serial.print("DISTANCE TRAVELLED: ");
    Serial.print(distanceTravelled);
    Serial.println(" cm");

    Serial.print("PITCH: ");
    Serial.print(pitch, 1);
    Serial.println(" deg");

    Serial.print("ROLL: ");
    Serial.println(roll, 1);
    Serial.println(" deg");

    Serial.print("OBSTACLE DISTANCE: ");
    Serial.print(obstacleDistance);
    Serial.println(" cm");

    Serial.print("OBSTACLE COUNT: ");
    Serial.println(obstacleCount);

    Serial.print("Low Battery Warning: ");
    if(lowBatteryWarning)
        Serial.println("YES");
    else
        Serial.println("NO");

    Serial.println("=============");
}

void moveForward(float distance) {
    logMovement("FORWARD", distance);
    distanceTravelled += distance;
    Serial.print("Moving Forward: ");
    Serial.print(distance);
    Serial.println(" cm");
}

void moveBackward(float distance) {
    logMovement("BACKWARD", distance);
    distanceTravelled += distance;
    Serial.print("Moving Backward: ");
    Serial.print(distance);
    Serial.println(" cm");
}

void turnLeft(float angle) {
    logMovement("LEFT", angle);
    Serial.print("Turning Left: ");
    Serial.print(angle);
    Serial.println(" deg");
}

void turnRight(float angle) {
    logMovement("RIGHT", angle);
    Serial.print("Turning Right: ");
    Serial.print(angle);
    Serial.println(" deg");
}

void stopRover() {
    Serial.println("Stopping Rover");
}

void loop() {
    readIMU();
    readBattery();
    readUltrasonic();
    handleExploreMode();
    checkSerialCommands();
    printTelemetry();
    delay(2000);
}