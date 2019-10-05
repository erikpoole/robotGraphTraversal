const int constGridLength = 10;
const int constGridWidth = 10;

class Sensor {
  private:
    int triggerPin;
    int echoPin;
  
  public:
    Sensor(int inputTriggerPin, int inputEchoPin) {
      triggerPin = inputTriggerPin;
      echoPin = inputEchoPin;

      pinMode(inputTriggerPin,OUTPUT);
      digitalWrite(inputEchoPin,LOW);
    }

    int check() {
      digitalWrite(triggerPin, 1);
      delayMicroseconds(10);
      digitalWrite(triggerPin, 0);

      while(digitalRead(echoPin) == 0) {}
      int startTime = micros();
      while(digitalRead(echoPin) == 1) {}
      int endTime = micros();

      return (endTime - startTime) / 58;
    }
};

class Grid {
  private:
  //TODO change to pointer and dynamically size
    int innerGrid[constGridLength][constGridWidth];

  public:
    int gridLength;
    int gridWidth;
  
   Grid() {
    gridLength = constGridLength;
    gridWidth = constGridWidth;
    for (int i; i < constGridLength; i++) {
      for (int j; j < constGridWidth; j++) {
        innerGrid[constGridLength][constGridWidth] = -1;
      }
    }
  }

  boolean isComplete() {
    for (int i; i < constGridLength; i++) {
      for (int j; j < constGridWidth; j++) {
        if (innerGrid[constGridLength][constGridWidth] == -1) {
          return false;
        }
      }
    }
    return true;
  }

  //TODO implement Traveling Salesman Solution
};

class Robot {
  private:
    int xIndex;
    int yIndex;
    char orientation;
    
  public:
    Robot(char inputOrientation) {
      xIndex = 0;
      yIndex = 0;
      orientation = inputOrientation;
    }

    //TODO interact with mechanical system
    void moveForward() {
      switch (orientation) {
        case 'N':
          yIndex++;
          break;
        case 'S':
          yIndex--;
          break;
        case 'E':
          xIndex++;
          break;
        case 'W':
          xIndex--;
          break;
      }
    }

    //TODO interact with mechanical system
    void rotateRight() {
      switch (orientation) {
        case 'N':
          orientation = 'E'
          break;
        case 'S':
          orientation = 'W'
          break;
        case 'E':
          orientation = 'S'
          break;
        case 'W':
          orientation = 'N'
          break;
      }
    }
};

//TODO Change Sensors values
const int leftSensorTrigger = 99; //Declare Pins Trigger
const int leftSensorEcho = 99; //Declare sensor pin

const int rightSensorTrigger = 99; //Declare Pins Trigger
const int rightSensorEcho = 99; //Declare sensor pin

const int forwardSensorTrigger = 99; //Declare Pins Trigger
const int forwardSensorEcho = 99; //Declare sensor pin

const Sensor leftSensor = Sensor(leftSensorTrigger, leftSensorEcho);
const Sensor rightSensor = Sensor(rightSensorTrigger, rightSensorEcho);
const Sensor forwardSensor = Sensor(forwardSensorTrigger, forwardSensorEcho);

const Grid grid = Grid();
const Robot robot = Robot('N');

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  //delay between steps
  delay(100);
  

  //move
  
  //read
  
  //write to grid
}
