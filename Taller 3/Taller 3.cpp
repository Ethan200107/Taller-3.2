// Taller 3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include "Usuario.h"
#include <random>
using namespace std;

vector<string> currentDateTime() {
	// Obtener el tiempo actual
	auto ahora = chrono::system_clock::now();

	// Convertir el tiempo a una estructura time_t
	time_t tiempo_actual = chrono::system_clock::to_time_t(ahora);

	// Crear una estructura tm para almacenar la hora local
	tm tiempo_local;
	vector<string> nulo;
	// Usar localtime_s en lugar de localtime por que da error
	if (localtime_s(&tiempo_local, &tiempo_actual) != 0) {
		cerr << "Error al obtener la hora local" << std::endl;
		return nulo;
	}

	// Crear un stream para formatear la fecha y la hora
	ostringstream oss;
	oss << put_time(&tiempo_local, "%d-%m-%Y %H:%M:%S");

	// Devolver la fecha y la hora como una cadena
	string fechaHora = oss.str();
	
	string token; // token para poder obtener el string

	istringstream iss(fechaHora); // llamamos al istingstream para poder separarlo

	vector<string> Datos; // donde vamos a guardar todos los datos del Cliente

	while (getline(iss, token, ' ')) { // hacemos todas las separaciones y las guardamos
		Datos.push_back(token);
	}
	return Datos;
}

bool verificarExistencia(Usuario* nuevo,vector<Usuario*> ListaCliente) {
	for (int i = 0; i < ListaCliente.size(); i++) {
		string nom = ListaCliente[i]->getNombre();
		if (nuevo->getNombre() == nom) {
			return false;
		}
	}       return true;

}

bool verificarExistencia(string nombre , string contrasena, vector<Usuario*> ListaCliente) {
	for (int i = 0; i < ListaCliente.size(); i++) {
		string nom = ListaCliente[i]->getNombre();
		string cont = ListaCliente[i]->getContrasena();
		if (nombre == nom && contrasena == cont ) {
			return true;
		}
	}       return false;

}

vector<Usuario*> lecturaCuentas() {
	vector<Usuario*> ListaCliente;
	ifstream archivo("cuentas.txt");
	if (!archivo.is_open()) {
		cerr << "No se pudo abrir el archivo." << std::endl;
		return ListaCliente;
	}
	string linea;
	while (getline(archivo, linea)) {
		string token; // token para poder obtener el string

		istringstream iss(linea); // llamamos al istingstream para poder separarlo

		vector<string> DatosClientes; // donde vamos a guardar todos los datos del Cliente

		while (getline(iss, token, ',')) { // hacemos todas las separaciones y las guardamos

			DatosClientes.push_back(token);
		}

		string nombre = DatosClientes[0];
		string contrasena = DatosClientes[1];

		Usuario* nuevo = new Usuario(nombre, contrasena);
		bool bandera = verificarExistencia(nuevo, ListaCliente);
		if (bandera) {
			ListaCliente.push_back(nuevo);
		}
		else
		{
			// cout << "Usuario Duplicado" << endl;
		}
	}
	return ListaCliente;
}

vector<Usuario*> crearNuevoUsuario(vector<Usuario*> cuentasL) {
	cout << "***** Nuevo Usuario ***** " << endl;
	cout << endl;
	cout << "Rut de ejemplo -> 12345678-9" << endl << endl;
	string nombre, contrasena;
	Usuario* nuevo;
	do {
		cout << "Rut: "; cin >> nombre;
		cout << "Contrase�a: "; cin >> contrasena;
		cout << endl;
		nuevo = new Usuario(nombre, contrasena);
		if (!verificarExistencia(nuevo, cuentasL)) {
			cout << "rut ya ingresado por favor cree una cuenta nueva" << endl;
		}
	} while (!verificarExistencia(nuevo, cuentasL));
	
	
	cuentasL.push_back(nuevo);
	cout << " ----- Nuevo Usuario Registrado -----" << endl;
	
	string linea = nombre + "," + contrasena;

	// Abrir el archivo en modo de adjuntar para agregar nuevos datos al final
	ofstream archivo("cuentas.txt", ios::app);

	if (archivo.is_open()) {
		// Escribir una nueva l�nea en el archivo
		archivo << "\n" << linea;

		// Cerrar el archivo
		archivo.close();
	}
	else {
		cerr << "No se pudo abrir el archivo." << endl;
	}

	cout << "Presione Enter para continuar..." << endl;
	system("pause");

	return cuentasL;
}

