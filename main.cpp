#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>

using namespace std;

constexpr int MIN_NUMBER = 1;

struct Node {
	int processTime;
	int nodeArrival;
	Node * next;
	Node * prev;
	bool bDelete;
};

void showElapsedTime(const int elapsedTime);
void showList(Node * cabeza, Node * fin, bool useNames = false);
void burstNodeInBarberTime(Node * cabeza, Node * fin, int & elapsedTime);
void showBarberNumber(int & barberNumber);
void showWaitQueueNumber(int & waitQueueNumber);
void initializeSimulation(
	Node *& cabezaEspera, Node *& finEspera, Node *& cabezaBarbero, Node *& finBarbero,  
	const int barberNumber, const int waitQueueNumber,  int & elapsedTime, 
	int & nNodosEspera, int & nNodosBarbero);
void pushNode(Node *& cabeza, Node *& fin, Node *& item, Node * nextTo);
bool isNodeInList(Node * cabeza, Node * toSearch);
Node * createNode(const int processTime, int nodeNumber);
Node * getLesserTimeNode(Node * cabeza, Node * fin);
Node * popNode(Node *& cabeza, Node *& fin, Node * toPop);

int mainMenu()
{
	int temp = -1;
	printf("\n	BARBERO DORMILON\n	1. Numero maximo de barberos\n	2. Numero maximo de sillas de espera\n	3. Ingresar procesos en espera\n	4. Simular	\n	0. Salir\n	Seleccion: ");
	cin >> temp;
	if (cin.fail())
	{
		cin.clear();
		cin.ignore();
	}
	return temp;
}

int pedirNumero(int minNumero)
{
	int num = 0;
	do {
		cin.clear();
		cin.ignore(256, '\n');
		printf("\n	Ingrese numero: ");
		cin >> num;
	} while (cin.fail() || num < minNumero);
	return num;
}

int main()
{
	Node * cabezaEspera, * finEspera, *cabezaBarbero, *finBarbero;
	cabezaEspera = finEspera = cabezaBarbero = finBarbero = NULL;
	int op, elapsedTime, nNodos, barberNumber, waitQueueNumber, nNodosBarbero;
	elapsedTime = nNodos = barberNumber = nNodosBarbero = op = waitQueueNumber = 0;

	do {
		system("CLS");
		if (cabezaEspera || barberNumber >= MIN_NUMBER || waitQueueNumber >= MIN_NUMBER || elapsedTime) {
			printf("\n	============ PARAMETROS LISTA BARBERO DORMILON ============	\n\n");
			if (barberNumber >= MIN_NUMBER)
				showBarberNumber(barberNumber);
			if (waitQueueNumber >= MIN_NUMBER)
				showWaitQueueNumber(waitQueueNumber);
			if (elapsedTime >= MIN_NUMBER)
				showElapsedTime(elapsedTime);
			if (cabezaEspera)
			{
				cout << "\n	====== Lista de procesos en espera ======\n\n";
				showList(cabezaEspera, finEspera, true);
				cout << "\n\n	====== Fin Lista procesos en espera ======\n\n";
			}
			printf("\n	============ FIN PARTAMETROS BARBERO DORMILON ============	\n\n");
		}
		switch (op = mainMenu())
		{
		case 1:
			cout << "\n	Ingrese el numero maximo de barberos. ";
			barberNumber = pedirNumero(MIN_NUMBER);
			break;
		case 2: 
			cout << "\n	Ingrese el numero maximo de sillas de espera. ";
			waitQueueNumber = pedirNumero(MIN_NUMBER);
			break;
		case 3:
		{
			if (waitQueueNumber <= 0)
			{
				cout << "\n	Debe ingresar un numero maximo de sillas.\n	";
				break;
			}

			if (nNodos < waitQueueNumber)
			{
				cout << "\n	Ingrese tiempo de nuevo proceso. \n";
				Node * item = createNode(pedirNumero(MIN_NUMBER), ++nNodos);
				pushNode(cabezaEspera, finEspera, item, finEspera);
			}
			else
				cout << "\n	Ya ingreso el maximo numero de procesos en espera.\n";			
		}
		break;
		case 4:
			initializeSimulation(cabezaEspera, finEspera, cabezaBarbero, finBarbero ,barberNumber, waitQueueNumber, elapsedTime, nNodos, nNodosBarbero);
			break;
		}
		printf("\n");
		system("PAUSE");
	} while (op != 0);
	return 0;
}

