#include <iostream>
using namespace std;

int main() 
{    
    int n;
    cin>>n;
    int A[1000];
    int C[1000];
    for ( int i=n;i>=1;i--)
    {
        A[i]=i*10;
        
    }
    for ( int i=n;i>=1;i--)
        {
            //A[i]=i;3
            cout<<A[i]<<" ";
        }
    return 0;

}