Usuario* ObtenerUsuario(string nombre, vector<Usuario*> CuentaL) {
	for (int i = 0; i < CuentaL.size(); i++) {
		string nom = CuentaL[i]->getNombre();
		if (nombre == nom) {
			return CuentaL[i];
		}
	}       return nullptr;

}

Usuario* LecturaDatosUsuario(string nombre,vector<Usuario*> cuentasL) {
	Usuario* actual = ObtenerUsuario(nombre, cuentasL);
	
	ifstream archivo("DataClientes.txt");
	if (!archivo.is_open()) {
		cerr << "No se pudo abrir el archivo." << std::endl;
		return actual;
	}
	
	string linea;
	while (getline(archivo, linea)) {
		string token; // token para poder obtener el string

		istringstream iss(linea); // llamamos al istingstream para poder separarlo

		vector<string> Datos; // donde vamos a guardar todos los datos del Cliente

		while (getline(iss, token, ',')) { // hacemos todas las separaciones y las guardamos

			Datos.push_back(token);
		}
		//      0        1             2          4      5
		string ID, CuentaOrigen, CuentaDestino, Fecha, Hora;
		//    3
		int monto;

		ID = Datos[0];
		CuentaOrigen = Datos[1];
		CuentaDestino = Datos[2];
		monto = stoi(Datos[3]);
		Fecha = Datos[4];
		Hora = Datos[5];

		Transferencia* nueva = new Transferencia(ID,CuentaOrigen,CuentaDestino,monto,Fecha,Hora);
		
		int montoactual = actual->getMonto();
		if (actual->getNombre() == CuentaOrigen ) {
			monto = monto * -1;
			nueva->setMonto(monto);
			actual->ingresarTransferencia(nueva);
			actual->setMonto((montoactual + monto));
		}
		else if(actual->getNombre() == CuentaDestino) {
			actual->ingresarTransferencia(nueva);
			actual->setMonto((montoactual + monto));
		}
		else {
			nueva->~Transferencia();
		}

	}
	// balancear el arbol
	return actual;
}

void Depositar(Usuario* actual) {

	cout << "**** Deposito ****" << endl << endl;
	cout << "rut de ejemplo -> 12345678-9" << endl << endl;

	string cuentaDestino;
	cout << "Cuenta de destino: "; cin >> cuentaDestino;

	int monto = 0;
	do {
		cout << "Monto: "; cin >> monto;
		cout << endl;
		if (monto > actual->getMonto()) {
			cout << " **** Excede el monto en la cuenta ****" << endl;
			cout << "Monto de la cuenta : " << actual->getMonto() << endl;
		}
	} while (monto > actual->getMonto());

	actual->setMonto(actual->getMonto() - monto);

	cout << "Para verificar monto y depositar, ingresar la contrase�a de la cuenta." << endl;
	string contra;
	do {
		cout << "Contrase�a: "; cin >> contra;
		if (contra != actual->getContrasena()) {
			cout << "Contrase�a incorrecta ingresar de nuevo" << endl;
		}
	} while (contra != actual->getContrasena());


	string cuentaOrigen = actual->getNombre();
	string fecha, hora;
	vector<string> FH = currentDateTime();
	fecha = FH[0];
	hora = FH[1];

	// Crear un generador de n�meros aleatorios
	random_device rd;
	mt19937 gen(rd()); // Mersenne Twister 19937 como motor

	// Definir la distribuci�n de n�meros aleatorios
	uniform_int_distribution<int> dis(1000, 10000000); // N�meros entre 1 y 100

	// Generar un n�mero aleatorio
	int ID = dis(gen);

	// vamos a generar una boleta 
	cout << endl << "----------------------------" << endl;
	cout << "    ****** Boleta ******    " << endl << endl;
	cout << "ID: " << ID << endl;
	cout << "Cuenta de origen: " << cuentaOrigen << endl;
	cout << "Cuenta de destino: " << cuentaDestino << endl;
	cout << "Monto: $" << monto << endl;
	cout << "Fecha: " << fecha << endl;
	cout << "Hora: " << hora << endl;
	cout << "----------------------------" << endl << endl;

	Transferencia* nueva = new Transferencia(to_string(ID), cuentaOrigen, cuentaDestino, monto*-1, fecha, hora);

	actual->ingresarTransferencia(nueva);

	string linea = to_string(ID) + "," + cuentaOrigen + "," + cuentaDestino + "," + 
		to_string(monto) + "," + fecha + "," + hora ;

	// Abrir el archivo en modo de adjuntar para agregar nuevos datos al final
	ofstream archivo("DataClientes.txt", ios::app);

	if (archivo.is_open()) {
		// Escribir una nueva l�nea en el archivo
		archivo << "\n" << linea;

		// Cerrar el archivo
		archivo.close();
	}
	else {
		std::cerr << "No se pudo abrir el archivo." << std::endl;
	}
}

