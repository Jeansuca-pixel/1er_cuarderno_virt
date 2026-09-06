#include<iostream>
using namespace std;
int main()
{
    int H,h,L,a;
    H=10;
    h=4;
    L=6;
    a=5;
    cout<<"VOLUMEN:"<<endl;
    cout<<L*L*H+((L*L*h)/3)<<endl;
    cout<<"SUPERFICIE:"<<endl;
    cout<<4*(H*L)+L*L+2*(a*L);
    return 0;
}