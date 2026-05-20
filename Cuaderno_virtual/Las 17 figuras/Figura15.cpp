#include <iostream>
using namespace std;
int main()
{
    int i,j,n;
    cout<<"ingrese n: ";cin>>n;
    for(i=1;i<=n;i++)
    {
        for(j=1;j<=n;j++)
        {
            if(i==j || j==n-i+1)
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