void buscarPorID(Usuario* actual) {
	int opcion;
	string ID;
	do{
	cout << "**** Buscar por ID" << endl;
	cout << "1) Ver ID's" << endl;
	cout << "2) Ingresar ID " << endl;
	cout << "3) Salir" << endl;
	cout << "opcion: "; cin >> opcion;

	switch (opcion)
	{
	case 1:
		actual->VerID(actual->getArbol());
		system("pause");
		break;
	case 2:
		cout << "Ingresar ID : "; cin >> ID;
		actual->ObtenerBoltaID(actual->getArbol(), ID);
		system("pause");
		break;
	default:
		break;
	}
	system("cls");
	} while (opcion != 3);

}

void buscarDeposito(Usuario* actual) {
	int opcion;
	do {
		cout << "**** Busqueda de deposito ****" << endl << endl;
		cout << "1) Buscar todos los depositos que llegaron a la cuenta" << endl;
		cout << "2) Buscar todos los depositos que salieron de la cuenta" << endl;
		cout << "3) Ver todos los depositos" << endl;
		cout << "4) Buscar Deposito por ID" << endl;;
		cout << "5) Salir" << endl;
		cout << "Opcion: "; cin >> opcion;
		
		system("cls");
		switch (opcion)
		{
		case 1:
			actual->VerDepositosOrdenados(actual->getArbol(), 0);
			break;
		case 2:
			actual->VerDepositosOrdenados(actual->getArbol(), 1);
			break;
		case 3:
			actual->VerDepositosOrdenados(actual->getArbol(), 2);
			break;
		case 4:
			buscarPorID(actual);
			break;
		case 5:break;
		default:
			break;
		}
		cout << "Presione Enter para continuar..." << endl;
		system("pause");
		
		system("cls");
	} while (opcion != 5);


}

void informe(Usuario* actual) {
	int opcion;
	string ingresarA , ingresarM;
	vector<string> aux;
	do {
		cout << "***** Menu de informes *****" << endl;
		cout << "1) ver informe Anual" << endl;
		cout << "2) Ver informe mensual" << endl;
		cout << "3) Salir" << endl;

		cout << "opcion: "; cin >> opcion;
		switch (opcion)
		{
		case 1:
			aux = actual->mostrarAnos();
			cout << "A�o a ver: "; cin >> ingresarA;
			cout << endl;
			if(!actual->existeEnVector(aux,ingresarA)){
			}
			else {
				actual->informeAnual(actual->getArbol(), ingresarA, 0);
			}
			break;
		case 2:
			aux = actual->mostrarAnos();
			cout << "A�o a ver: "; cin >> ingresarA;
			cout << endl;
			if (!actual->existeEnVector(aux, ingresarA)) {
				cout << "No existen datos de ese a�o" << endl;
			}
			else {
				aux = actual->mostrarMeses(ingresarA);
				
				cout << "Mes a ver dentro de " << ingresarA << ": "; cin >> ingresarM;
				
				if (actual->existeEnVector(aux, ingresarM)) {
					actual->informeMensual(actual->getArbol(), ingresarA,ingresarM);
				}
				else {
					cout << "No existen datos de ese mes" << endl;
				}
			
			}
			break;
		case 3: break; default:break;} // estas lineas son para nada (estan de lujo)
		system("pause");
		system("cls");
	} while (opcion != 3);
	
}

