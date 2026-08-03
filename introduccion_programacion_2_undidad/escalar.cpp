#include <iostream>
using namespace std;

int main() 
{    
    int n,suma,escalar;
    cin>>n;
    cin>>escalar;
    int A[1000];
    int C[1000];
    for ( int i=1;i<=n;i++)
    {
        A[i]=i*10;
        C[i]=A[i]*escalar;
        
    }
    for(int i=1;i<=n;i++)
        {
            //A[i]=i;
            cout<<C[i]<<" ";
        }
    return 0;

}