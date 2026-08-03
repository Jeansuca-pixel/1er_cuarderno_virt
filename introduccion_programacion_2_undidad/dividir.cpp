#include <iostream>
using namespace std;
int main() {
    #include <iostream>

    int f=3;
    int c=3;
    int suma=0;
    int matriz[f][c]={
        {1,2,3},
        {4,5,6},
        {7,8,9}
    };
   
    cout<<"La matriz es: "<<endl;
    for(int i=0;i<f;i++){    
        for(int j=0;j<c;j++){
            cout<<matriz[i][j]<<" ";
            suma+=matriz[i][j];
        }
        cout<<endl;
    }
    cout<<"La suma de los elementos de la matriz es: "<<suma<<endl;
    return 0;
}