EESchema Schematic File Version 2
LIBS:Wireless RGB Pixel-rescue
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:custom
LIBS:Wireless RGB Pixel-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Wireless RGB Pixel"
Date "2016-07-09"
Rev "1"
Comp ""
Comment1 "Copyright Mark Benson 2016"
Comment2 "License Creative Commons Atribution 4.0 (CC-BY 4.0)"
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ATTINY85-P U1
U 1 1 577FF051
P 5450 2600
F 0 "U1" H 4300 3000 50  0000 C CNN
F 1 "ATTINY85-P" H 6450 2200 50  0000 C CNN
F 2 "DIP8" H 6450 2600 50  0000 C CIN
F 3 "" H 5450 2600 50  0000 C CNN
	1    5450 2600
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 577FF0D9
P 6500 3750
F 0 "C2" H 6525 3850 50  0000 L CNN
F 1 "10n" H 6525 3650 50  0000 L CNN
F 2 "Capacitors_ThroughHole:C_Disc_D6_P5" H 6538 3600 50  0001 C CNN
F 3 "" H 6500 3750 50  0000 C CNN
	1    6500 3750
	1    0    0    -1  
$EndComp
$Comp
L CP C1
U 1 1 577FF120
P 7250 2600
F 0 "C1" H 7275 2700 50  0000 L CNN
F 1 "100u" H 7275 2500 50  0000 L CNN
F 2 "Capacitors_ThroughHole:C_Radial_D5_L11_P2" H 7288 2450 50  0001 C CNN
F 3 "" H 7250 2600 50  0000 C CNN
	1    7250 2600
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 577FF166
P 6500 3350
F 0 "R1" V 6580 3350 50  0000 C CNN
F 1 "10k" V 6500 3350 50  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Cement_Vertical_KOA-BGR-5N-7N" V 6430 3350 50  0001 C CNN
F 3 "" H 6500 3350 50  0000 C CNN
	1    6500 3350
	1    0    0    -1  
$EndComp
$Comp
L D D1
U 1 1 577FF1AF
P 5400 3200
F 0 "D1" H 5400 3300 50  0000 C CNN
F 1 "1N4148" H 5400 3100 50  0000 C CNN
F 2 "Diodes_ThroughHole:Diode_DO-41_SOD81_Vertical_KathodeUp" H 5400 3200 50  0001 C CNN
F 3 "" H 5400 3200 50  0000 C CNN
	1    5400 3200
	1    0    0    -1  
$EndComp
$Comp
L NRF24L01+ U2
U 1 1 578038B0
P 5800 4000
F 0 "U2" H 6230 4150 60  0000 C CNN
F 1 "NRF24L01+" H 6240 3550 60  0000 C CNN
F 2 "" H 5800 4010 60  0001 C CNN
F 3 "" H 5800 4010 60  0000 C CNN
	1    5800 4000
	-1   0    0    1   
$EndComp
$Comp
L WS2812B-RESCUE-Wireless_RGB_Pixel U3
U 1 1 57803B0F
P 5400 4650
F 0 "U3" H 5400 4360 60  0000 C CNN
F 1 "WS2812B" H 5390 4900 60  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03" H 5720 4580 60  0001 C CNN
F 3 "" H 5720 4580 60  0000 C CNN
	1    5400 4650
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X02 P1
U 1 1 57803DD6
P 4350 4550
F 0 "P1" H 4350 4700 50  0000 C CNN
F 1 "Debug Output" V 4450 4550 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02" H 4350 4550 50  0001 C CNN
F 3 "" H 4350 4550 50  0000 C CNN
	1    4350 4550
	0    1    1    0   
$EndComp
$Comp
L Battery BT1
U 1 1 578269B4
P 7650 2600
F 0 "BT1" H 7750 2650 50  0000 L CNN
F 1 "3v" H 7750 2550 50  0000 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02" V 7650 2640 50  0001 C CNN
F 3 "" V 7650 2640 50  0000 C CNN
	1    7650 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 4000 7050 4000
Wire Wire Line
	5800 3900 6950 3900
Wire Wire Line
	6250 3100 6250 4790
Wire Wire Line
	6250 3100 6500 3100
Wire Wire Line
	6500 4300 6500 3900
Wire Wire Line
	6500 3100 6500 3200
Wire Wire Line
	6500 3500 6500 3600
Wire Wire Line
	5800 3800 6250 3800
Connection ~ 6250 3800
Wire Wire Line
	6050 3700 5800 3700
Wire Wire Line
	6050 3200 6050 3700
Wire Wire Line
	6050 3550 6500 3550
Connection ~ 6500 3550
Wire Wire Line
	6050 3200 5550 3200
Connection ~ 6050 3550
Wire Wire Line
	5250 3200 4650 3200
Wire Wire Line
	4650 3200 4650 4000
Wire Wire Line
	3800 4000 4900 4000
Wire Wire Line
	3800 4000 3800 2550
Wire Wire Line
	3800 2550 4100 2550
Connection ~ 4650 4000
Wire Wire Line
	4100 2450 3700 2450
Wire Wire Line
	3700 2450 3700 3900
Wire Wire Line
	3700 3900 4900 3900
Wire Wire Line
	4100 2350 3600 2350
Wire Wire Line
	3600 2350 3600 3800
Wire Wire Line
	3600 3800 4900 3800
Connection ~ 6250 3900
Wire Wire Line
	4400 4300 6500 4300
Connection ~ 6500 4000
Wire Wire Line
	4100 2750 4000 2750
Wire Wire Line
	4000 2750 4000 5050
Wire Wire Line
	4000 5050 6000 5050
Wire Wire Line
	6000 5050 6000 4550
Wire Wire Line
	7050 4000 7050 2850
Wire Wire Line
	6800 2850 7650 2850
Wire Wire Line
	6950 3900 6950 2350
Wire Wire Line
	6800 2350 7650 2350
Wire Wire Line
	7250 2350 7250 2450
Connection ~ 6950 2350
Wire Wire Line
	7250 2850 7250 2750
Connection ~ 7050 2850
Wire Wire Line
	4400 4350 4400 4300
Connection ~ 4850 4300
Wire Wire Line
	4300 4350 3900 4350
Wire Wire Line
	3900 4350 3900 2650
Wire Wire Line
	3900 2650 4100 2650
Wire Wire Line
	7650 2350 7650 2450
Connection ~ 7250 2350
Wire Wire Line
	7650 2850 7650 2750
Connection ~ 7250 2850
Wire Wire Line
	4850 4300 4850 4550
Wire Wire Line
	4850 4550 4970 4550
Wire Wire Line
	6000 4550 5820 4550
Wire Wire Line
	6250 4790 5820 4790
$EndSCHEMATC
