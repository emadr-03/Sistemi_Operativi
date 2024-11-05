#include <iostream>
#include <cmath>

using namespace std;

int main()
{
    cout<<"Calcolatore area di un quadrato"<<endl;
    float numero;
    cout<<"Inserire lato del quadrato: ";
    cin>>numero;
    cout<<"L'area del quadrato è pari a: "<<pow(numero,2);
    return 0;
}
