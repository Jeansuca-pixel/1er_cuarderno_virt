#include <iostream>
using namespace std;

int main() 
{    
    int n,suma;
    cin>>n;
    int A[1000];
    int B[1000];
    int C[1000];
    for ( int i=1;i<=n;i++)
    {
        A[i]=i*10;
        B[i]=i;
        C[i]=A[i]+B[i];
        
    }
    for(int i=1;i<=n;i++)
        {
            //A[i]=i;
            cout<<C[i]<<" ";
        }
    return 0;

}