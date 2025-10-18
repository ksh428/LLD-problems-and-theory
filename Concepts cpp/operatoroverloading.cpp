#include <bits/stdc++.h>

using namespace std;

class Complex{
    int real;
    int img;
    public:
    Complex(int r,int i): real(r),img(i) {}
    Complex operator + (const Complex a) const{
        Complex c(a.real+this->real,a.img+this->img);
        return c;
    }
    Complex& operator +=(const Complex& a){
        this->real+=a.real;
        this->img+=a.img;
        return *this;
    } 
    Complex operator - (const Complex a) const{
        Complex c(a.real-this->real,a.img-this->img);
        return c;
    }
    Complex& operator -=(const Complex& a){
        this->real-=a.real;
        this->img-=a.img;
        return *this;
    } 
    bool operator == (const Complex& a)
    {
        return (a.real==this->real && a.img==this->img);
    }
    Complex& operator = (const Complex& a)
    {
        this->real=a.real;
        this->img=a.img;
        return *this;
    }
    void print()
    {
        cout<<real<<" "<<img<<endl;
    }
    // this needds to be a friend function as ostream << is not a member of Complex class and it needs access to the private members of this class
    friend ostream& operator << (ostream& out,Complex& a)
    {
        out<<a.real<<"+"<<"i"<<a.img;
        return out;
    }
};

int main() {
    Complex c(2,3);
    Complex c1(4,5);
    Complex c2=c+c1;
    Complex c3(5,5);
    c2+=c3;
    c2.print();
    Complex c4(11,13);
    c4=c1;
    cout<<(c2==c4)<<endl;
    c4.print();
    Complex c5=c1;
    c5.print();
    cout<<c2;
}