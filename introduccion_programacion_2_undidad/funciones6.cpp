#include <iostream>
using namespace std;
void imprime(int array[], int n){
    for(int i=0; i<n; i++)
        cout<<array[i]<<" ";
}
int main(){
    const int n=5;
    int A[5]={1,2,3,4,5};
    imprime(A, n);
    return 0;
}