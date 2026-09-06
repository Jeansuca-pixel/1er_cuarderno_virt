#include <iostream>
using namespace std;
int main()
{
    int i,j,n,x,y;
    cout<<"ingrese n: ";cin>>n;
    for(i=1;i<=n;i++)
    {
        if(i<=(n+1)/2)
        {
            x=i;
        }
        else
        {
            x=n-i+1;
        }
        y=n+1-(2*x);
        for(j=1;j<=x;j++)
        {
            cout<<"x";
        }
        for(j=1;j<=y;j++)
        {
            cout<<" ";
        }
        for(j=1;j<=x;j++)
        {
            cout<<"x";
        }
        cout<<endl;
    }
    return 0;
}