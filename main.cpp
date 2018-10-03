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
void copyList(Node *& cabezaOri, Node *& finOri, Node *& cabezaDest, Node *& finDest);
void freeList(Node *& cabeza, Node *& fin);
bool isNodeInList(Node * cabeza, Node * toSearch);
Node * createNode(const int processTime, int nodeNumber);
Node * getLesserTimeNode(Node * cabeza, Node * fin);
Node * popNode(Node *& cabeza, Node *& fin, Node * toPop);

int mainMenu()
{
	int temp = -1;
	printf("\n	BARBERO DORMILON\n	1. Numero maximo de barberos\n	2. Numero maximo de sillas de espera\n	3. Ingresar procesos en espera\n	4. Limpiar lista\n	5. Simular	\n	0. Salir\n	Seleccion: ");
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
	Node * cabezaEspera, * finEspera, *cabezaBarbero, *finBarbero, *cabezaEsperaCopy, *finEsperaCopy;
	cabezaEspera = finEspera = cabezaBarbero = finBarbero = cabezaEsperaCopy = finEsperaCopy = NULL;
	int op, elapsedTime, nNodos, nNodosCopy, barberNumber, waitQueueNumber, nNodosBarbero;
	elapsedTime = nNodos = nNodosCopy = barberNumber = nNodosBarbero = op = waitQueueNumber = 0;

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
			if (!cabezaEspera)
			{
				cout << "\n	Ingrese el numero maximo de sillas de espera. ";
				waitQueueNumber = pedirNumero(MIN_NUMBER);
			}else
				cout << "\n	Debe limpiar la lista actual primero.\n ";
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
			nNodos = 0;
			freeList(cabezaEspera, finEspera);
			break;
		case 5:
			freeList(cabezaEsperaCopy, finEsperaCopy);
			copyList(cabezaEspera, finEspera, cabezaEsperaCopy, finEsperaCopy);
			initializeSimulation(cabezaEsperaCopy, finEsperaCopy, cabezaBarbero, finBarbero ,barberNumber, waitQueueNumber, elapsedTime= 0, nNodosCopy = nNodos, nNodosBarbero);
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
			while (aux)
			{
				if (aux->bDelete)
				{
					popNode(cabezaBarbero, finBarbero, aux);
					nNodosBarbero--;
				}
 				aux = aux->next;
			}
		}
	}
}

void showList(Node * cabeza, Node * fin, bool useNames) {
	Node * aux = cabeza;
	while (aux)
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
		while (aux)
		{
			elapsedTime += aux->processTime;
			if (lesser->processTime >= aux->processTime)
			{		
				aux->processTime = 0;
				aux->bDelete = true;
			}
			else
				aux->processTime -= lesser->processTime;			
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

	if (item)
	{
		item->next = NULL;
		item->prev = NULL;
	}
	if (!cabeza) {
		cabeza = item;
		fin = item;
	}
	else if(!nextTo) {
		item->next = cabeza;
		cabeza->prev = item;
		cabeza = item;	
	}
	else if (nextTo == fin)
	{
		item->prev = fin;
		fin->next = item;
		fin = item;
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
		while (aux)
		{
			if ((aux->processTime < lesserNode->processTime))
				lesserNode = aux;
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
			if (cabeza == fin)
			{
				cabeza = NULL;
				fin = NULL;
			}
			else if (toPop == cabeza) {
				cabeza = toPop->next;
				cabeza->prev = NULL;
			}
			else if (toPop == fin) {
				fin = toPop->prev;
				fin->next = NULL;
			}
			else {
				Node * sig = toPop->next;
				Node * prev = toPop->prev;
				prev->next = sig;
				sig->prev = prev;
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

void copyList(Node *& cabezaOri, Node *& finOri, Node *& cabezaDest, Node *& finDest) {
	Node * aux = cabezaOri;
	Node * nuevo = NULL;
	while (aux) {
		nuevo = createNode(aux->processTime, aux->nodeArrival);
		pushNode(cabezaDest, finDest, nuevo, finDest);
		aux = aux->next;
	}
}

void freeList(Node *& cabeza, Node *& fin) {
	while (cabeza)
		delete popNode(cabeza, fin, cabeza);
}
