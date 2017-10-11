#include <iostream>
#include "rmlib.h"

using namespace std;

int main()
{

    char num1 = 't';
    void* ptr1 = &num1;

    double num2 = 2.34;
    void* ptr2 = &num2;

    int num3 = 3598;
    void* ptr3 = &num3;

    int num4 = 852;
    void* ptr4 = &num4;

    char* a = "a1";

    string gato = serialize(ptr2, sizeof(double));

    rm_init("127.0.0.1", 10599, "", 4464);
    rm_new(a, ptr1, sizeof(char));
    rm_new("a2", ptr2, sizeof(double));
    rm_new("a3", ptr3, sizeof(int));
    rm_new("a4", ptr2, sizeof(double));
    rm_new("a5", ptr1, sizeof(char));
    rm_new("a6", ptr2, sizeof(double));
    rm_new("a7", ptr3, sizeof(int));
    rm_new("a8", ptr2, sizeof(double));
    rm_new("GOKU", ptr4, sizeof(int));

    rmRef_H prueba = rm_get("a1");

    rmRef_H prueba1 = rm_get("a2");

    rmRef_H prueba2 = rm_get("a3");

    rmRef_H prueba3 = rm_get("a4");

    rmRef_H prueba4 = rm_get("a5");

    rmRef_H prueba5 = rm_get("a6");

    rmRef_H prueba6 = rm_get("a7");

    rmRef_H prueba7 = rm_get("a8");

    rmRef_H Goku = rm_get("GOKU");


    cout << prueba.get_key() << "," << prueba.get_value() <<  endl;

    cout << prueba2.get_key() << "," << prueba2.get_value() <<  endl;

    cout << prueba3.get_key() << "," << prueba3.get_value() <<  endl;

    cout<< "Pruebas////////////////////////////////////////////////// " <<endl;

    cout <<  "Prueba " <<(*((char*) prueba.get_value())) << endl;

    cout <<  "PRUEBA1 " <<(*((int*) prueba1.get_value())) << endl;

    cout <<  "PRUEBA2 " <<(*((double*) prueba2.get_value())) << endl;

    cout <<  "PRUEBA3 " <<(*((double*) prueba3.get_value())) << endl;

    cout <<  "Prueba4 " <<(*((char*) prueba4.get_value())) << endl;

    cout <<  "PRUEBA5 " <<(*((int*) prueba5.get_value())) << endl;

    cout <<  "PRUEBA6 " <<(*((double*) prueba6.get_value())) << endl;

    cout <<  "PRUEBA7 " <<(*((double*) prueba7.get_value())) << endl;

    cout <<  "GOKU " <<(*((int*) Goku.get_value())) << endl;

    rm_delete(&Goku);

    cout << ( prueba1 == prueba3 )<< endl;

    cout << ( prueba != prueba3 )<< endl;

    (prueba = prueba7);

    cout << prueba.get_key() << "," << prueba.get_value() <<"," << prueba.get_size()<<  endl;
    cout << prueba7.get_key() << "," << prueba7.get_value() <<"," << prueba7.get_size()<<  endl;

    cout << ( prueba == prueba7 )<< endl;



    return 0;
}
