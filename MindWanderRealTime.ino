# include "BasicLinearAlgebra.h"    // Library for matrix creation and manipulation
using namespace BLA; 
#include "Brain.h"                  // Library for Reading MindFlex data 
#include <string.h>                 // Library for standard string functions 

// VARIABLES FOR BUZZER //
#define buzzerPin 10                            // Connected Arduino Pin
#define buzzFreq 2500                           // Buzz Frequency (Hz)
#define buzzDur 300                             // Buzz Duration (ms)

// Set up the brain parser, pass it the hardware serial object you want to listen on.
Brain brain(Serial);

// DEFINE ACTIVATION FUNCTIONS // 
float sigmoid (const float x) { return 1 / (1 + exp(-x)); }
float relu (const float x) { return (x > 0) ? x : 0; }
float elu(const float x) { return (x > 0) ? x :(exp(x) - 1); }

// DEFINE LAYER-WISE FUNCTIONS //
// Contents: 
//      1. Layer Bias values as a Matrix 
//      2. Layer Weights as a Matrix
//      3. Layer activation


// Layer 1 (Input)
Matrix<1,7> L1(Matrix<1,24> input)
{ 
  // Bias
  const Matrix <1,7> bias PROGMEM = { -0.2517,  0.6357, -0.3491,  0.7057, -0.3068,  0.6104,  0.0860};   

  // Weights
  const Matrix <24,7> weight PROGMEM = {                                   
     -0.7109,  0.8510,  0.3809,  0.8162,  0.8062,  0.1766,  0.4414,
        -0.0101,  0.1669, -0.5794, -0.1604,  0.1819, -0.7792,  0.0728,
         0.8731, -0.4685, -0.1741,  0.9154,  0.1490,  0.1535, -0.0932,
        -0.2356, -0.1268,  0.3998,  0.7051,  0.6105,  0.0670, -0.3723,
        -0.7170,  0.5608, -0.2402, -0.1164, -0.3904, -0.2280,  0.5092,
        -0.2117, -0.5373,  0.8327,  0.9219,  0.8558,  0.4211, -0.3143,
        -0.2147,  0.2443, -0.1768, -0.3953, -0.4071, -0.0308,  0.6533,
        -0.5665,  0.0968,  0.1838, -0.2971, -0.3521, -0.0022,  0.1724,
        -0.2377, -0.2034,  0.0861, -0.3679, -0.3127, -0.3041, -0.4911,
        -0.3205,  0.3139,  0.2065, -0.4588,  0.1134, -0.0393, -0.7195,
        -0.5351,  0.1150,  0.4603, -0.4572,  0.0140,  0.3723, -0.0657,
         0.3550, -0.5115,  0.5111,  0.0517,  0.5035,  0.3824, -0.7093,
        -0.4950,  0.1971,  0.0071, -0.0213, -0.4725, -0.0837,  0.2077,
         0.1424, -0.4406,  0.7665, -0.1001,  0.3727,  0.3705, -0.3305,
         0.0313, -0.4009,  0.1758, -0.6052, -0.6080,  0.3661,  0.0339,
        -0.3436, -0.3680,  0.6389, -0.5360, -0.1970,  1.0029,  0.0715,
         0.2788, -0.9685, -0.0651, -0.1795, -0.8181, -0.8032,  0.7606,
        -0.3906,  0.0506, -0.1237,  0.9123,  0.1666,  0.6325,  0.5268,
         0.3241, -0.2574,  0.0612,  0.1019,  0.1651, -0.0704,  0.1862,
         0.2884, -0.3700,  0.0810,  0.2544,  0.3571,  0.1098, -0.0325,
        -0.7772,  0.3862, -0.6116, -0.2757,  0.2836,  0.0517,  0.2334,
        -0.1178, -0.1635, -0.0910,  0.2777,  0.1826,  0.0612,  0.4742,
        -0.1695,  0.1709, -0.1820, -0.0281, -0.2970, -0.2499,  0.7422,
        -0.0169, -0.2456, -0.1461, -0.2266, -0.1562, -0.2563,  0.1958};  
  
  // Calculation of layer raw vales               
  Matrix<1,7> h1 = (input * weight) + bias;                             

  // Applying appropriate activation function
  auto ref = h1.Ref();                          // Define a reference matrix to an existing matrix (layer raw output: h1) 
  int row  = h1.GetRowCount();                  // Get no.of rows
  int col = h1.GetColCount();                   // Get no. of columns
  for (uint8_t i=0; i<row; i++)                 
  {
    for (uint8_t j=0; j<col; j++)
    {
      h1(i,j) = elu(ref(i,j));                 // Apply Activation Function to each element 
    }
  }
  return h1;                        
}

