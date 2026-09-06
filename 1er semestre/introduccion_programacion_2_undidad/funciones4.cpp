#include <iostream>
using namespace std;
int multiplicacion(int n1, int n2){
    if(n2==0) return 0;
    int cont=0;
    for(int i=0; i<n2; i++){
        cont+=n1;
    }
    return cont;
}
int main(){
    int a, b;
    cin>>a>>b;
    cout<<multiplicacion(a, b)<<endl;
    return 0;
}
