#include <iostream>
using namespace std;
int main()
{
    int i,j,n,x,y;
    cout<<"ingrese n: ";cin>>n;
    for(i=1;i<=(n+1)/2;i++)
    {
        x=2*i-1;
        y=(n-x)/2;
        for(j=1;j<=y;j++)
        {
            cout<<" ";
        }
        for(j=1;j<=x;j++)
        {
            if(j==1 || j==x || x==n)
            {
                cout<<"x";
            }
            else
            {
                cout<<" ";
            }
        }
        cout<<endl;
    }
    return 0;
}