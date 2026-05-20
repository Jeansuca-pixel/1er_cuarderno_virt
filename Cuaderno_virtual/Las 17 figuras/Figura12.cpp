#include <iostream>
using namespace std;
int main()
{
    int i,j,n;
    cout<<"ingrese n: ";cin>>n;
    for(i=1;i<=n;i+=2)
    {
        for(j=1;j<=(n-i)/2;j++)
        {
            cout<<" ";
        }
        for(j=1;j<=i;j++)
        {
            cout<<"x";
        }
        cout<<endl;
    }
    for(i=n-2;i>=1;i-=2)
    {
        for(j=1;j<=(n-i)/2;j++)
        {
            cout<<" ";
        }
        for(j=1;j<=i;j++)
        {
            cout<<"x";
        }
        cout<<endl;
    }
    return 0;
}