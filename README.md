# Motivation
  I found that piercing the hobbyist Brain-Computer-Interface(BCI) realm to be diffiuclt. Firtly, it is expensive - specifically because an EEG device can cost anywhere from 200 dollars to more than a few thousand. Second, it requires quite a wealth of experience - Signal processing, Neural Networks, Electronics, etc. This project is my attempt at creating a useful, relatively functional BCI at relatively low-cost and minimal experience. 

# About the Project 
  This is a neurofeedback system that detects Mind-Wandering during driving: it reads your brain electrical activity, detects if you're Mind-Wandering, and beeps if you do. 
  
# Project Overview
  Phase 1: Preparing the EEG headet - Hacking the Mindflex headset
  
  Phase 2: Data Collection Module
  
  Phase 3: Data Processing: Process data and train classifier
  
  Phase 4: Make the device: Software 
  
  Phase 5:  Make the device: Hardware 
  
  Phase 6: Real-Time testing 

# Getting started
  All details about getting started with the project + details about specific phases of the project can be found on a series of articles on my website : https://joshetasrinivasan.com/distracted-driving-detector-intro/ 

# Content Overview
  1. MindWanderDataCollect.ino: Arduino code for Data Collection Module: reads breain data and prints onto serial 
  2. EEGData_mindWander.py: Python script to save Arduino Serial contents into CSV file 
  3. SampleData.csv: Sample csv file after data collection 
  4. circuitElements.pdf: Circuit details for Data Collection Module 

# Contact 
You can contact me at josheta.srinivasan@gmail.com in the case of any questions or suggestions. 

# Acknowlegements 
< list all resources referred > 
