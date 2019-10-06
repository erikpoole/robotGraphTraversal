const int constGridWidth = 3;
const int constGridHeight = 3;

//const int leftSensorTrigger = 99; //Declare Pins Trigger
//const int leftSensorEcho = 99; //Declare sensor pin
//
//const int rightSensorTrigger = 99; //Declare Pins Trigger
//const int rightSensorEcho = 99; //Declare sensor pin

const int forwardSensorTrigger = 12; //Declare Pins Trigger
const int forwardSensorEcho = 13; //Declare sensor pin

const int motorRDirection = 4;
const int motorRPower = 5;
const int motorLDirection = 6;
const int motorLPower = 7;

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

    int scan() {
      digitalWrite(triggerPin, 1);
      delayMicroseconds(10);
      digitalWrite(triggerPin, 0);

      while(digitalRead(echoPin) == 0) {}
      int startTime = micros();
      while(digitalRead(echoPin) == 1) {}
      int endTime = micros();

      int sensorValue = (endTime - startTime) / 58;
      if (sensorValue > 20) {
        return 0;
      }
      return  1;
    }
};

class Grid {
  private:
  //TODO change to pointer and dynamically size


  public:
    int innerGrid[constGridWidth][constGridHeight];
    int height;
    int width;
  
    Grid() {
      width = constGridWidth;
      height = constGridHeight;

      for (int i = 0; i < constGridWidth; i++) {
        for (int j = 0; j < constGridHeight; j++) {
          innerGrid[constGridWidth][constGridHeight] = -1;
        }
      }
    }

    bool isComplete() {
      for (int i = 0; i < constGridWidth; i++) {
         for (int j = 0; j < constGridHeight; j++) {
          if (innerGrid[constGridWidth][constGridHeight] == -1) {
            return false;
          }
        }
      }
      return true;
    }

    int getCoordValue(int x, int y) {
      return innerGrid[x][y];
    }

    void setCoordValue(int x, int y, int value) {
      innerGrid[x][y] = value;
    }
};

class Robot {
  private:
    
  public:
    int xIndex;
    int yIndex;
    char orientation;
//    Sensor leftSensor = Sensor(leftSensorTrigger, leftSensorEcho);
//    Sensor rightSensor = Sensor(rightSensorTrigger, rightSensorEcho);
    Sensor forwardSensor = Sensor(forwardSensorTrigger, forwardSensorEcho);
  
    Robot(char inputOrientation) {
      xIndex = 1;
      yIndex = 0;
      orientation = inputOrientation;
    }

    //TODO interact with mechanical system
    void moveForward() {
      Serial.println("Move Forward");
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
      analogWrite(motorLPower, 150);
      analogWrite(motorRPower, 150);
      delay(250);
      analogWrite(motorLPower, 0);
      analogWrite(motorRPower, 0);
    }

    //TODO interact with mechanical system
    void rotateRight() {
      Serial.print("Rotate Right, new orientation: ");
      Serial.println(orientation);
      
      switch (orientation) {
        case 'N':
          orientation = 'E';
          break;
        case 'S':
          orientation = 'W';
          break;
        case 'E':
          orientation = 'S';
          break;
        case 'W':
          orientation = 'N';
          break;
      }
      digitalWrite(motorLDirection, HIGH);
      analogWrite(motorLPower, 150);
      analogWrite(motorRPower, 150);
      delay(250);
      analogWrite(motorLPower, 0);
      analogWrite(motorRPower, 0);
      digitalWrite(motorLDirection, LOW);
      
    }
};

void scanAndUpdateGrid(Grid grid, Robot robot) {
  Serial.println("Scanning and updating grid");
  switch (robot.orientation) {
    case 'N':
      if (robot.yIndex + 1 < grid.height) {
        grid.setCoordValue(robot.xIndex, robot.yIndex + 1, robot.forwardSensor.scan());
      }
      break;
    case 'S':
      if (robot.yIndex - 1 >= 0) {
        grid.setCoordValue(robot.xIndex, robot.yIndex - 1, robot.forwardSensor.scan());
      }
      break;
    case 'E':
      if (robot.xIndex + 1 < grid.width) {
        grid.setCoordValue(robot.xIndex + 1, robot.yIndex, robot.forwardSensor.scan());
      }
      break;
    case 'W':
      if (robot.xIndex - 1 >= 0) {
        grid.setCoordValue(robot.xIndex - 1, robot.yIndex, robot.forwardSensor.scan());
      }
      break;
  }
}

