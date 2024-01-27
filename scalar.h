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

            Scalar operator+(Scalar &other)
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
                Scalar set[]={*this, other};
                Scalar out((value * other.value), set, '*');
                grad = other.value;
                other.grad = value;
                return out;
            }

            Scalar operator*(double x)
            {
                Scalar out(value * x);
                return out;
            }
            friend Scalar operator*(const double& other, const Scalar& obj) {
                Scalar result(other * obj.value);
                return result;
            }

            Scalar operator-(){
                Scalar result(-value);
                return result;
            }
            Scalar operator-(double num) {
                Scalar result(value - num);
                return result;
            }
            friend Scalar operator-(double num, const Scalar& obj) {
                Scalar result(num - obj.value);
                return result;
            }


            void Backward()
            {
                // TODO - implement topo sort algo

                this->grad = 1;
                for (auto node : /*topoSortResult*/)//in reverse
                    node._backwards();
            }

            void Print()
            {
                cout << "Deriv: " << grad << " Child 1 " << children[0] << " Child 2: " << children[1] << endl;
            }
    };
}
