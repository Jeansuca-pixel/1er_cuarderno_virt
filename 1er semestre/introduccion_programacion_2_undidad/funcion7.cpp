#include <iostream>
using namespace std;
void imprime(int array[], int n){
    for(int i=0; i<n; i++)
        cout<<array[i]<<" ";
}
int suma(int array[], int n){
    int sum=0;
    for(int i=0; i<n; i++)
        sum+=array[i];
    return sum;
}
int main(){
    const int n=5;
    int A[5]={1,2,3,4,5};
    imprime(A, n);
    cout<<"Suma: "<<suma(A, n)<<endl;
    return 0;
}