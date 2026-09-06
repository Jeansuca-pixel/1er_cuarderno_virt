#include<iostream>
#include<cmath>
using namespace std;
int main()
{
    int a,b;
    cin>>a;
    cin>>b;
    
    if (a+b==5)
    {
        b=3+b;
        cout<<(2*a+b);
    }
    else if(a+b!=5)
    {
        int x,r;
        a=a-1;
        x=7*a+b;
        r=x%2;
        if(r==0)
            cout<<a-b;
        else if(r!=0)
            cout<<a*b;
    }
}