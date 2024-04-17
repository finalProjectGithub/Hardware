#define POLE_COUNT 5

#include <stdarg.h>

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
      }
    }

    // destructor
    ~streetLight(){
      Serial.println("object destroyed");
    }

  private:
    int streetLightID;
    int sensorPin, ledPin;
    float intensityArray[POLE_COUNT] = {0}; 
    volatile int sensorInput;
}; 

class logic {
  public:
    static void processIntensityArrays(int poleCount, ...) {
      va_list args;
      va_start(args, poleCount);

      float** intensityArrays = (float**)malloc(poleCount * sizeof(float*));

      // make a 2D intensity array
      for(auto i = 0 ; i < poleCount ; i++) {
        float* x = va_arg(args, float*);
        intensityArrays[i] = x;
        //Debug::printArray(x, POLE_COUNT);
      }
      
      for(int i = 0 ; i < poleCount ; i++) {
          Serial.println("Printing array " + String(i));
        for(int j = 0 ; j < poleCount ; j++) {
          Serial.print(String(intensityArrays[i][j])+" ");
        }
        Serial.println();
      }
      //Debug::print2DArray(intensityArrays, POLE_COUNT);

    }

  private:
    volatile float intensityValues[POLE_COUNT];

};

void loop(){
  int sensor;
  
  // streetLight(int sensorPin, int ledPin, int streetLightID)
  streetLight* SL0 = new streetLight(2, 3, 0);
  streetLight* SL1 = new streetLight(2, 3, 1);
  streetLight* SL2 = new streetLight(2, 3, 2);
  streetLight* SL3 = new streetLight(2, 3, 3);
  streetLight* SL4 = new streetLight(2, 3, 4);

  //int senseVal = SL0->sense();
  float* intensityArray0 = SL0->returnIntensityArray(0);
  float* intensityArray1 = SL1->returnIntensityArray(0);
  float* intensityArray2 = SL2->returnIntensityArray(0);
  float* intensityArray3 = SL3->returnIntensityArray(0);
  float* intensityArray4 = SL4->returnIntensityArray(0);

  // to combine the intensity arrays into one final array
  logic::processIntensityArrays(
    POLE_COUNT,
    intensityArray0,
    intensityArray1,
    intensityArray2,
    intensityArray3,
    intensityArray4
  );

  Serial.println();
  Serial.println();

  // Serial.println("Printing final intensity arrays");
  // //float* array = new float[POLE_COUNT];
  // Debug::printArray(intensityArray0, POLE_COUNT);
  // Debug::printArray(intensityArray1, POLE_COUNT);
  // Debug::printArray(intensityArray2, POLE_COUNT);
  // Debug::printArray(intensityArray3, POLE_COUNT);
  // Debug::printArray(intensityArray4, POLE_COUNT);

  delay(10000);
  delete SL0;

  Serial.println();
  Serial.println();
  
}
