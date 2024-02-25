// Define the number of regions and months
const int numberOfRegions = 13;
const int numberOfMonths = 12; // From January - December 2023

// Define region names for reference
// Note that those regions where where the incidents happened and were reported
const char* regionNames[numberOfRegions] = {
  //   1              2                   3                4            5             6             
  "Caribbean", "Central America", "Eastern Africa", "Eastern Asia", "Europe", "Mediterranean",
  //    7                 8                   9                 10
  "North America", "Northern Africa", "South America", "South-eastern Asia",
  //     11                12               13
  "Southern Asia", "Western Africa", "Western Asia"
};

// Assign PWM-capable LED pins to each region
int ledPins[numberOfRegions] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}; 

// NOTES: For prototyping purpose of this project, we use array structure to input our subset of data.
//        If we expand this project to use the full dataset, we would need a more powerful board
//        to read CSV file from a SD card or something. 
// Data for number of dead and missing migrants for each region and month
// The rows represent months (0=January, 1=February, etc.)
// The columns represent regions in a consistent order (e.g., 0=Caribbean, 1=Central America, etc.)
int regionData[numberOfMonths][numberOfRegions] = {
  // Jan, Feb, Mar, ..., Dec (Data for each month for all regions)
  // Caribbean, Central America, Eastern Africa, ..., Western Asia
  {95, 23, 4, 0, 10, 76, 17, 26, 32, 1, 79, 10, 12}, // January
  {0, 99, 32, 0, 28, 271, 25, 75, 5, 0, 80, 96, 10}, // February
  {21, 62, 52, 14, 9, 241, 42, 98, 1, 6, 114, 4, 43}, // March
  {0, 29, 8, 0, 13, 562, 17, 80, 1, 7, 2, 11, 16}, // April
  {0, 25, 5, 21, 9, 146, 27, 45, 2, 4, 22, 91, 17}, // May
  {4, 17, 11, 0, 2, 781, 18, 93, 2, 7, 49, 3, 255}, // June
  {24, 35, 29, 0, 4, 99, 58, 149, 11, 4, 99, 82, 9}, // July
  {14, 133, 47, 0, 32, 347, 43, 65, 0, 55, 132, 84, 24}, // August
  {2, 29, 20, 1, 5, 83, 38, 65, 5, 1, 114, 15, 63}, // September
  {40, 62, 4, 0, 25, 83, 30, 53, 1, 1, 136, 65, 12}, // October
  {0, 36, 6, 8, 14, 136, 34, 53, 15, 1, 126, 314, 84}, // November
  {47, 17, 2, 0, 6, 304, 14, 19, 1, 4, 129, 35, 84} // December
};

// Function to scale the value to PWM range using a logarithmic scale
// (to make sure the lights are all visible on the map within the brightness 0-255)
int logScale(int input) {
  const int maxInput = 781; // The maximum value from your dataset
  if (input <= 0) return 0; // If the input is 0, keep the LED off
  double normalized = log(input) / log(maxInput); // Logarithmic scaling
  int pwmValue = int(normalized * 255);
  if (pwmValue < 10) pwmValue = 10; // Minimum brightness threshold
  return pwmValue;
}


void setup() {
  // Initialize LED pins
  for (int i = 0; i < numberOfRegions; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}


void loop() {
    for (int month = 0; month < numberOfMonths; month++) {
        // Briefly dim all LEDs as a signal of transitioning to the next month
        for (int region = 0; region < numberOfRegions; region++) {
            analogWrite(ledPins[region], 0); // Dim or turn off
        }
        delay(200); // Short delay for the transition effect

        // Update LEDs to the next month's data
        for (int region = 0; region < numberOfRegions; region++) {
            int targetBrightness = logScale(regionData[month][region]);
            analogWrite(ledPins[region], targetBrightness);
        }

        // Pause to allow the audience to view the display
        delay(7000); // 7 seconds to view each month's data
    }
}

