#include <iostream>
#include "rmlib.h"

using namespace std;

int main()
{
	rm_init("127.0.0.1", 10599, "", 4464);

	int numEstudiantes;
	string estudiante;
	string estudiante1;
	char est[254];
	char est1[254];
	vector<string> estudiantes;
	int calificacion;
	void* ptr;
	bool x = true;

	int op1;
	//string op2;
	//string op3;
	cout<<"Que desea realizar?"<<endl;
	cout<<"1. Agregar"<<endl;
	cout<<"2. Obtener"<<endl;
	cout<<"3. Eliminar"<<endl;
	cout<<"4. Comparar"<<endl;
	cout<<"5. Copiar"<<endl;
	cout<<"6. Salir"<<endl;
	cout<<endl;

	while(x){
		
		cout<<"Digite su opcion: ";
		cin>>op1;
		cout<<endl;

		if(op1==1){

			cout<<"Numero de estudiantes que desea insertar"<<endl;
			cin>>numEstudiantes;
			cout<<endl;

			for(int i=0; i<numEstudiantes; i++){
				cout<<"Digite el nombre del estudiante"<<endl;
				cin>>estudiante;
				cout<<endl;
				strcat(est, estudiante.c_str());
				//estudiantes.push_back( estudiante );
				cout<<"Digite la calificacion"<<endl;
				cin>>calificacion;
				cout<<endl;
				ptr = &calificacion;
				rm_new(est, ptr, sizeof(int));
				cout<<endl;

				est[0] ='\0';

			}

		}

		if(op1==2){

			cout<<"De cual alumno desea obtener la nota"<<endl;
			cin>>estudiante;
			cout<<endl;
			strcat(est, estudiante.c_str());

			rmRef_H estructura = rm_get(est);
			cout <<  estudiante <<" su nota es: " <<(*((int*) estructura.get_value())) << endl;
			cout<<endl;
			est[0] ='\0';
		}

		if(op1==3){

			cout<<"Cual alumno desea eliminar"<<endl;
			cin>>estudiante;
			cout<<endl;
			strcat(est, estudiante.c_str());

			rmRef_H estructura = rm_get(est);
			rm_delete(&estructura);
			cout<<endl;
			est[0] ='\0';
		}

		if(op1==4){
			
			int opccion;
			cout<<"como desea comparar? "<<endl;
			cout<<"1. == "<<endl;
			cout<<"2. != "<<endl;
			cout<<endl;
			cin>>opccion;

			if(opccion == 1){

				cout<<"Digite el nombre del estudiante 1"<<endl;
				cout<<"Digite el nombre del estudiante 2"<<endl;
				cout<<endl;
				cin>>estudiante;
				cin>>estudiante1;
				cout<<endl;
				strcat(est, estudiante.c_str());
				strcat(est1, estudiante1.c_str());
				rmRef_H estructura = rm_get(est);
				rmRef_H estructura1 = rm_get(est1);
				cout<<(estructura1 == estructura1)<<endl;
				cout<<endl;
				est[0] ='\0';
				est1[0] ='\0';
			}
			else if(opccion == 2){

				cout<<"Digite el nombre del estudiante 1"<<endl;
				cout<<"Digite el nombre del estudiante 2"<<endl;
				cout<<endl;
				cin>>estudiante;
				cin>>estudiante1;
				cout<<endl;
				strcat(est, estudiante.c_str());
				strcat(est1, estudiante1.c_str());
				rmRef_H estructura = rm_get(est);
				rmRef_H estructura1 = rm_get(est1);
				cout<<(estructura1 != estructura1)<<endl;
				cout<<endl;
				est[0] ='\0';
				est1[0] ='\0';
			}
		}

		if(op1 == 5){

			cout<<"Digite el nombre del estudiante 1"<<endl;
			cout<<"Digite el nombre del estudiante 2"<<endl;
			cout<<endl;
			cin>>estudiante;
			cin>>estudiante1;
			cout<<endl;
			strcat(est, estudiante.c_str());
			strcat(est1, estudiante1.c_str());
			rmRef_H estructura = rm_get(est);
			rmRef_H estructura1 = rm_get(est1);
			estructura1 = estructura1;
			cout<<endl;
			est[0] ='\0';
			est1[0] ='\0';
		}

		if(op1 == 6){

			x= false;
		}



	}
}