#include<iostream>
using namespace std;
int main()
{
    float r,h;
    r=5;
    h=12;
    cout<<"VOLUMEN:"<<endl;
    cout<<(((2*r*r*r)/3)+r*r*h);
    cout<<"π"<<endl;
    cout<<"SUPERFICIE:"<<endl;
    cout<<(2*r*(h+r))+r*r;
    cout<<"π";
    return 0;
}