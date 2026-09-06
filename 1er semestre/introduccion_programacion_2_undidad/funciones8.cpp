#include <iostream>
using namespace std;
const int c=3;
void imprime(int array[][c], int n){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cout<<array[i][j]<<" ";
        }
    cout<<endl;
    }
}
int suma(int array[][c], int n){
    int sum=0;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            sum+=array[i][j];
        }
    }
    return sum;
}
int main(){
    const int n=3;
    int A[3][3]={{1,2,3},{4,5,6},{7,8,9}};
    imprime(A, n);
    cout<<"Suma: "<<suma(A, n)<<endl;
    return 0;
}