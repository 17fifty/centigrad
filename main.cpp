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
    Scalar zbarinkov(8);
	Scalar q=y + z + zbarinkov;
    q.Backward();

}
