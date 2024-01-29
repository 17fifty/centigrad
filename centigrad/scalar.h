#include <iostream>
#include <cstring>
#include <math.h>
#include <functional>
#include <unordered_set>

using namespace std;

//TODO : Fix topo sort nested children issue
//TODO:  Add power and division
//TODO:  call _backwards();

namespace autodiff
{
    class Scalar
    {
        private:
            double value;
            double grad;
            vector<Scalar*> children = {};
            char op;
            function<void()> _backwards;

        public:
            Scalar(double value, vector<Scalar*> children= {}, char op=' ')
            {
                this->value = value;
                this->grad = 0;
                this->children = children;
                this->op = op;
            }

            Scalar operator+(Scalar &other)
            {
                Scalar* p = &other;
                vector<Scalar*> set={this,p};
                Scalar out((value + other.value), set, '+');

                out._backwards =[this, &other, out]()
                {
                    this->grad += out.grad;
                    other.grad += out.grad;
                };
                return out;
            }
            Scalar operator+(double num) {
                Scalar v(num);
                return *this + v;
            }
            friend Scalar operator+(double num, Scalar& obj) {
                return Scalar(num)+ obj;
            }

            Scalar operator*(Scalar &other)
            {
                Scalar* p = &other;
                vector<Scalar*> set={this,p};
                Scalar out((value * other.value), set, '*');

            out._backwards =[this, &other, out]()
            {
                this->grad += other.value * out.grad;
                other.grad += this->value * out.grad;
            };
            return out;
        }
        Scalar operator*(double num)
        {
            Scalar x(num);
            return *this * x;
        }
        friend Scalar operator*(double other, Scalar& obj) {
            Scalar result(other * obj.value);
            return result;
        }

        Scalar operator-(){
            Scalar x( -1 );
            return *this * x;
        }
        Scalar operator-(double num) {
            Scalar x(num);
            return *this - x;
        }
        Scalar operator-(Scalar& other) {
            Scalar x(-other);
            return (*this) + x;
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
                    if (v != NULL) {
                        for (Scalar *child: v->children) {
                            buildTopo(child);
                        }
                    }
                    topo.push_back(v);
                }
            };
            buildTopo(this);
            cout << "Topo Order: ";
            for (Scalar* node : topo) {//should reverse order
                //doesnt work here !
                //node->_backwards();
                node->Print();
            }
            cout << std::endl;
        }

        void Print()
        {
            cout << "Value:" << this->value << ", grad:" << this->grad << endl; 
        }
    };
}
