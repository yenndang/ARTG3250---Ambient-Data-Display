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
  {133, 347, 43, 65, 132, 84}, // August
  {29, 83, 38, 65, 114, 15}, // September
  {62, 83, 30, 53, 136, 65}, // October
  {36, 136, 34, 53, 126, 314}, // November
  {17, 304, 14, 19, 129, 35} // December
};

// Function to scale the value to PWM range using non-linear scaling
// This method enhances the differentiation of brightness levels across LEDs, 
// especially beneficial for datasets with a wide range of values. 

// More details: This function converts raw data values into scaled PWM values for
//  controlling the brightness of LEDs. Unlike logarithmic scaling, which equally 
//  distributes the perception of brightness across a wide range of values, non-linear
//  scaling using a square root function amplifies the differences in mid-range values. 
// This ensures that smaller and mid-range values are also perceptibly represented by 
// the LEDs, making the visualization more dynamic and informative. Larger values are 
// prevented from monopolizing the higher brightness levels, ensuring a balanced and 
// visible spectrum of data points.

// Example: For an input value of 781, 
//    the normalized value approaches 1, 
//    and after applying the square root scaling,
//    it remains close to 1. This value is then scaled 
//    to the maximum PWM value of 255, representing full brightness.
//    For mid-range values, the square root scaling significantly increases
//    their relative brightness, ensuring that these values are more distinguishable.
int logScale(int input) {
    const int maxInput = 781;  // Define the maximum input value expected
    if (input <= 0) return 0;  // Return 0 for non-positive input values to keep the LED off
    
    // Normalize the input value between 0 and 1
    double normalized = (double)input / maxInput;

    // Apply a square root function to the normalized value for non-linear scaling
    double scaled = pow(normalized, 0.5);  // Enhances sensitivity for mid-range values

    // Convert the scaled value to the PWM range (0-255), applying a minimum brightness threshold
    int pwmValue = (int)(scaled * 255);
    pwmValue = max(10, pwmValue);  // Ensures visibility for low but non-zero values

    return pwmValue;
}

void setup() {
  // Initialize LED pins
  for (int i = 0; i < numberOfRegions; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  //DEBUGGING
  Serial.begin(9600); // Start serial communication in your setup()

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

            // Debugging output + for monitoring
            Serial.print("Region: ");
            Serial.print(regionNames[region]);
            Serial.print(", Month: ");
            Serial.print(month + 1);
            Serial.print(", Data Value: ");
            Serial.print(regionData[month][region]);
            Serial.print(", PWM Value: ");
            Serial.println(targetBrightness);

            analogWrite(ledPins[region], targetBrightness);
        }

        // Pause to allow the audience to view the display
        delay(7000); // 7 seconds to view each month's data
    }
}