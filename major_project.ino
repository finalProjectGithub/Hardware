#define POLE_COUNT 5
const int mosfetPins[POLE_COUNT] = {3, 5, 9, 10, 11};
const int sensorPins[POLE_COUNT] = {2, 4, 7, 8, 12};

#include <stdarg.h>
#include <math.h>
#include <limits.h>

void setup() {
  // setup serial port
  Serial.begin(9600); 
  for (int i = 0; i < POLE_COUNT; i++) {
    pinMode(sensorPins[i], INPUT); // Set sensor pins as input
    pinMode(mosfetPins[i], OUTPUT); // Set MOSFET pins as output
  }

}

//// Debug class, remove in final code
class Debug {
public:
    template<typename T>
    static void printArray(T* arr, int size) {
        for (int i = 0; i < size; ++i) {
            Serial.print(arr[i]);
            Serial.print(" ");
        }
        Serial.println();
    }

    template<typename T>
    static void print2DArray(T** arr, int size) {
      Serial.println("Printing 2D array");
      for (int i = 0 ; i < size ; i++) {
        for(int j = 0 ; i < size ; j++) {
          Serial.print(String(arr[i][j])+" ");
        }
        Serial.println();
      }
    }
};

class previousIntensityArray {
  public:
    static  float* PreviousIntensityArray[POLE_COUNT];
};

//
class led{
  public:
    virtual void changeIntensity(float intensity) = 0;

};

class sensor{
  public:
    virtual int sense() = 0;

};

class streetLight : public led, public sensor{
  
  public:

    // a parametrized constructor of the class streetLight
    streetLight(int sensorPin, int ledPin, int streetLightID){
      this -> streetLightID = streetLightID;
      this->sensorPin = sensorPin;
      this->ledPin = ledPin;
    }

    // an implementation of sense() method from the class sensor
    int sense() {
      // put sensorInput = 0 for testing
      sensorInput = digitalRead(sensorPin);
      return sensorInput;
    }

    // an implementation of changeIntensity() method from the class led
    void changeIntensity(float intensity) {
      analogWrite(ledPin, intensity*255);
    }

    // returns the intensity array according to a single sensor
    float* returnIntensityArray(int sensorInput) {
      // assuming a vehicle is detected
      if(!sensorInput) { 
        // case 1 : first pole
         if(streetLightID == 0) {
            intensityArray [ streetLightID ] = 1;
            intensityArray [ streetLightID + 1 ] = 0.1;
        // case 2 : last pole
         } else if(streetLightID == POLE_COUNT) {
            intensityArray[streetLightID - 1] = 0.1;
            intensityArray[streetLightID] = 1;
        // case 3 : general case           
         } else {
          intensityArray[streetLightID - 1] = 0.1;
          intensityArray[streetLightID + 1] = 0.1;
          intensityArray[streetLightID] = 1;
         }
//         Debug::printArray(intensityArray, POLE_COUNT);

         return intensityArray;
      } else {
        for(int i = 0 ; i < POLE_COUNT ; i++) {
          intensityArray[i] = 0;
        }

        return intensityArray;
      }
    }

  private:
    int streetLightID;
    int sensorPin, ledPin;
    float intensityArray[POLE_COUNT] = {0}; 
    volatile int sensorInput;
}; 

class Logic {
  public:

    float* processIntensityArrays(int poleCount, ...) {
      va_list args;
      va_start(args, poleCount);
      
      float* intensityArrays[poleCount]; 
      // make a 2D intensity array
      for(auto i = 0 ; i < poleCount ; i++) {
        float* x = va_arg(args, float*);
//        Debug::printArray(x, POLE_COUNT);
        intensityArrays[i] = x;
        //Debug::printArray(x, POLE_COUNT);
      }


      for(int i = 0 ; i < poleCount ; i++) {
        float max_ele = INT_MIN;
        for(int j = 0 ; j < poleCount ; j++) {
          if (intensityArrays[j][i] > max_ele) {
            max_ele = intensityArrays[j][i];
          }
        }
        intensityValues[i] = max_ele;
        if (max_ele == 1) 
          carCounter++;
      }


      Serial.println(carCounter);
      return intensityValues;
      
    }

  private:
    float intensityValues[POLE_COUNT] = {0};
    int carCounter = 0;

};

void loop(){
  int sensor;
  
  // streetLight(int sensorPin, int ledPin, int streetLightID)
  streetLight* SL0 = new streetLight(2, 3, 0);
  streetLight* SL1 = new streetLight(4, 5, 1);
  streetLight* SL2 = new streetLight(7, 9, 2);
  streetLight* SL3 = new streetLight(8, 10, 3);
  streetLight* SL4 = new streetLight(12, 11, 4);

  int sensorVal0 = SL0->sense();
  int sensorVal1 = SL1->sense();
  int sensorVal2 = SL2->sense();
  int sensorVal3 = SL3->sense();
  int sensorVal4 = SL4->sense();

  //int senseVal = SL0->sense();
  float* intensityArray0 = SL0->returnIntensityArray(sensorVal0);
  float* intensityArray1 = SL1->returnIntensityArray(sensorVal1);
  float* intensityArray2 = SL2->returnIntensityArray(sensorVal2);
  float* intensityArray3 = SL3->returnIntensityArray(sensorVal3);
  float* intensityArray4 = SL4->returnIntensityArray(sensorVal4);

  // to combine the intensity arrays into one final array
  Logic* logic = new Logic();

  float* intensityValues = new float[POLE_COUNT];
  
  intensityValues = logic -> processIntensityArrays(
    POLE_COUNT,
    intensityArray0,
    intensityArray1,
    intensityArray2,
    intensityArray3,
    intensityArray4
  );
//  Debug::printArray(intensityValues, POLE_COUNT);
  SL0->changeIntensity(intensityValues[0]);
  SL1->changeIntensity(intensityValues[1]);
  SL2->changeIntensity(intensityValues[2]);
  SL3->changeIntensity(intensityValues[3]);
  SL4->changeIntensity(intensityValues[4]);

  delete logic;
  delete SL0;
  delete SL1;
  delete SL2;
  delete SL3;
  delete SL4;

  delay(500);
  
}
