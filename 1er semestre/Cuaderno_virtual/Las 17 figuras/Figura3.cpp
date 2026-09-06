#include<iostream>
using namespace std;
int main()
{
    int i,j,n;
    cout<<"ingrese n: ";cin>>n;
    for(i=1;i<=n;i++)//filas
    {
        for(j=1;j<=n;j++)//columnas
        {
            if(i==1 || i==n || j==n-i+1)
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