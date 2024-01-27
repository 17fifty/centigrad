#include <iostream>
#include <cstring>
#include <math.h>
#include <functional>

using namespace std;

namespace autodiff
{

    class Scalar
    {
        private:
            double value;
            double grad;
            Scalar* children[2] = {};
            char op;
            function<void()> _backwards;

        public:
            Scalar(double value, Scalar children[] = {}, char op=' ')
            {
                this->value = value;
                this->grad = 0;
                *this->children = children;
                this->op = op;
            }

            Scalar operator +(Scalar &other)
            {
                Scalar set[]={*this, other};
                Scalar out((value + other.value), set, '+');

                out._backwards =[this, &other, out]()
                {
                    this->grad += out.grad;
                    other.grad += out.grad;
                };
                return out;
            }

            Scalar operator*(Scalar &other)
            {
                Scalar out(value * other.value, value, other.value, '*');
                grad = other.value;
                other.grad = value;
                return out;
            }

            Scalar operator*(double x)
            {
                Scalar out(value * x, value, x, '*');
                return out;
            }

            Scalar operator-(Scalar &other)
            {
                return *this * -1;
            }

            Scalar OperatorBackward(Scalar &other, Scalar &out)
            {
                if (this->op == '+')
                {
                    this->grad += out.grad;
                    other.grad += out.grad;
                }
                if (this->op == '*')
                {
                    this->grad += other.value * out.grad;
                    other.grad += this->value * out.grad;
                }
                if (this->op == '^')
                {
                    this->grad += (other.value * pow(this->value, (other.value - 1)) * out.grad);
                    // TODO 1 - implement backwards function for power operations
                }
            }

            void Backward()
            {
                // TODO 2 - implement topo sort algo

                this->grad = 1;
                for (auto node : /*topoSortResult*/)
                    node.OperatorBackward(); // TODO 3 - find solution for calling other and out from here
            }

            void Print()
            {
                cout << "Deriv: " << grad << " Child 1 " << children[0] << " Child 2: " << children[1] << endl;
            }
    };
}
