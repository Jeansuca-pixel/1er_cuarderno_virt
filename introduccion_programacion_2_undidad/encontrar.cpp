#include<iostream>
using namespace std;

int main(){
int A[]={10,20,30,40,50};
int B[]={15,25,35,45,55};
int C[1000];
for(int i=0; i<5; i++)
{
    C[2*i] = A[i];
    C[2*i+1] = B[i];
}
cout << "ingrese la posicion del vector que desea ver: ";
int pos;
cin >> pos;
if(pos<0 || pos>9){
    cout << "Posicion invalida" << endl;
}
else
cout << C[pos] << endl;
return 0;

}