#include <iostream>
using namespace std;
int potencia(int n1, int n2){
    if(n2==0) return 1;
    int cont=1;
    for(int i=0; i<n2; i++){
        cont*=n1;
    }
    return cont;
}
int main(){
    int a, b;
    cin>>a>>b;
    cout<<potencia(a, b)<<endl;
    return 0;
}
