#include <iostream>
using namespace std;
int main()
{
    int i,j,n;
    cout<<"ingrese n: ";cin>>n;
    for(i=n;i>=1;i--)
    {
        for(j=1;j<=n-i;j++) 
        {
            cout<<" ";
        }
        for(j=1;j<=i;j++) 
        {
            cout<<"x";
        }
        cout<<endl;
    }
}