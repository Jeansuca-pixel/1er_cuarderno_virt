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
            cout<<"x";
        }
        cout<<endl;
    }
    return 0;
}