void initializeSimulation(
	Node *& cabezaEspera, Node *& finEspera, Node *& cabezaBarbero, Node *& finBarbero,
	const int barberNumber, const int waitQueueNumber, int & elapsedTime,
	int & nNodosEspera, int & nNodosBarbero)
{
	if (barberNumber <= 0)
	{
		cout << "\n	Debe ingresar un numero maximo de barberos.\n	";
		return;
	}

	if (waitQueueNumber <= 0)
	{
		cout << "\n	Debe ingresar un numero maximo de sillas.\n	";
		return;
	}
	
	while ((nNodosEspera > 0 || nNodosBarbero > 0) && (cabezaEspera || cabezaBarbero))
	{
		Node * pFront;

		while ((nNodosBarbero < barberNumber) && (cabezaEspera))
		{
			pFront = popNode(cabezaEspera, finEspera, cabezaEspera);
			pushNode(cabezaBarbero, finBarbero, pFront, finBarbero);  //ARREGLAR
			nNodosBarbero++;
			nNodosEspera--;
		}

		if (cabezaBarbero)
		{
			printf("\n	========= Barberos ========	\n");
			showList(cabezaBarbero, finBarbero, true);
			printf("\n	========= Baberos ========	\n");
		}
		if (cabezaEspera)
		{
			printf("\n	========= Espera =======	\n");
			showList(cabezaEspera, finEspera, true);
			printf("\n	========= Fin Espera =======	\n");
		}

		burstNodeInBarberTime(cabezaBarbero, finBarbero, elapsedTime);

		{
			Node * aux = cabezaBarbero;
			Node * aBorrar = NULL;
			bool continuar = true;

			while (continuar && cabezaBarbero)
			{
				if (aux->bDelete)
				{
					aBorrar = aux;
					aux = aux->prev;
					popNode(cabezaBarbero, finBarbero, aBorrar);
					nNodosBarbero--;
				}
				if (aux == finBarbero)
					continuar = !continuar;
				aux = aux->next;
			}
		}
	}
}

void showList(Node * cabeza, Node * fin, bool useNames) {
	Node * aux = cabeza;
	bool continuar = true;
	while (continuar && cabeza)
	{
		if (useNames)
			cout << "	" << "P" << aux->nodeArrival << "(";
		else
			cout << "	";
		cout << aux->processTime;
		if (useNames)
			cout << ")	";
		else
			cout << "	";
		if (aux == fin)
			continuar = false;
		aux = aux->next;
	}
}

void showElapsedTime(const int elapsedTime)
{
	cout << "\n	Tiempo transcurrido (Ultima simulacion): " << elapsedTime << " /s\n";
}

void burstNodeInBarberTime(Node * cabeza, Node * fin, int & elapsedTime)
{
	if (Node * lesser = getLesserTimeNode(cabeza, fin)) {
		Node * aux = cabeza;
		bool continuar = true;
		while (continuar && aux)
		{
			elapsedTime += aux->processTime;
			if (lesser->processTime >= aux->processTime)
			{		
				aux->processTime = 0;
				aux->bDelete = true;
			}
			else
				aux->processTime -= lesser->processTime;

			if (aux == fin)
				continuar = false;
			aux = aux->next;
		}
	}
}

void showBarberNumber(int & barberNumber)
{
	cout << "\n	Numero de barberos: " << barberNumber << " barbero(s)\n";
}

void showWaitQueueNumber(int & waitQueueNumber)
{
	cout << "\n	Numero de sillas de espera: " << waitQueueNumber << " silla(s)\n";
}

void pushNode(Node *& cabeza, Node *& fin, Node *& item, Node * nextTo = NULL)
{
	if (!cabeza) {
		cabeza = item;
		fin = item;
		item->next = item;
		item->prev = item;
	}
	else if(!nextTo || nextTo == fin) {
		item->next = cabeza;
		fin->next = item;
		if (!nextTo)
		{
			cabeza->prev = item;
			cabeza = item;
		}
		else if (nextTo == fin)
		{
			item->prev = fin;
			fin = item;
		}
	}
	else
	{
		item->next = nextTo->next;
		item->prev = nextTo;
		item->next->prev = item;
		nextTo->next = item;
	}
}

Node * createNode(const int processTime, int nodeNumber)
{
	Node * nuevo = (Node *)malloc(sizeof(Node));
	nuevo->bDelete = false;
	nuevo->next = NULL;
	nuevo->prev = NULL;
	nuevo->nodeArrival = nodeNumber;
	nuevo->processTime = processTime;
	return nuevo;
}

Node * getLesserTimeNode(Node * cabeza, Node * fin) {
	
	Node * lesserNode = cabeza;
	if (cabeza)
	{
		Node * aux = cabeza;
		bool continuar = true;
		while (continuar)
		{
			if ((aux->processTime < lesserNode->processTime))
				lesserNode = aux;
			if (aux == fin)
				continuar = !continuar;
			aux = aux->next;
		}
	}
	return lesserNode;
}

Node * popNode(Node *& cabeza, Node *& fin, Node * toPop) {
	if (cabeza)
	{
		if (isNodeInList(cabeza, toPop))
		{
			if (toPop->next == toPop && toPop->prev == toPop)
			{
				cabeza = NULL;
				fin = NULL;
			}
			else {
				if (toPop == cabeza)
					cabeza = toPop->next;
				else if (toPop == fin)
					fin = toPop->prev;
				fin->next = cabeza;
				cabeza->prev = fin;
			}
			return toPop;
		}
	}
	return NULL;
}

bool isNodeInList(Node * cabeza, Node * toSearch) {
	Node * aux = cabeza;
	while (aux) {
		if (aux == toSearch)
			return true;
		aux = aux->next;
	}
	return false;
}
