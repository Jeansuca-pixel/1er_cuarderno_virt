#include<iostream>
#include<cmath>
using namespace std;
int main()
{
    double a;
    float L,H,h,p;
    L=4;
    H=4;
    h=2;
    a=2*sqrt(2);
    p=h+H;
    cout<<"VOLUMEN:"<<endl;
    cout<<(L*L*p)-(L*L*h)/3<<endl;
    cout<<"SUPERFICIE:"<<endl;
    cout<<4*(p*L)+2*(a*L)+L*L;
    return 0;
}