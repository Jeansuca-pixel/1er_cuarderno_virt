#include <iostream>
using namespace std;
int mayor(int n1, int n2){
    if(n1>n2) return n1;
    else return n2;
    }
int main(){
    int a, b;
    cin>>a>>b;
    cout<<mayor(a, b)<<endl;
    return 0;
}
