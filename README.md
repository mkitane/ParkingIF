ParkingIF
=========

Parking is a simulation application for managing a car park.

It allows you to manage the entries and the exits of this parking which is composed of 8 parking lots.
This parking has 3 entries and 1 exit.


##Build And Run
Move the tp-multitache folder into the /public/tp folder.
	
	mkdir -p /public/tp
	
	mv tp-multitache /public/tp
	make
	./Parking
	

##Dependencies
This project only works on linux because libtp is only available on linux.
 
*	ncurses
*	tcl
*	libtp	(Available on tp-multitache folder)