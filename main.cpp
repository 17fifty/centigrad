#include <iostream>
#include <cstring>
#include <math.h>
#include <functional>
#include "scalar.h"

using namespace std;
using namespace autodiff; 

int main() {
	Scalar z(10);
	Scalar y(5);
	Scalar q=y * z * 5;
	Scalar r = z+y;


	Scalar yy = z-y;

	z.Print();
	y.Print();
	q.Print();
}
