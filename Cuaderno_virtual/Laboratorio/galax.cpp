#include<iostream>
#include<cmath>
using namespace std;
int main()
{
    int a,b,c;
    cout<<"insertar a: ";cin>>a;
    cout<<"insertar b: ";cin>>b;
    cout<<"insertar c: ";cin>>c;
    cout<<"a es menor que b: ";
    {
    if(a<b)
        cout<<"True"<<endl;
    else if(a>b)
        cout<<"False"<<endl;
    }
    cout<<"c es mayor que a: ";
    {
    if(c>a)
        cout<<"True"<<endl;
    else if(c<a)
        cout<<"False"<<endl;
    }
    cout<<"a es igual a b: ";
    {
    if(a==b)
        cout<<"True"<<endl;
    else if(a!=b)
        cout<<"False"<<endl;
    }
    cout<<"a es diferente de c: ";
    {
    if(a!=c)
        cout<<"True"<<endl;
    else if(a==c)
        cout<<"False"<<endl;
    }
    cout<<"c es menor o igual que b: ";
    {
    if(c<=b)
        cout<<"True"<<endl;
    else if(c>b)
        cout<<"False"<<endl;
    }
    return 0;
}