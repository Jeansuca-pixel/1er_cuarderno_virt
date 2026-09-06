//Metodo Burbuja

#include<iostream>

using namespace std;

int main(){
	int array[5] = {2,3,1,5,4};
	int i,j,aux;
	
	for(i=0;i<5;i++){
		for(j=0;j<4;j++){
			if(array[j] > array[j+1]){
				aux = array[j];//auxiliar es ahora 3 
				array[j] = array[j+1];//array[j] es ahora 1
				array[j+1] = aux;//array[j+1] es ahora 3
                //reodeden ahora serai 2,1,3,5,4
			}
		}
	}
	
	cout<<"Numeros de forma Ascendente: ";
	for(i=0;i<5;i++){
		cout<<array[i]<<" ";
	}
	
	cout<<"\nNumeros de forma Descendente: ";
	for(i=4;i>=0;i--){
		cout<<array[i]<<" ";
	}

		return 0;
}