#include<iostream>
#include <cstdlib>
#include <ctime> 
#include<cmath>
using namespace std;
int main ()
{
    /*{
        int a,b;
        cin>>a>>b;
        cout<<(a < 0 and b > 0)<<endl;
    }
    {
        int a;
        cin>>a;
        cout<<(not a)<<endl;
    }*/
   /*int opcion;
   while (opcion != 0){
    {
        int x,y;
        cout<<"ingrese (x) y (y): "<<endl;
        cin>>x>>y;
        if (x>0 and y>0)
            cout<<"I"<<endl;
        else if(x<0 and y>0)
            cout<<"II"<<endl;
        else if(x<0 and y<0)
            cout<<"III"<<endl;
        else if(x>0 and y<0)
            cout<<"IV"<<endl;
    }}*/
   /*{
    int a,b;
    cout<<"Ingresar edades: ";
    cin>>a>>b;
    (a>b)? cout<<"A es mayot":cout<<"B es mayor"<<endl;
   }*/
  {
    srand(time(0));
    int yo,pc;
    cout<<"piedra=1,papel=2,tijera=3"<<endl;
    cin>>yo;
    pc = rand() % 3 + 1;
        cout << "La computadora eligió: "<<pc;
    if (yo==pc)
        cout<<"empate"<<endl;
    else if 
    (
        (yo==1 and pc==3) cout<<"Ganaste"<<endl;
        (yo==2 and pc==1) cout<<"Ganaste"<<endl;
        (yo==3 and pc==2) cout<<"Ganaste"<<endl;
    )
  }
    return 0;
}    