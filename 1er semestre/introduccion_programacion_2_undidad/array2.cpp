#include <iostream>
using namespace std;

int main() 
{
    int n,suma;
    cin>>n;
    suma=0;
    int A[1000];
    for ( int i=1;i<=n;i++)
    {
        A[i]=i;
        suma=suma+A[i];
        
    }
    cout<<"la suma es: "<<suma*10<<endl;
    
    for(int i=1;i<=n;i++)
        {
            //A[i]=i;
            cout<<A[i]*10<<" ";
        }
    return 0;

}