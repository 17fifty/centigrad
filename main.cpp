#include <iostream>
#include <cstring>

using namespace std;

class Scalar {
private:
	double value;
	double grad = 0;
	Scalar child[2];
	char op;
public:
	Scalar(int x, double c1, double c2, char opr) {
		value = x;
		child[0] = c1;
		child[1] = c2;
		op = opr;
	}

	Scalar(const Scalar& S) {
		value = S.value;
		child[0] = S.child[0]; 
		child[1] = S.child[1];
		op = S.op;
	}

	Scalar operator +(Scalar& other) {
		Scalar out(value + other.value, value, other.value, '+');
		grad = 1;
		return out;
	}

	Scalar operator *(Scalar& other) {
		Scalar out(value * other.value, value, other.value, '*');
		grad = other.value;
		other.grad = value;
		return out;
	}

	Scalar operator *(double x) {
		Scalar out(value * x, value, x, '*');
		return out;
	}

	/*Scalar operator -(Scalar& other) {
		Scalar out(value - other.value, value, other.value, '-');
		grad = 1;
		return out;
	}*/

	Scalar OperatorBackward(Scalar& other, Scalar& out) {
		if (this->op == '+') {
			this->grad += out.grad;
			other.grad += out.grad;
		}
		if (this->op == '*') {
			this.grad += other.data * out.grad;
			other.grad += this.data * out.grad;
		}
		if (this->op == '^') {
			//TODO 1 - implement backwards function for power operations
		}
	}

	void Backward() {
		//TODO 2 - implement topo sort algo

		this.grad = 1;
		for (auto node : /*topoSortResult*/)
			node.OperatorBackward();	//TODO 3 - find solution for calling other and out from here
	}


	void Print() {
		cout << "Deriv: " << grad << " Child 1 " << child[0] << " Child 2: " << child[1] << endl;
	}

};


int main() {
	Scalar z(10, 0, 0, 'L');
	Scalar y(5, 0, 0, 'L');
	Scalar q(y * z * 5);
	z.Print();
	y.Print();
	q.Print();
}
