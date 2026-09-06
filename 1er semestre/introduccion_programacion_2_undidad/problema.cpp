#include <iostream>
using namespace std;

int main() 
{
    int n,k;
    cin>>n;
    int A[10];
    for ( int i=0 ;i<n;i++)
    {
        for(int j=1;j<=3;j++)
        {
            A[k]=i;
            k=k+1;
        }
        
    }
    for(int i=0;i<=n;i++)
        {
            //A[i]=i;
            cout<<A[i]<<" ";
        }
    return 0;
}