void menu(Usuario* actual) {
	while (true) {
		cout << "*******  Banco UCN  *******" << endl << endl;
		cout << " " << " Monto " << endl;
		cout << "| $" << actual->getMonto() << " |" << endl << endl;
		cout << "1) Revisar historial de depositos" << endl;
		cout << "2) Depositar" << endl;
		cout << "3) Buscar deposito" << endl;
		cout << "4) Evaluar posibles estafas" << endl;
		cout << "5) Actividad financiera" << endl;
		cout << "6) Salir" << endl << endl;

		int opcion;

		cout << "Opcion: "; cin >> opcion;

		system("cls");
		if (opcion == 6){ break; }

		switch (opcion)
		{
		case 1:
			actual->VerDepositosOrdenados(actual->getArbol(), 2);
			system("pause");
			break;
		case 2:
			if (0 == actual->getMonto()) { cout << "No tienes fondos o dinero dentro de esta cuenta" << endl; }
			else { Depositar(actual); }
			cout << "Presione Enter para continuar..." << endl;
			system("pause");
			break;
		case 3:
			buscarDeposito(actual);
			break;
		case 4:
			actual->evaluarestafas(actual->getArbol());
			system("pause");
			break;
		case 5:
			informe(actual);
			break;
		default:

			break;
		}
		
		
		system("cls");
	}
}

void ingreso(vector<Usuario*> cuentasL) {
	string nombre, contrasena , verificacion;
	while (true) {
		cout << "***** BIENVENIDO *****" << endl;
		cout << "1) Ingresar" << endl;
		cout << "2) Nuevo usuario" << endl;
		cout << "3) Salir" << endl;
		int opcion;
		cout << "Opcion: "; cin >> opcion;
		cout << endl;
		system("cls");
		switch (opcion)
		{
		case 1:
			while (true) {
				cout << "***** INGRESO *****" << endl << endl;
				cout << "Rut de ejemplo -> 12345678-9" << endl << endl;
				cout << "Rut: "; cin >> nombre; cout << endl;
				cout << "Contase�a: "; cin >> contrasena; cout << endl;
				bool bandera = verificarExistencia(nombre, contrasena, cuentasL);
				if (bandera) {
					cout << endl;
					Usuario* actual = LecturaDatosUsuario(nombre, cuentasL);
					
					if (actual->getMonto() <  0) {
						cout << " Cuenta bloqueada por deuda " << endl;
						cout << " Deuda de: $" << actual->getMonto()*-1 << endl;
						cout << " Por favor acercarse a su banco mas cercano" << endl << endl;
						break;
					}
					system("cls");
					menu(actual);
					break;
				}
				else {
					cout << "Usuario o contrae�a incorrecto" << endl;
					cout << "Desea Ingresar nuevo usuario? (Si/No) : ";
					cin >> verificacion;
					cout << endl;
					if (verificacion == "Si") {
						cuentasL = crearNuevoUsuario(cuentasL);
					}
					system("cls");

				}



			}
			break;
		case 2:
			cuentasL = crearNuevoUsuario(cuentasL); // hacer una verificacion por cuentas clonadas
			break;
		case 3:
			// los datos se guardan automaticamente
			exit(0);
		default:
			break;
		}
		system("cls");
	}
}

int main()
{
    vector<Usuario*> cuentasL =  lecturaCuentas();
    ingreso(cuentasL);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
