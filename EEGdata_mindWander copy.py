# Imports 
import serial # To access arduino serial monitor
import time   # To facilitate data collection for a set duration
import os     # For file path creation

######################################################################################################################################################################
# Code Info: 
# Read Serial input from hacked mindflex toy to detect MINDWANDERING 
# PROGRAM RESULTS IN : 
#                       1 CSV file with EEG data ("Signal Quality, Attention, Meditation, Delta, Theta, Low Alpha, HIgh Alpha, Low Beta, High Beta, Low Gamma, High Gamma")
#                       ~ plus an optional "*" if button was pressed during data collection
######################################################################################################################################################################

# Note: Lines followed by '##' are those that can/need to be edited to run locally 

# Set-up
arduinoPort = "/dev/cu.usbmodem142201" ##           # Specify port that Arduino is plugged into 
baud = 9600                                         # Arduino Baud rate
directory = os.path.dirname(__file__)               # Find directory this code is located in 
fileName = "EEGdata_mindWander.csv" ##              # Name of file  to store EEG data 
filePath = os.path.join(directory, fileName)        # Make path to EEG data file 
fileExists = os.path.isfile(filePath)               # 1: if EEG data file alreadyexists ; 0: otherwise
dur = 15*60 ##                                      # Duration of data collection (seconds)
headers = "Signal Quality, Attention, Meditation, Delta, Theta, Low Alpha, High Alpha, Low Beta, High Beta, Low Gamma, High Gamma" # Headers for CSV file 

# Connect to Arduino Port
ser = serial.Serial(arduinoPort, baud)              # Instance of serial
print("Connected to Arduino port: ", arduinoPort)   # Status message 

# Get User Input: 
# 'c' for continuing data collection (in case data collection had been previously halted halfway or unitentionally)
# 's' for starting new data collction (i.e. overwrite any old data if file already exists with data)
while 1: 
    inp = input("Press 'c' to continue data collection and 's' to start new data collection: ") 
    if len(inp) == 1:               # check if single character
        if inp.isalpha():           # check if alphabetical
            if inp.isupper():       # check if upper case
                inp.lower()         # change upper case to lower case
            if inp == 'c' or inp == 's':
                break               # break loop if valid input received
            else: 
                errorPrompt = "Error: please enter a valid character: 'c' or 's'\n"
        else: 
            errorPrompt = "Error: please enter a valid character: 'c' or 's'\n"
    else:
        errorPrompt = "Error: input must be a single character only: 'c' or 's'\n"
    
    print(errorPrompt)              # Print Error Prompt till proper data is received
    
# Create File (to save EEG data)
# "w" to overwrite ; "a" to append data if file already has data
if inp == 's':
    file = open(filePath, "w") # Overwrite if data in old file; if not create new file
elif inp == 'c' and fileExists:
    file = open(filePath, "a") # Append new data to the file containg old data
else: 
    print("\nError creating file\n")
print("\nCreated file\n")

# Innitializations
startTime = time.time()     # Innit time
line = 0                    # Innit line

# Start Data collection
while time.time() - startTime <= dur:           # For the duration of data collection specified; write data from serial to file
    Status = (time.time() - startTime)/dur*100  # Calculate how far along data collection we are
    print()

    if line == 0 and inp == 's':                # Print Headers if new file
        file.write(headers + "\n")
        line = line + 1
    
    else:
        getData = str(ser.readline())           # Get data as a string from Arduino Serial 

        data = getData[0:][:-2]
        data = data.strip("b\'r\\")             # remove unwanted characters from string 
        
        
        # print(data) # uncomment if you want to see data in terminal

        # Write data to file
        if len(data.split(",")) == 11 or len(data.split(",")) == 12: # only if complete data is collected
            # There are 11 data points in an EEG reading + 1 optional '*' for each reading if buzzer is pressed
            file.write(data + "\n") # write data with a newline
            line = line+1 

    print("Data Collection {0:.2f} percent complete".format(Status))  # Print status of Data Collection

file.close()                                    # Close file
print("Data collection complete! ")     