// Layer 2 (hidden1)
Matrix<1,6> L2(Matrix<1,7> h1)
{
 const Matrix<1,6> bias PROGMEM = {0.3426,  0.7902, -0.0063,  0.2981,  0.7279,  0.4850};

 const Matrix<7,6> weight PROGMEM = {
     -0.6380,  0.5847,  0.1242,  0.5562, -0.4243, -0.6035,
        0.9608, -1.1288,  0.2324, -0.6350,  1.2560,  0.2709,
        -0.6555,  0.1769,  0.4807, -0.1378, -0.7942, -0.7357,
         0.1086,  1.8027,  3.1698, -0.1676, -0.4212,  1.1854,
        -0.4715,  0.1041,  1.0443,  0.3378, -0.4691, -1.1837,
         0.2910,  0.4982,  1.0747,  0.4926, -0.1499, -0.4094,
         1.7875, -0.3607, -0.9957, -1.3941,  0.4323,  2.0123};
  Matrix<1,6> h2 = (h1 * weight) + bias;

  
  auto ref = h2.Ref(); 
  int row  = h2.GetRowCount();
  int col = h2.GetColCount();
  for (uint8_t i=0; i< row; i++)
  {
    for (uint8_t j=0; j< col; j++)
    {
      h2(i,j) = relu(ref(i,j));
    }
  }

  return h2; 
}

// Layer 3 (hidden 2)
Matrix<1,5> L3(Matrix<1,6> h2)
{
  const Matrix<1,5> bias PROGMEM = { 0.5460,  0.8043, -0.5330,  0.8857,  0.5894};

  const Matrix<6,5> weight PROGMEM = {
       -1.2875,  1.1694, -0.5028, -1.8394,  1.7576,
         1.1011, -0.8152, -0.1247,  1.1677, -1.8159,
         1.5977, -0.5963,  0.8439,  2.3602, -0.5672,
         0.1094, -0.7828, -0.3571, -0.5283, -0.6407,
        -0.4906,  0.8981,  0.0490, -0.0721,  1.1773,
        -1.3812,  0.9367, -0.4593, -2.1601,  1.6151};

  Matrix<1,5> h3 = (h2 * weight) + bias;


  auto ref = h3.Ref(); 
  int row  = h3.GetRowCount();
  int col = h3.GetColCount();
  for (uint8_t i=0; i< row; i++)
  {
    for (uint8_t j=0; j< col; j++)
    {
      h3(i,j) = relu(ref(i,j));
    }
  }
  
  return h3; 

}


// Layer 4 (output)
double L4(Matrix<1,5> h3)
{
  const Matrix<1,1> bias PROGMEM = {-0.8265};

  const Matrix<5,1> weight PROGMEM = { 
        1.2618,
        -1.4704,
         1.0304,
         2.3992,
        -1.1466
  };

  Matrix<1,1> h4 = (h3 * weight) + bias;
  

  auto ref = h4.Ref(); 
  uint8_t row  = h4.GetRowCount();
  uint8_t col = h4.GetColCount();
  for (uint8_t i=0; i< row; i++)
  {
    for (uint8_t j=0; j< col; j++)
    {
      h4(i,j) = sigmoid(ref(i,j));
    }
  }
  

  return h4(0,0);                                       // Return element; not (1,1) matrix
}