// Path ahead has been previously visited or is out of bounds
//Scanning and updating grid
//Rotate Right
//x: 1, y: 0

bool isPathAheadUnvisited(Grid grid, Robot robot) {
  switch (robot.orientation) {
    case 'N':
      Serial.print("Grid Height: ");
      Serial.println(grid.height);
      Serial.print("Future Coordinate Value: ");
      Serial.println(grid.getCoordValue(robot.xIndex, robot.yIndex + 1));
      if (robot.yIndex < grid.height - 1 && grid.getCoordValue(robot.xIndex, robot.yIndex + 1) == -1)  {
        Serial.println("Path ahead is unvisited");
        return true;
      }
    case 'S':
      if (robot.yIndex > 0 && grid.getCoordValue(robot.xIndex, robot.yIndex - 1) == -1) {
        Serial.println("Path ahead is unvisited");
        return true;
      }
    case 'E':
      if (robot.xIndex < grid.width - 1 && grid.getCoordValue(robot.xIndex + 1, robot.yIndex) == -1) {
        Serial.println("Path ahead is unvisited");
        return true;
      }
    case 'W':
      if (robot.xIndex > 0 && grid.getCoordValue(robot.xIndex, robot.yIndex - 1) == -1) {
        Serial.println("Path ahead is unvisited");
        return true;
      }
  }
  Serial.println("Path ahead has been previously visited or is out of bounds");
  return false;
}

bool isPathAheadClear(Grid grid, Robot robot) {
  switch (robot.orientation) {
    case 'N':
      if (robot.yIndex < grid.height - 1 && grid.getCoordValue(robot.xIndex, robot.yIndex + 1) != 1) {
        Serial.println("Path ahead is clear");
        return true;
      }
    case 'S':
      if (robot.yIndex > 0 && grid.getCoordValue(robot.xIndex, robot.yIndex - 1) != 1) {
        Serial.println("Path ahead is clear");
        return true;
      }
    case 'E':
      if (robot.xIndex < grid.width - 1 && grid.getCoordValue(robot.xIndex + 1, robot.yIndex) != 1) {
        Serial.println("Path ahead is clear");
        return true;
      }
    case 'W':
      if (robot.xIndex > 0 && grid.getCoordValue(robot.xIndex, robot.yIndex - 1) != 1) {
        Serial.println("Path ahead is clear");
        return true;
      }
  }
  Serial.println("Path ahead is blocked");
  return false;
}

Grid grid = Grid();
Robot robot = Robot('N');

void setup() {
  // put your setup code here, to run once:
  pinMode(motorRDirection,OUTPUT);
  pinMode(motorLDirection,OUTPUT);
  pinMode(motorRPower,OUTPUT);
  pinMode(motorLPower,OUTPUT);  

  digitalWrite(motorRDirection, HIGH);
  digitalWrite(motorLDirection, LOW);

    Serial.begin(9600);
}

void loop() {
  Serial.println("before loop");
  for (int i = 0; i < 5; i++) {
    Serial.println("loop hit");
  }
  
//      for (int i = 0; i < 3; i++) {
//        for (int j = 0; j < 3; j++) {
//          Serial.println(grid.innerGrid[i][j]);
//        }
//      }
  
  delay(5000);
  Serial.println("Program begin");

  
  while (true) {

    
    //delay between steps
    delay(100);
    Serial.print("x: ");
    Serial.print(robot.xIndex);
    Serial.print(", y: ");
    Serial.println(robot.yIndex);

    bool isPathUnvisited = isPathAheadUnvisited(grid, robot);
    scanAndUpdateGrid(grid, robot);

    if (isPathUnvisited && isPathAheadClear(grid, robot)) {
      robot.moveForward(); 
    } else {
      robot.rotateRight();
    }  
  }

  //victory dance
  robot.rotateRight();
}
