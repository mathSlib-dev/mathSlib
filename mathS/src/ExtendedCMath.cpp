#include<ExtendedCMath.h>


double mathS::double_less(double a, double b) {
	return a < b ? 1. : 0.;
}

double mathS::double_greater(double a, double b) {
	return a > b ? 1. : 0.;
}
double mathS::double_lesseq(double a, double b) {
	return a <= b ? 1. : 0.;
}
double mathS::double_greatereq(double a, double b) {
	return a >= b ? 1. : 0.;
}