#define POLE_COUNT 5

#include <stdarg.h>
#include <math.h>
#include <limits.h>

void setup() {
  // setup serial port
  Serial.begin(9600); 

}

// Debug class, remove in final code
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

class led{
  public:
    virtual void changeIntensity() = 0;

    // destructor of led class
    ~led() {
      Serial.println("led class object destroyed");
    }
};

class sensor{
  public:
    virtual int sense() = 0;

    // destructor of sensor class
    ~sensor() {
    Serial.println("Sensor class object destroyed");
    }
};

class streetLight : public led, public sensor{
  
  public:

    // a parametrized constructor of the class streetLight
    streetLight(int sensorPin, int ledPin, int streetLightID){
      this -> streetLightID = streetLightID;
      this->sensorPin = sensorPin;
      this->ledPin = ledPin;
      Serial.println("Object " + String(streetLightID) + " created");
    }

    // an implementation of sense() method from the class sensor
    int sense() {
      // put sensorInput = 0 for testing
      sensorInput = 0; //digitalRead(sensorPin);
      return sensorInput;
    }

    // an implementation of changeIntensity() method from the class led
    void changeIntensity() {
      analogWrite(ledPin, 5);
    }

    // returns the intensity array according to a single sensor
    float* returnIntensityArray(int sensorInput) {
      Serial.println("streetLightID = " + String(streetLightID));
      // assuming a vehicle is detected
      if(!sensorInput) { 
        // case 1 : first pole
         if(streetLightID == 0) {
            intensityArray [ streetLightID ] = 1;
            intensityArray [ streetLightID + 1 ] = 0.5;
        // case 2 : last pole
         } else if(streetLightID == POLE_COUNT) {
            intensityArray[streetLightID - 1] = 0.5;
            intensityArray[streetLightID] = 1;
        // case 3 : general case           
         } else {
          intensityArray[streetLightID - 1] = 0.5;
          intensityArray[streetLightID + 1] = 0.5;
          intensityArray[streetLightID] = 1;
         }
         //Debug::printArray(intensityArray, POLE_COUNT);

         return intensityArray;
      } else {
        for(int i = 0 ; i < POLE_COUNT ; i++) {
          intensityArray[i] = 0;
        }

        return intensityArray;
      }
    }

    // destructor
    ~streetLight(){
      Serial.println("Street light object destroyed");
    }

  private:
    int streetLightID;
    int sensorPin, ledPin;
    float intensityArray[POLE_COUNT] = {0}; 
    volatile int sensorInput;
}; 

class Logic {
  public:
    Logic() {
      Serial.println("Logic class default constructor");
    }

    void processIntensityArrays(int poleCount, ...) {
      Serial.println("Inside processIntensityArrays function");
      va_list args;
      va_start(args, poleCount);
      
      float* intensityArrays[poleCount]; 
      // make a 2D intensity array
      for(auto i = 0 ; i < poleCount ; i++) {
        float* x = va_arg(args, float*);
        Debug::printArray(x, POLE_COUNT);
        intensityArrays[i] = x;
        //Debug::printArray(x, POLE_COUNT);
      }
      
      // remove this, only for debug purpose
      for(int i = 0 ; i < poleCount ; i++) {
          Serial.println("Printing array " + String(i));
        for(int j = 0 ; j < poleCount ; j++) {
          Serial.print(String(intensityArrays[i][j])+" ");
        }
        Serial.println();
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


      Serial.println("Printing intensityValues array: ");
      Serial.println("Car Count"+ String(carCounter));
      
      Debug::printArray(intensityValues, POLE_COUNT);
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

  //int senseVal = SL0->sense();
  float* intensityArray0 = SL0->returnIntensityArray(0);
  float* intensityArray1 = SL1->returnIntensityArray(1);
  float* intensityArray2 = SL2->returnIntensityArray(1);
  float* intensityArray3 = SL3->returnIntensityArray(1);
  float* intensityArray4 = SL4->returnIntensityArray(0);

  // to combine the intensity arrays into one final array
  Logic* logic = new Logic();
  
  logic -> processIntensityArrays(
    POLE_COUNT,
    intensityArray0,
    intensityArray1,
    intensityArray2,
    intensityArray3,
    intensityArray4
  );

  delete logic;
  delete SL0;
  delete SL1;
  delete SL2;
  delete SL3;
  delete SL4;

  Serial.println();
  Serial.println();

  delay(10000);

  Serial.println();
  Serial.println();
  
}
