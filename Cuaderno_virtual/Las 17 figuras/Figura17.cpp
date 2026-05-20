#include <iostream>
using namespace std;

int main()
{
    int i, j, n, y;
    cout<<"ingrese n: ";cin>>n;
    for (i = 1; i <= n; i += 2)
    {
        y = (n - i) / 2;
        for (j = 1; j <= y; j++)
        {
            cout << " ";
        }
        for (j = 1; j <= i; j++)
        {
            cout << "x";
        }
        cout << endl;
    }
    y = n/2 + 1;
    for (i = 1; i <= n/2 -1 ; i++)
    {
        for (j = 1; j <= n; j++)
        {
            if (j == y)
            {
                cout << "x";
            }
            else
            {
                cout << " ";
            }
        }
        cout << endl;
    }
    for (j = 1; j <= n; j++)
        {
            if (j==y || j==y-1 || j== y+1)
            {
                cout << "x";
            }
            else
            {
                cout << " ";
            }
           
        }
        cout<<endl;
    return 0;
}