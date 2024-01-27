#include <iostream>
#include <cstring>
#include <math.h>
#include <functional>
#include <unordered_set>

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
            Scalar operator+(double num) {
                return *this + Scalar(num);
            }
            friend Scalar operator+(double num, Scalar& obj) {
                return Scalar(num)+ obj;
            }

            Scalar operator*(Scalar &other)
            {
                Scalar set[]={*this, other};
                Scalar out((value * other.value), set, '*');
                grad = other.value;
                other.grad = value;
                return out;
            }
            Scalar operator*(double num)
            {
                return *this * Scalar(num);
            }
            friend Scalar operator*(double other, Scalar& obj) {
                Scalar result(other * obj.value);
                return result;
            }

            Scalar operator-(){
                return *this * Scalar(-1);
            }
            Scalar operator-(double num) {
                return *this - Scalar(num);
            }
            Scalar operator-(Scalar& other) {
                return (*this) + (-other);
            }
            friend Scalar operator-(double num, Scalar& obj) {
                return Scalar(num) - obj;
            }


            void Backward()
            {
                // TODO - implement topo sort algo
                vector<Scalar*> topo;
                unordered_set<Scalar*> visited;
                this->grad = 1;
                function<void(Scalar*)> buildTopo = [&](Scalar* v) {
                    if (visited.find(v) == visited.end()) {
                        visited.insert(v);
                        for (Scalar* child : v->children) {
                            buildTopo(child);
                        }
                        topo.push_back(v);
                    }
                };
                buildTopo(this);
                cout << "Topo Order: ";
                for (Scalar* node : topo) {
                    std::cout << node << " ";
                }
                cout << std::endl;
            }

            void Print()
            {
                cout << "Deriv: " << grad << " Child 1 " << children[0] << " Child 2: " << children[1] << endl;
            }
    };
}
