#include <iostream>
using namespace std;

int main()
{
    int A[]={10,20,30,40,50};
    int B[]={15,25,35,45,55};
    int C[1000];

    for(int i=0; i<5; i++)
    {
        C[i] = A[i];
        C[i+5] = B[i];
    }

    for(int i=0; i<10; i++)
    {
        cout << C[i] << " ";
    }

    return 0;
}   