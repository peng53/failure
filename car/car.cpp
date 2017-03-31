#ifndef MY_CAR
#define MY_CAR
#include "car.h"

struct Car::SPRMEM {
	string n;
	float s;
	float c_s = 0;
	SPRMEM(string &name,float speed) : n(name), s(speed){}
};

Car::Car(string& name,float speed){
	M = new SPRMEM(name,speed);	
}
Car::~Car(){
	delete M;
}
void Car::engineOn(){
	M->c_s = M->s;
}
void Car::engineOff(){
	M->c_s = 0;
}
void Car::speedUp(){
	M->c_s *= 1.2;
}
void Car::speedDown(){
	M->c_s *= 0.8;
}
ostream& operator<<(ostream& O, Car& C){
	O << C.M->n << " is going at " << C.M->c_s << " mph.";
	return O;
}


#endif
