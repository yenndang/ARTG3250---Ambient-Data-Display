// Define the number of regions and months
const int numberOfRegions = 13;
const int numberOfMonths = 12; // From January - December 2023

// Define region names for reference
// Note that those regions where where the incidents happened and were reported
const char* regionNames[numberOfRegions] = {
  //   3                    5          
  "Central America", "Mediterranean", // Top 6 where most incidents happened 2023
  //    6                 9           
  "North America", "Northern Africa", 
  //     10                11             
  "Southern Asia", "Western Africa"
};

// Assign PWM-capable LED pins to each region 
int ledPins[numberOfRegions] = { 3, 5, 6, 9, 10, 11};  // 3, 5, 6, 9, 10, 11 ports with "PWM~" label on RedBoard

// NOTES: For prototyping purpose of this project, we use array structure to input our subset of data.
//        If we expand this project to use the full dataset, we would need a more powerful board
//        to read CSV file from a SD card or something. 
// Data for number of dead and missing migrants for each region and month
// The rows represent months (0=January, 1=February, etc.)
// The columns represent regions in a consistent order (e.g., 0=Caribbean, 1=Central America, etc.)
int regionData[numberOfMonths][numberOfRegions] = {
  // Jan, Feb, Mar, ..., Dec (Data for each month for all regions)
  // Central America, Mediterranean, North America, Northern Africa, Southern Asia, Western Africa
  {23, 76, 17, 26, 79, 10}, // January
  {99, 271, 25, 75, 80, 96}, // February
  {62, 241, 42, 98, 114, 4}, // March
  {29, 562, 17, 80, 2, 11}, // April
  {25, 146, 27, 45, 22, 91}, // May
  {17, 781, 18, 93, 49, 3}, // June
  {35, 99, 58, 149, 99, 82}, // July
  {133, 347, 43, 65 132, 84}, // August
  {29, 83, 38, 65, 114, 15}, // September
  {62, 83, 30, 53, 136, 65}, // October
  {36, 136, 34, 53, 126, 314}, // November
  {17, 304, 14, 19, 129, 35} // December
};

// Function to scale the value to PWM range using a logarithmic scale
// (to make sure the lights are all visible on the map within the brightness 0-255)

// *More details*: convert raw data values into scaled PWM values for controlling the 
// brightness of LEDs. The purpose of using a logarithmic scale is to visually normalize 
// the wide range of data values, ensuring that even smaller values are perceptibly 
// represented by the LEDs, while also preventing larger values from maxing out the 
// brightness too quickly. This approach helps to maintain visual distinction across 
// a broad spectrum of data points.
//    Example: Example 1: Input value is 781 (the maximum in your dataset).
//    normalized = log(781) / log(781) = 1
//    pwmValue = 1 * 255 = 255
//    Since 255 is not less than 10, the function returns 255. This represents full brightness.
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