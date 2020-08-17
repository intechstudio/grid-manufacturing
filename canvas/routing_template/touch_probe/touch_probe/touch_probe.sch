EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L power:GND #PWR0101
U 1 1 5F3A74E2
P 4300 3400
F 0 "#PWR0101" H 4300 3150 50  0001 C CNN
F 1 "GND" H 4305 3227 50  0000 C CNN
F 2 "" H 4300 3400 50  0001 C CNN
F 3 "" H 4300 3400 50  0001 C CNN
	1    4300 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 3400 4300 3200
Wire Wire Line
	4700 3200 4300 3200
$Comp
L Connector_Generic:Conn_01x01 J4
U 1 1 5F3A80BB
P 4900 3200
F 0 "J4" H 4980 3242 50  0000 L CNN
F 1 "Conn_01x01" H 4980 3151 50  0000 L CNN
F 2 "suku_basics:minipin" H 4900 3200 50  0001 C CNN
F 3 "~" H 4900 3200 50  0001 C CNN
	1    4900 3200
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J3
U 1 1 5F3A87E3
P 4900 3000
F 0 "J3" H 4980 3042 50  0000 L CNN
F 1 "Conn_01x01" H 4980 2951 50  0000 L CNN
F 2 "suku_basics:minipin" H 4900 3000 50  0001 C CNN
F 3 "~" H 4900 3000 50  0001 C CNN
	1    4900 3000
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J2
U 1 1 5F3A892A
P 4900 2800
F 0 "J2" H 4980 2842 50  0000 L CNN
F 1 "Conn_01x01" H 4980 2751 50  0000 L CNN
F 2 "suku_basics:minipin" H 4900 2800 50  0001 C CNN
F 3 "~" H 4900 2800 50  0001 C CNN
	1    4900 2800
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J1
U 1 1 5F3A8A88
P 4900 2600
F 0 "J1" H 4980 2642 50  0000 L CNN
F 1 "Conn_01x01" H 4980 2551 50  0000 L CNN
F 2 "suku_basics:minipin" H 4900 2600 50  0001 C CNN
F 3 "~" H 4900 2600 50  0001 C CNN
	1    4900 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 2600 4300 2600
Wire Wire Line
	4300 2600 4300 2800
Connection ~ 4300 3200
Wire Wire Line
	4700 2800 4300 2800
Connection ~ 4300 2800
Wire Wire Line
	4300 2800 4300 3000
Wire Wire Line
	4700 3000 4300 3000
Connection ~ 4300 3000
Wire Wire Line
	4300 3000 4300 3200
$EndSCHEMATC
