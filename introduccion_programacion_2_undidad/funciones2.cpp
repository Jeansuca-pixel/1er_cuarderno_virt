#include <iostream>
using namespace std;
int factorial(int n1){
    if(n1==0) return 1;
    return n1*factorial(n1-1);
}
int main(){
    int a;
    cin>>a;
    cout<<factorial(a)<<endl;
    return 0;
    
}