// DEFINING EEG READING FUNCTION // 
Matrix <1,24> readInput(void)
{
  // Innitializations 
  Matrix<1,24> input;        // Define Input matrix
  input.Fill(0) ;            // Innitialize Input matrix with 0s
  uint8_t c = 0;             // Innit  a counter variable 
  
  while(1)
  {
    if (brain.update())
       {
          char* EEG_str = brain.readCSV();    // store EEG data string 
          #define bufferSize 11               // define bufferSize to be length of EEG data string
          float arr[1][8];                    // innitialize an array to store EEG data as floats
  
          char *p = strtok(EEG_str, ",");     // create temporary string that holds first EEG element 
                                             
          size_t index = 0;                             // Innit index
          size_t offset = 0;                            // Innit offset
          while (p != nullptr && index<bufferSize)      // for every EEG data element
          {    
            if(index>=3)                                  // if the element is the third or greater
            {
              arr[0][offset++] = atof(p);                 // convert the element from string to float
            }
            index++;                                      // update index (increment it by 1) 
            p = strtok(NULL, ",");                        // set p to the next element of EEG data 
          }
        
          {
            
            // Center for scaling 
            const float centerArr[] PROGMEM = {763727.5, 305545.2192, 120259.8082, 108257.6575, 107781.8836, 93348.2671, 59532.411, 41665.4795, 774772.5342, 307755.4315, 127224.137, 99825.274, 119648.1644, 111443.3493, 68701.6712, 47535.4247, 670013.2603, 336091.5137, 137296.1849, 111801.1438, 130838.7534, 114929.1096, 77775.8356, 57266.9521};
            // Sclaes for scaling 
            const float scaleArr[] PROGMEM = {689481.9648, 344369.2637, 168157.1435, 164327.4593, 145694.5021, 121902.0424, 76709.8253, 60074.2447, 650649.7913, 364156.8013, 177017.4129, 129661.4743, 160570.0097, 159264.7604, 102066.6425, 64606.5694, 558380.8684, 470732.7316, 224739.0479, 145396.4656, 177029.3171, 176695.2469, 127958.0964, 97400.3227};
    
            if(c==0) // First 8 elements 
            {
              RefMatrix<1,8, Array<1,24>> inp1(input.Submatrix(Slice<0,1>(), Slice<0,8>()));  // create a reference matrix to the first 8 elemnts of INPUT array
              for(uint8_t it=0; it<8; it++){
                arr[0][it] = (arr[0][it]-centerArr[it+0])/scaleArr[it+0]; }; // Scale input to feed into Neural Network
              inp1 = arr;
            }
            else if (c==1) // Second 8 Elements
            {
              RefMatrix<1,8, Array<1,24>> inp2(input.Submatrix(Slice<0,1>(), Slice<8,16>())); // create a reference matrix to the second 8 elemnts of INPUT array
              for(uint8_t it=0; it<8; it++){
                arr[0][it] = (arr[0][it]-centerArr[it+8])/scaleArr[it+8]; }; // Scale input to feed into Neural Network
              inp2 = arr;
            } 
            else if (c==2) // Third 8 elements
            {
              RefMatrix<1,8, Array<1,24>> inp3(input.Submatrix(Slice<0,1>(), Slice<16,24>())); // create a reference matrix to the third 8 elemnts of INPUT array
              for(uint8_t it=0; it<8; it++){
                arr[0][it] = (arr[0][it]-centerArr[it+16])/scaleArr[it+16]; }; // Scale input to feed into Neural Network
              inp3 = arr; 
              return input; 
            }
            
          }

          c++;  // update counter (incremenet by 1)
     }

  } 

}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // open Serial 
}

void loop() {
  // put your main code here, to run repeatedly:
  
// Sample Input for debugging NN implementation: (NN output should be 1.0000000)
//          Matrix<1,24> input = {1.1999e+00,  1.9412e+00, -4.4360e-01, -1.0592e-01, -4.4011e-01,
//         -3.3042e-01, -4.1873e-01, -5.5564e-01,  1.1092e+00,  1.1190e+00,
//         -5.7833e-01,  8.0910e-01, -5.8992e-01, -3.2458e-01, -2.6332e-01,
//         -5.7832e-01,  1.0491e+00, -2.5156e-01, -3.9398e-01, -2.5180e-02,
//         -1.6684e-01, -4.0006e-01, -3.5726e-01, -4.8050e-01};
// If uncommenting above; comment out lines 259 and 260 (*)
  
  uint8_t posEvents = 0;       // count +ve events ('Mind Wandering') 
  uint8_t i = 0;               // innit iterator
  
  while (posEvents<3)          // Until MindWandering detceted four consecutive times
  {
      Matrix<1,24> input =  readInput(); // (*)        // Read input
  
      // Feed-Forward 
      Matrix<1,7> h1 = L1(input);
      Matrix<1,6> h2 = L2(h1);
      Matrix<1,5> h3 = L3(h2);
      double output = L4(h3); 
  
     if (output > 0.5)              // If output > 0.5 (+ve output reading) 
        posEvents++ ;               // update posEvents
     
     else 
        posEvents = 0;              // re-start counting for positive events

//    Uncomment following lines to see inputs and outputs (for debugging)
//    Serial << "input: " << input << "\n";    
//    Serial.print(F("output: "));
//    Serial.println(output, 7);              
    
  }

    // posEvens = 3
    tone(buzzerPin, buzzFreq, buzzDur); // Beeeeeeep!
}
