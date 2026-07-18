#include <string.h>
#include <iostream>
using namespace std;
//LUIS LARA Y GABRIEL FREAY

#define MAX_VENTAS 1000
struct Sventa {
    int num_operacion;
    int codigo_producto;
    int cantidad;
    float precio_unidad;
    float monto;
    int fecha;                //formato AAAAMMDD
    
    struct Sventa* prventas; 
};

struct Sasociado {
    int codigo;              
    char nombre[50];
    char direccion[100];
    char telefono[21]; 
    
    struct Sasociado* pnext; 
    struct Sventa* pventas;  
};

struct Sproducto {
    int codigo;
    float precio;
    char nombre[30];
    char marca[30];
    char descripcion[100];
    Sproducto* psig;
};

bool BuscarCodigoProducto(Sproducto *lista, int codigo){
    //Revisar si la lista esta vacia
    if(!(lista)){
        //Se retorna true porque la funcion esta hecha para que trabajarse con (!BuscarCodigoProducto)
        return true;
    }
    else{
        //En caso tal que no este vacia, se recorre la lista hasta encontrar un elemento con el mismo codigo introducido en la funcion.
        while(lista){
            if(lista->codigo==codigo){
                return false;
            }
            else{lista=lista->psig;}
        }
        //Si no encuentra ningun codigo se retorna true.
        return true;
    }
}

bool BuscarCodigoAsociado(Sasociado *lista, int codigo){
    //Misma logica que en BuscarCodigoProducto.
    if(!(lista)){
        return true;
    }
    else{
        while(lista){
            if(lista->codigo==codigo){
                return false;
            }
            else{lista=lista->pnext;}
        }
        return true;
    }
}

//La idea de esta funcion es cambiar todo a minusculas para evitar posibles problemas al buscar iteraciones con el mismo nombre pero pequeñas diferencias
//Para evitar posibles problemas, se usa const en el char de origen para evitar cualquier tipo de posible modificacion
void ToLower(const char origen[], char destino[]) {
    //Se usa size_t en lugar de int, debido a una mejor portabilidad del codigo y mejor seguridad
    size_t i = 0;
    //Se revisa que la palabra no este vacia
    while (origen[i] != '\0') {
        //Se crea una copia del char original
        char c = origen[i];
        //Se revisa si el la posicion actual del mismo es una mayuscual con la linea 85
        if (c >= 'A' && c <= 'Z') {
            //Se realiza esta operacion para pasarlo a minusculas, y se copia al char destino
            destino[i] = c - 'A' + 'a';
        } else {
            //En caso tal de que la palabra ya este en mayusculas simplemente se copia
            destino[i] = c;
        }
        //Se avanza de indice hasta recorrer la palabra entera
        ++i;
    }
    destino[i] = '\0';
}

//Funcion para revisar si contiene numeros un char, de igual manera se trabaja con const.
//Esta misma tiene una idea similar a ToLower
bool IsNumeric(const char origen []){
    size_t i = 0;
    //Se asegura que el char no este vacio
    while (origen[i] != '\0'){
        char c = origen[i];
        //Se copia un unico char "c" que se usa para revisar si es un digito con la funcion isdigit
        if(!isdigit(c)){
            //Si se cumple eso, se retorna falso
            return false;
        }
        //En cualquier otro caso, sigue revisando el arreglo.
        i++;
    }
    return true;
}

//Funcion para encontrar una subcadena dentro de otra
static int ContainsIgnoreCase(const char texto[], const char patron[]) {
    //Se crean 2 arreglos para poder luego con ToLower replicarlos en minusculas
    char textoMinusculas[200];
    char patronMinusculas[200];
    //Replicar los arreglos
    ToLower(texto, textoMinusculas);
    ToLower(patron, patronMinusculas);
    /*Usando strstr nos permite saber si existe la subcadena dentro del texto, 
    de esta forma se logra que si el usuario coloque "Juan" salgan todos los "Juan", sin importar si es "Juan Pablo" o "Juan Jose"
    */
    return strstr(textoMinusculas, patronMinusculas) != NULL;
}

//Guarda la informacion de los productos en un archivo plano para persistencia de datos
void GuardarProductos(Sproducto *lista){
	//Se abre el archivo en modo de escritura ("w")
	FILE *f = fopen("productos.txt", "w");
	if(!f) { 
		printf("Error al abrir el archivo .txt \n");
		return;
	}
	//Se recorre toda la lista de productos
	while(lista) {
		//Se escriben los datos separados por un caracter delimitador '|'
		fprintf(f, "%d|%s|%s|%s|%.2f\n",
			lista->codigo,
			lista->nombre,
			lista->marca,
			lista->descripcion,
			lista->precio);
		lista=lista->psig;
	}
	//Se cierra el archivo tras finalizar la escritura
	fclose(f);
}

//Carga la informacion de los productos desde el archivo de texto a la memoria
void CargarProductos(Sproducto **lista) {
	//Se abre el archivo en modo de lectura ("r")
	FILE *f = fopen("productos.txt", "r");
	if(!f) {return;}
	while(1) {
		//Se crea un nuevo nodo de producto
		Sproducto *nuevo = new Sproducto;
		//Se lee cada linea usando fscanf basandose en el delimitador '|'
		if (fscanf(f, "%d|%29[^|]|%29[^|]|%99[^|]|%f\n",
			&nuevo->codigo,
			nuevo->nombre,
			nuevo->marca,
			nuevo->descripcion,
			&nuevo->precio) == 5) {
			//Se inserta el nuevo nodo al inicio de la lista
			nuevo->psig = *lista;
			*lista = nuevo;
		}
		else{
			//Si no se logran leer los 5 elementos esperados, se libera el nodo y termina el bucle
			delete nuevo;
			break;
		}
	}
	fclose(f);
}

//Guarda la informacion de los asociados en un archivo de texto
void GuardarAsociados(Sasociado *lista){
	//Se abre en modo escritura
	FILE *f = fopen("asociados.txt", "w");
	if (!f) {
	printf("error al abrir asociados.txt\n");
	return;
	}
	//Se recorre la lista de asociados
	while (lista){
	//Se usa '|' como separador para no causar conflictos con espacios en nombres o direcciones
	fprintf(f,  "%d|%s|%s|%s\n",
		lista->codigo,
		lista->nombre,
		lista->direccion,
		lista->telefono);
	lista = lista->pnext;
	}
	fclose(f);
}

//Carga la informacion de asociados a la memoria desde el archivo
void CargarAsociados(Sasociado **lista) {
    //Apertura en modo lectura
    FILE *f = fopen("asociados.txt", "r");
    if (!f) return;
    Sasociado *nuevo;
    while (1) {
        nuevo = new Sasociado;
        //Se lee el archivo respetando el formato delimitado
        if (fscanf(f, "%d|%49[^|]|%99[^|]|%20[^\n]\n",
            &nuevo->codigo,
            nuevo->nombre,
            nuevo->direccion,
            &nuevo->telefono) == 4) {
            //Se inicializan las ventas del asociado en NULL y se vincula a la lista
            nuevo->pventas = NULL;
            nuevo->pnext = *lista;
            *lista = nuevo;
        } else {
            //Se borra el nodo si falla la lectura (fin de archivo)
            delete nuevo;
            break;
        }
    }
    fclose(f);
}

//============================ASOCIADOS============================
//Inserta un nuevo asociado creado previamente al inicio de la lista
void AgregarAsociados(Sasociado **lista, Sasociado** asociado){
        (*asociado)->pnext=*lista;
        *lista=*asociado;
        printf("Codigo agregado con exito!");
}

//Crea y solicita los datos al usuario de un nuevo asociado
Sasociado* NuevoAsociado(Sasociado *ListaAsociados){
    //Reserva de memoria para el nodo
    Sasociado *nuevo=new Sasociado;
    char nombre[100];
    char direccion[150];
    char telefono[21];
    int codigo;
	int valido=0;
    
    //Peticion del codigo con validacion de entrada numerica
    printf("Ingrese el codigo del Asociado: ");
    fflush(stdin);
    while(scanf("%d", &codigo) != 1){
        printf("El codigo es invalido, porfavor intente de nuevo. \n");
        while (getchar() != '\n');
        ("%d", &codigo);
    }
    //Validacion para evitar codigos duplicados
    while(!(BuscarCodigoAsociado(ListaAsociados, codigo))){
        printf("El codigo ya se encuentra en la lista, por favor introduzca uno distinto: ");
        fflush(stdin);scanf("%d", &codigo);fflush(stdin);
    }

    nuevo->codigo=codigo;

    //Peticion del nombre con validacion de longitud
    printf("Ingrese el nombre del Asociado (No puede ser mayor de 30 caracteres): \n");
    fflush(stdin);fflush(stdin);scanf(" %99[^\n]", nombre);fflush(stdin);
    while(strlen(nombre)>sizeof(nuevo->nombre)){
        printf("El nombre excede los 30 caracteres, por favor introduzca uno mas corto");
        fflush(stdin);fflush(stdin);scanf(" %99[^\n]", nombre);fflush(stdin);
    }
    strcpy(nuevo->nombre, nombre);

    //Peticion de la direccion con validacion de longitud
    printf("Ingrese la direcccion del Asociado (No puede ser mayor de 100 caracteres): \n");
    fflush(stdin);fflush(stdin);scanf(" %149[^\n]", direccion);fflush(stdin);
    while(strlen(direccion)>sizeof(nuevo->direccion)){
        printf("La direccion excede los 100 caracteres, por favor introduzca uno mas corto");
        fflush(stdin);fflush(stdin);scanf(" %149[^\n]", direccion);fflush(stdin);
    }
    strcpy(nuevo->direccion, direccion);

    //Peticion del telefono con validacion de longitud y que solo contenga numeros
    printf("Ingrese el numero de telefono del Asociado: ");
    fflush(stdin);scanf(" %20[^\n]", telefono);fflush(stdin);
    while(strlen(telefono)>sizeof(nuevo->telefono)){
        printf("Se excede el numero de telefono.");
        fflush(stdin);scanf(" %20[^\n]", telefono);fflush(stdin);
    }
    while(!IsNumeric(telefono)){
        printf("El numero de telefono no es valido, por favor revise si hay una letra. ");
        fflush(stdin);scanf(" %20[^\n]", telefono);fflush(stdin);
    }
    strcpy(nuevo->telefono, telefono);

    //Se inicializan punteros del nodo a NULL
    nuevo->pnext=NULL;
    nuevo->pventas=NULL;
    return nuevo;
}

//Muestra en consola un asociado especifico
void MostrarAsociado(Sasociado *p){
    //Si el puntero es nulo, no hace nada
    if(!p) return;
    printf("Nombre: %s | Direccion: %s | Numero de telefono: %s | Codigo del producto: %d\n", 
    p->nombre, p->direccion, p->telefono, p->codigo);
}

//Busca e imprime un asociado coincidente con el codigo indicado
void ConsultarCodigoAsociado(Sasociado *lista, int n){
    //Asegurarse de que exista la lista
    if(!(lista)){
        printf("No hay asociados en la lista\n");
    }
    else{
        Sasociado *aux = lista;
        //Recorremos hasta llegar a nulo o encontrar el codigo
        while(aux && aux->codigo != n){
            aux = aux->pnext;
        }
        //Si salimos del ciclo y aux no es nulo, encontramos la coincidencia
        if(aux && aux->codigo == n){
            MostrarAsociado(aux);
        }
        else{
            printf("El codigo no esta en la lista\n");
        }
    }
}

//Busca asociados empleando un fragmento del nombre (no case-sensitive)
void BuscarPorNombreAsociado(Sasociado *lista, const char nombre[]){
    //Revisar lista vacia
    if(!lista){
        printf("No hay productos en la lista\n");
        return;
    }

    bool encontrado = false;
    //Recorre toda la lista comprobando si el texto ingresado esta en el nombre
    while(lista){
        if(ContainsIgnoreCase(lista->nombre, nombre)){
            MostrarAsociado(lista);
            encontrado = true;
        }
        lista = lista->pnext;
    }

    //Si al acabar el ciclo no consiguio nada, notifica
    if(!encontrado){
        printf("No se encontro ningun producto con ese nombre\n");
    }
}

//Retorna el puntero al asociado que coincide con el codigo, de lo contrario retorna NULL
Sasociado* ExisteAsociado(Sasociado *lista, int n){
    Sasociado* ax = lista;
    while(ax && ax->codigo != n){
        ax = ax->pnext;
    }
    return ax;
}

//Permite al usuario cambiar los datos de un asociado
void ModificarCodigoAsociado(Sasociado *lista){
    int CambioNum;
    int menu;
    char Cambio[150];

    //Comprobacion base
    if(lista==NULL){
        printf("La lista esta vacia. ");
        return;
    }
    else{
    //Despliega menu de opciones
    printf("Que deseas modificar: \n");
    printf("1- Nombre. \n");
    printf("2- Direccion. \n");
    printf("3- Telefono. \n");
    printf("4- Codigo. \n");
    fflush(stdin);scanf("%d", &menu);fflush(stdin);
    //Dependiendo de la eleccion pedira datos distintos y los guardara usando strcpy o asignacion
    switch(menu){
        case 1:
            printf("Ingrese el nombre: \n");
            scanf(" %149[^\n]", Cambio);fflush(stdin);
            //Se asegura que no pase la memoria destinada en la estructura
            while (strlen(Cambio)>=sizeof(lista->nombre)){
                printf("El nombre excede los 30 caracteres, por favor introduzca algo mas corto. \n");
                fflush(stdin);scanf(" %99[^\n]", Cambio);
                }
            strcpy(lista->nombre, Cambio);
            fflush(stdin);
            printf("El codigo fue cambiado con exito!");
            printf("\n");
            system("pause");
            //Llama a guardar los cambios en el archivo .txt
            GuardarAsociados(lista);
            break;
        case 2:
            printf("Ingrese la direccion: \n");
            scanf(" %149[^\n]", Cambio);fflush(stdin);
            while (strlen(Cambio)>=sizeof(lista->direccion)){
                printf("La direccion se excede los 100 caracteres, por favor introduzca algo mas corto. \n");
                fflush(stdin);scanf(" %99[^\n]", Cambio);
            }
            strcpy(lista->direccion, Cambio);
            fflush(stdin);
            printf("El codigo fue cambiado con exito!");
            printf("\n");
            system("pause");
            GuardarAsociados(lista);
            break;
        case 3:
            printf("Ingrese el numero de telefono del Asociado: ");
            fflush(stdin);scanf(" %20[^\n]", Cambio);fflush(stdin);
            while(strlen(Cambio)>sizeof(lista->telefono)){
                printf("Se excede el numero de telefono.");
                fflush(stdin);scanf(" %20[^\n]", Cambio);fflush(stdin);
            }
            //Se vuelve a validar que el string contenga puros digitos
            while(!IsNumeric(Cambio)){
                printf("El numero de telefono no es valido, por favor revise si hay una letra. ");
                fflush(stdin);scanf(" %20[^\n]", Cambio);fflush(stdin);
            }

            strcpy(lista->telefono, Cambio);
            printf("Codigo cambiado con exito! \n");
            system("pause");
            GuardarAsociados(lista);
            break;
        case 4:
            printf("Ingrese el codigo del Asociado: ");
            while(scanf("%d", &CambioNum) != 1){
                printf("El codigo es invalido, porfavor intente de nuevo. \n");
                while (getchar() != '\n');
                ("%d", &CambioNum);
            }
            //Comprueba si el codigo nuevo no le pertenece ya a alguien
            while(!(BuscarCodigoAsociado(lista, CambioNum))){
                printf("El codigo ya se encuentra en la lista, por favor introduzca uno distinto: ");
                fflush(stdin);scanf("%d", &CambioNum);fflush(stdin);
            }
            lista->codigo=CambioNum;
            printf("\n");
            printf("Codigo cambiado con exito!");
            printf("\n");
            system("pause");
            GuardarAsociados(lista);
            break;
        default:
            printf("Por favor introduzca una opcion valida. \n");
            fflush(stdin);scanf("%d", &menu);fflush(stdin);
        }
    }
}

//Elimina un asociado en especifico reconectando los punteros
void EliminarPorCodigoAsociado(Sasociado **a, int n){
    Sasociado *aux;
    Sasociado *t;
    //Caso en el que el asociado a eliminar es el primero de la lista
    if((*a)->codigo==n){
        aux=*a;
        *a=(*a)->pnext;
        delete aux;
    }
    //Si es un nodo posterior se busca hasta que el pnext contenga el codigo
    else{
        aux=*a;
        while(aux->pnext && aux->pnext->codigo!=n){
            aux=aux->pnext;
        }
            //Si encontro a alguien, puentea el nodo a eliminar y libera la memoria
            if(aux->pnext!=NULL){
                t=aux->pnext;
                aux->pnext=t->pnext;
                delete t;
                printf("El codigo fue eliminado con exito!");
            }
            else{
                //Si termina el ciclo sin resultado
                printf("El codigo no esta en la lista");
            }
    }
}

//Recorre la lista entera imprimiendo todos los asociados
void MostrarAsociados(Sasociado *lista){
    while(lista){
        printf("Nombre: %s | Direccion: %s | Numero de telefono: %s | Codigo del producto: %d\n", 
        lista->nombre, lista->direccion, lista->telefono, lista->codigo);
        lista=lista->pnext;
    }
}

//============================PRODUCTO============================
//Peticion de datos de un producto a agregar
Sproducto* NuevoProducto(Sproducto *ListaProductos){ 
    //Reserva de memoria
    Sproducto *nuevo=new Sproducto;
    char temp[100];
    char marca[100];
    char descripcion[150];
    int codigo;
	int entrada_valida;

    //Validaciones equivalentes a las usadas en asociados (numerico y unicidad)
    printf("Ingrese el codigo del producto: ");
    while(scanf("%d", &codigo) != 1){
        printf("El codigo es invalido, porfavor intente de nuevo. \n");
        while (getchar() != '\n');
        ("%d", &codigo);
    }
    while(!(BuscarCodigoProducto(ListaProductos, codigo))){
        printf("El codigo ya se encuentra en la lista, por favor introduzca uno distinto: ");
        fflush(stdin);scanf("%d", &codigo);fflush(stdin);
    }

    printf("Ingrese el nombre (No puede ser mayor de 30 caracteres): \n");
    fflush(stdin);fflush(stdin);scanf(" %99[^\n]", temp);  fflush(stdin); 
    while (strlen(temp)>=sizeof(nuevo->nombre)){
        printf("El nombre excede los 30 caracteres, por favor introduzca algo mas corto. \n");
        fflush(stdin);scanf(" %99[^\n]", temp);
    }
    strcpy(nuevo->nombre, temp);

    printf("Ingrese la marca (No puede ser mayor de 30 caracteres): \n");
    fflush(stdin);scanf(" %99[^\n]", marca);fflush(stdin);
    while(strlen(marca)>=sizeof(nuevo->marca)){
        printf("Muy largo. \n");
        fflush(stdin);scanf(" %99[^\n]", marca);fflush(stdin);
    }
    strcpy(nuevo->marca, marca);

    printf("Ingrese el descripcion (No puede ser mayor a 100 caracteres): \n");
    fflush(stdin);scanf(" %99[^\n]", descripcion);fflush(stdin);
    while(strlen(descripcion)>=sizeof(nuevo->descripcion)){
        printf("Muy largo. \n");
        fflush(stdin);scanf(" %99[^\n]", descripcion);fflush(stdin);
    }
    strcpy(nuevo->descripcion, descripcion);

    //Se pide y valida que el precio sea mayor que cero
    printf("Ingrese el precio del producto: ");
    fflush(stdin);scanf("%f", &nuevo->precio);fflush(stdin);
    while(nuevo->precio<=0){
        printf("Por favor introduzca un numero mayor a 0: \n");
        fflush(stdin);scanf("%f", &nuevo->precio);fflush(stdin);
    }

    //Cierre de la configuracion del nodo
    nuevo->codigo=codigo;
    nuevo->psig=NULL;
    return nuevo;
    
}

//Agrega el nuevo producto por el inicio de la lista
void AgregarProductos(Sproducto **lista, Sproducto** producto){
        (*producto)->psig=*lista;
        *lista=*producto;
        printf("Codigo agregado con exito!");
}

//Imprime en consola todos los productos
void MostrarProductos(Sproducto *lista){
    while(lista){
        printf("Nombre: %s | Nombre de la marca: %s | Descripcion del producto: %s | Codigo del producto: %d\n", 
        lista->nombre, lista->marca, lista->descripcion, lista->codigo);
        lista=lista->psig;
    }
}

//Muestra unicamente los detalles del producto recibido por parametro
void MostrarProducto(Sproducto *p){
    if(!p) return;
        printf("Nombre: %s | Nombre de la marca: %s | Descripcion del producto: %s | Codigo del producto: %d\n", 
        p->nombre, p->marca, p->descripcion, p->codigo);
}

//Recorre y muestra si existe un producto que concuerde con el codigo proporcionado
void ConsultarCodigo(Sproducto *lista, int n){
    if(!(lista)){
        printf("No hay productos en la lista\n");
    }

    else{
        Sproducto *aux = lista;
        while(aux && aux->codigo != n){
            aux = aux->psig;
        }

        if(aux && aux->codigo == n){
            MostrarProducto(aux);
        }
        else{
            printf("El codigo no esta en la lista\n");
        }
    }
}

//Busca un producto que cuente con parte o el todo del texto introducido
void BuscarPorNombre(Sproducto *lista, const char nombre[]){
    if(!lista){
        printf("No hay productos en la lista\n");
        return;
    }

    bool encontrado = false;
    while(lista){
        //El metodo auxiliar comprueba en minusculas la existencia de la subcadena
        if(ContainsIgnoreCase(lista->nombre, nombre)){
            MostrarProducto(lista);
            encontrado = true;
        }
        lista = lista->psig;
    }

    if(!encontrado){
        printf("No se encontro ningun producto con ese nombre\n");
    }
}

//Retorna el nodo del producto correspondiente si este es hallado
Sproducto* ExisteProducto(Sproducto *lista, int n){
    Sproducto* ax = lista;
    while(ax && ax->codigo != n){
        ax = ax->psig;
    }
    return ax;
}

//Permite la modificacion interactiva de los datos del producto
void ModificarCodigo(Sproducto *lista){
    int mod;
    int menu;
    char Cambio[150];

    //Si esta vacia simplemente lo rechaza
    if(lista==NULL){
        printf("La lista esta vacia. ");
        return;
    }
    else{
    printf("Que deseas modificar: \n");
    printf("1- Nombre. \n");
    printf("2- Marca. \n");
    printf("3- Descripcion. \n");
    printf("4- Precio. \n");
    printf("5- Codigo. \n");
    fflush(stdin);scanf("%d", &menu);fflush(stdin); 
    switch(menu){
        case 1:
            //Modificacion del nombre 
            printf("Ingrese el nombre: \n");
            scanf(" %149[^\n]", Cambio);fflush(stdin);
            while (strlen(Cambio)>=sizeof(lista->nombre)){
                printf("El nombre excede los 30 caracteres, por favor introduzca algo mas corto. \n");
                fflush(stdin);scanf(" %99[^\n]", Cambio);
                }
            strcpy(lista->nombre, Cambio);
            fflush(stdin);
            printf("El codigo fue cambiado con exito!");
            printf("\n");
            system("pause");
            GuardarProductos(lista);
            break;
        case 2:
            //Modificacion de la marca
            printf("Ingrese la direccion: \n");
            scanf(" %149[^\n]", Cambio);fflush(stdin);
            while (strlen(Cambio)>=sizeof(lista->marca)){
                printf("La marca se excede los 30 caracteres, por favor introduzca algo mas corto. \n");
                fflush(stdin);scanf(" %99[^\n]", Cambio);
            }
            strcpy(lista->marca, Cambio);
            fflush(stdin);
            printf("El codigo fue cambiado con exito!");
            printf("\n");
            system("pause");
            GuardarProductos(lista);
            break;
        case 3:
            //Modificacion de la descripcion
            printf("Ingrese la descripcion: \n");
            scanf(" %149[^\n]", Cambio);fflush(stdin);
            while (strlen(Cambio)>=sizeof(lista->descripcion)){
                printf("La descripcion se excede los 100 caracteres, por favor introduzca algo mas corto. \n");
                fflush(stdin);scanf(" %99[^\n]", Cambio);
                }
            strcpy(lista->descripcion, Cambio);
            fflush(stdin);
            printf("El codigo fue cambiado con exito!");
            printf("\n");
            system("pause");
            GuardarProductos(lista);
            break;
        case 4: {
            //Modificacion del precio, con validacion contra precios negativos o nulos
            float modprecio;
            printf("Ingrese el precio: \n");
            fflush(stdin);scanf("%f", &modprecio);fflush(stdin);
            while(modprecio<=0){
                printf("Por favor introduzca un numero mayor a 0: \n");
                fflush(stdin);scanf("%f", &modprecio);fflush(stdin);
            }
            lista->precio=modprecio;
            printf("El codigo fue cambiado con exito!");
            printf("\n");
            system("pause");
            GuardarProductos(lista);
            break;
        }
        case 5:
            //Reemplazo del codigo con verificacion de colisiones
            printf("Ingrese el codigo del Asociado: ");
            scanf("%d", &mod);fflush(stdin);
            while(!(BuscarCodigoProducto(lista, mod))){
                printf("El codigo ya se encuentra en la lista, por favor introduzca uno distinto: ");
                fflush(stdin);scanf("%d", &mod);fflush(stdin);
            }
            lista->codigo=mod;
            printf("\n");
            system("pause");
            GuardarProductos(lista);
            break;
        default:
            printf("Por favor introduzca una opcion valida. \n");
            fflush(stdin);scanf("%d", &menu);fflush(stdin);
            }
        }
    }

//Reconecta los nodos de la sublista evitando que se pierda la estructura al eliminar producto
void EliminarPorCodigo(Sproducto **a, int n){
    Sproducto *aux;
    Sproducto *t;
    //Verifica la cabeza de la lista
    if((*a)->codigo==n){
        aux=*a;
        *a=(*a)->psig;
        delete aux;
    }
    else{
        //Sino es la cabeza, recorre revisando en 'psig' de cada iteracion
        aux=*a;
        while(aux->psig && aux->psig->codigo!=n){
            aux=aux->psig;
        }
            //Realiza el salto de nodo para mantener integridad y libera
            if(aux->psig!=NULL){
                t=aux->psig;
                aux->psig=t->psig;
                delete t;
                printf("El codigo fue eliminado con exito!");
            }
            else{
                printf("El codigo no esta en la lista");
            }
    }
}


//============================VENTAS============================
//Analiza matematicamente la fecha para desglosarla y corroborar coherencia basica
bool EsFechaValida(int fecha){
    //Descomposicion extrayendo usando modulos y divisiones
    int anio = fecha/10000;
    int mes = (fecha/100)%100;
    int dia = fecha%100;
    //Validacion de bordes
    if(anio<1900 || anio>2100){
        return false;
    }
    if(mes<1 || mes>12){
        return false;
    }
    if(dia<1 || dia>31){
        return false;
    }
    return true;
}

//Rutina que construye el objeto de venta interactuando con los catalogos de productos y asociando IDs
Sventa* NuevaVenta(Sproducto *ListaProductos, int *contador){
    //Preparacion
    Sventa *nueva=new Sventa;
    Sproducto *prod;
    int codigo_producto;
    int cantidad;
    int fecha;

    //Solicita y se asegura que el producto vendido en efecto exista en la DB
    printf("Ingrese el codigo del producto vendido: ");
    fflush(stdin);scanf("%d", &codigo_producto);fflush(stdin);
    prod=ExisteProducto(ListaProductos, codigo_producto);
    while(prod==NULL){
        printf("El producto no existe, por favor introduzca un codigo valido: ");
        fflush(stdin);scanf("%d", &codigo_producto);fflush(stdin);
        prod=ExisteProducto(ListaProductos, codigo_producto);
    }

    //Ingreso de cantidad
    printf("Ingrese la cantidad vendida: ");
    fflush(stdin);scanf("%d", &cantidad);fflush(stdin);
    while(cantidad<=0){
        printf("La cantidad debe ser mayor a 0, por favor introduzca de nuevo: ");
        fflush(stdin);scanf("%d", &cantidad);fflush(stdin);
    }

    //Ingreso de la fecha y verificacion logica con la funcion EsFechaValida
    printf("Ingrese la fecha de la venta (formato AAAAMMDD): ");
    fflush(stdin);scanf("%d", &fecha);fflush(stdin);
    while(!EsFechaValida(fecha)){
        printf("La fecha no es valida, por favor introduzca de nuevo (formato AAAAMMDD): ");
        fflush(stdin);scanf("%d", &fecha);fflush(stdin);
    }

    //Generacion automatica de la venta
    nueva->num_operacion=*contador;
    *contador=*contador+1;
    nueva->codigo_producto=codigo_producto;
    nueva->cantidad=cantidad;
    nueva->precio_unidad=prod->precio;
    nueva->monto=prod->precio*cantidad; //Calculo del monto final
    nueva->fecha=fecha;
    nueva->prventas=NULL;

    return nueva;
}

//Asocia de forma cruzada la venta en el puntero del asociado
void AgregarVenta(Sasociado *vendedor, Sventa *venta){
    venta->prventas=vendedor->pventas;
    vendedor->pventas=venta;
    printf("Venta agregada con exito!");
}

//Respaldo en TXT de todas las sublistas de ventas de todos los asociados
void GuardarVentas(Sasociado *lista){
	FILE *f = fopen("ventas.txt", "w");
	Sventa *v;
	if(!f) { 
		printf("Error al abrir el archivo ventas.txt \n");
		return;
	}
	//Itera primero sobre todos los asociados en el sistema
	while(lista) {
		v=lista->pventas;
		//Por cada asociado iterara sus propias ventas guardando tambien el ID de quien vendio
		while(v){
			fprintf(f, "%d|%d|%d|%d|%.2f|%.2f|%d\n",
				lista->codigo,
				v->num_operacion,
				v->codigo_producto,
				v->cantidad,
				v->precio_unidad,
				v->monto,
				v->fecha);
			v=v->prventas;
		}
		lista=lista->pnext;
	}
	fclose(f);
}

//Lee del registro conectando las ventas devuelta con sus vendedores designados
void CargarVentas(Sasociado *listaAsociados, int *contador) {
	FILE *f = fopen("ventas.txt", "r");
	int codigo_asociado;
	Sasociado *vendedor;
	Sventa *nueva;
	if(!f) {return;}
	//Procesamiento por registro
	while(1) {
		nueva = new Sventa;
		//Recupera todos los campos formateados desde el string guardado
		if (fscanf(f, "%d|%d|%d|%d|%f|%f|%d\n",
			&codigo_asociado,
			&nueva->num_operacion,
			&nueva->codigo_producto,
			&nueva->cantidad,
			&nueva->precio_unidad,
			&nueva->monto,
			&nueva->fecha) == 7) {
			
			//Conecta a memoria el puntero de vendedor
			vendedor=ExisteAsociado(listaAsociados, codigo_asociado);
			if(vendedor!=NULL){
				//Agrega al listado local
				nueva->prventas=vendedor->pventas;
				vendedor->pventas=nueva;
				//Actualiza el contador global de operaciones por si luego se cargan mas para evitar chocar nums
				if(nueva->num_operacion>=*contador){
					*contador=nueva->num_operacion+1;
				}
			}
			else{
				//Si el vendedor fue eliminado del historial borra la venta en memoria
				delete nueva;
			}
		}
		else{
			delete nueva;
			break;
		}
	}
	fclose(f);
}

//Convierte del int AAAAMMDD a impresion visual DD/MM/AAAA
void MostrarFecha(int fecha){
    int anio = fecha/10000;
    int mes = (fecha/100)%100;
    int dia = fecha%100;
    printf("%02d/%02d/%d", dia, mes, anio);
}

//Desglose de los detalles de operacion de una Sventa en pantalla
void MostrarVentaDetalle(Sventa *v, Sasociado *vendedor, Sproducto *ListaProductos){
    //Rescata el producto real
    Sproducto *prod = ExisteProducto(ListaProductos, v->codigo_producto);
    printf("Numero de operacion: %d\n", v->num_operacion);
    printf("Vendedor: %s\n", vendedor->nombre);
    //Verifica que el producto de la venta no se haya borrado en la base de datos
    if(prod!=NULL){
        printf("Producto: %s | Marca: %s\n", prod->nombre, prod->marca);
    }
    else{
        printf("Producto: (no encontrado, codigo %d)\n", v->codigo_producto);
    }
    printf("Cantidad: %d\n", v->cantidad);
    printf("Precio unidad: %.2f\n", v->precio_unidad);
    printf("Monto total: %.2f\n", v->monto);
    printf("Fecha: ");
    MostrarFecha(v->fecha);
    printf("\n");
}

//Realiza la consulta general cruzando asociados
void ConsultarVentaPorOperacion(Sasociado *listaAsociados, Sproducto *ListaProductos, int numOp){
    Sasociado *aux=listaAsociados;
    Sventa *v;
    bool encontrado=false;

    //Doble ciclo while, el exterior recorre asociados el interior las ventas
    while(aux && !encontrado){
        v=aux->pventas;
        while(v && !encontrado){
            if(v->num_operacion==numOp){
                MostrarVentaDetalle(v, aux, ListaProductos);
                encontrado=true;
            }
            v=v->prventas;
        }
        aux=aux->pnext;
    }

    if(!encontrado){
        printf("No se encontro ninguna venta con ese numero de operacion\n");
    }
}

//Borra de manera segura una venta registrada especificando el numero de operacion global
void EliminarVentaPorOperacion(Sasociado *listaAsociados, int numOp){
    Sasociado *aux=listaAsociados;
    Sventa *v, *t;
    bool encontrado=false;

    while(aux && !encontrado){
        //Maneja eliminacion en cabeza
        if(aux->pventas!=NULL && aux->pventas->num_operacion==numOp){
            t=aux->pventas;
            aux->pventas=t->prventas;
            delete t;
            encontrado=true;
            printf("La venta fue eliminada con exito!");
        }
        //Maneja eliminacion en nodos hijos
        else if(aux->pventas!=NULL){
            v=aux->pventas;
            while(v->prventas && v->prventas->num_operacion!=numOp){
                v=v->prventas;
            }
            if(v->prventas!=NULL){
                t=v->prventas;
                v->prventas=t->prventas;
                delete t;
                encontrado=true;
                printf("La venta fue eliminada con exito!");
            }
        }
        aux=aux->pnext;
    }

    if(!encontrado){
        printf("No se encontro ninguna venta con ese numero de operacion");
    }
}

//Filtro estadistico de ventas pertenecientes a un unico vendedor dadas 2 fechas (range)
void MostrarVentasVendedorEntreFechas(Sasociado *vendedor, Sproducto *ListaProductos, int fechaIni, int fechaFin){
    //Arreglo temporal para ordenar estaticamente
    Sventa *arreglo[MAX_VENTAS];
    int total=0;
    Sventa *v=vendedor->pventas;
    Sproducto *prod;
    int i, j;
    Sventa *temp;

    //Almacena los matchs del rango de fechas
    while(v && total<MAX_VENTAS){
        if(v->fecha>=fechaIni && v->fecha<=fechaFin){
            arreglo[total]=v;
            total++;
        }
        v=v->prventas;
    }

    if(total==0){
        printf("No hay ventas de ese vendedor entre las fechas indicadas\n");
        return;
    }

    //Algoritmo basico de Bubble Sort por numero de operacion
    for(i=0;i<total-1;i++){
        for(j=0;j<total-1-i;j++){
            if(arreglo[j]->num_operacion>arreglo[j+1]->num_operacion){
                temp=arreglo[j];
                arreglo[j]=arreglo[j+1];
                arreglo[j+1]=temp;
            }
        }
    }

    //Impresion del reporte pre-calculado
    printf("Ventas del vendedor %s:\n", vendedor->nombre);
    for(i=0;i<total;i++){
        prod=ExisteProducto(ListaProductos, arreglo[i]->codigo_producto);
        printf("Operacion: %d | Producto: %s | Cantidad: %d | Precio unidad: %.2f | Monto total: %.2f | Fecha: ",
            arreglo[i]->num_operacion,
            (prod!=NULL)?prod->nombre:"(desconocido)",
            arreglo[i]->cantidad,
            arreglo[i]->precio_unidad,
            arreglo[i]->monto);
        MostrarFecha(arreglo[i]->fecha);
        printf("\n");
    }
}

//============================REPORTES============================
//Genera reportes custom enfocados en variables especificas de ventas
struct SReporteProducto{
    char marca[30];
    int cantidad;
    float monto;
};

//Dado un producto en particular agrupa estadisticamente cuanto vendio y sus recaudos
void Reporte1_Producto(Sasociado *listaAsociados, Sproducto *ListaProductos){
    int codigo_producto;
    Sproducto *prod;
    SReporteProducto arreglo[MAX_VENTAS];
    int total=0;
    Sasociado *aux;
    Sventa *v;
    int i, j;
    SReporteProducto temp;

    //Checkeos contra la database de productos
    printf("Ingrese el codigo del producto que desea consultar: ");
    fflush(stdin);scanf("%d", &codigo_producto);fflush(stdin);
    prod=ExisteProducto(ListaProductos, codigo_producto);
    while(prod==NULL){
        printf("El producto no existe, por favor introduzca un codigo valido: ");
        fflush(stdin);scanf("%d", &codigo_producto);fflush(stdin);
        prod=ExisteProducto(ListaProductos, codigo_producto);
    }

    //Guarda transacciones que concuerden con el producto buscado
    aux=listaAsociados;
    while(aux){
        v=aux->pventas;
        while(v){
            if(v->codigo_producto==codigo_producto && total<MAX_VENTAS){
                strcpy(arreglo[total].marca, prod->marca);
                arreglo[total].cantidad=v->cantidad;
                arreglo[total].monto=v->monto;
                total++;
            }
            v=v->prventas;
        }
        aux=aux->pnext;
    }

    if(total==0){
        printf("No hay ventas registradas para ese producto\n");
        return;
    }

    //Ordenado descendente por cantidad de productos a traves de Bubble Sort
    for(i=0;i<total-1;i++){
        for(j=0;j<total-1-i;j++){
            if(arreglo[j].cantidad<arreglo[j+1].cantidad){
                temp=arreglo[j];
                arreglo[j]=arreglo[j+1];
                arreglo[j+1]=temp;
            }
        }
    }

    //Imprime el resumen
    printf("Ventas del producto %s:\n", prod->nombre);
    for(i=0;i<total;i++){
        printf("Marca: %s | Cantidad de unidades: %d | Monto total: %.2f\n",
            arreglo[i].marca, arreglo[i].cantidad, arreglo[i].monto);
    }
}

//Estadisticas especificas de todos los productos agrupados por marca
struct SReporteMarca{
    int codigo_producto;
    char nombreProducto[30];
    int cantidad;
    float monto;
};

void Reporte2_Marca(Sasociado *listaAsociados, Sproducto *ListaProductos){
    char marca[100];
    SReporteMarca arreglo[MAX_VENTAS];
    int total=0;
    Sasociado *aux;
    Sventa *v;
    Sproducto *prod;
    int i, j, k;
    bool encontrado_prod;
    SReporteMarca temp;

    //Consulta de String
    printf("Ingrese la marca que desea consultar: \n");
    fflush(stdin);scanf(" %99[^\n]", marca);fflush(stdin);

    //Iteracion cruzada acumulando los totales de cada subproducto
    aux=listaAsociados;
    while(aux){
        v=aux->pventas;
        while(v){
            prod=ExisteProducto(ListaProductos, v->codigo_producto);
            //Uso de ContainsIgnoreCase para facilitar la busqueda
            if(prod!=NULL && ContainsIgnoreCase(prod->marca, marca)){
                encontrado_prod=false;
                for(k=0;k<total;k++){
                    //Si ya existe en el array temporal, le sumamos el total
                    if(arreglo[k].codigo_producto==prod->codigo){
                        arreglo[k].cantidad=arreglo[k].cantidad+v->cantidad;
                        arreglo[k].monto=arreglo[k].monto+v->monto;
                        encontrado_prod=true;
                        break;
                    }
                }
                //Si no fue encontrado en el array temporal previo, creamos el slot nuevo
                if(!encontrado_prod && total<MAX_VENTAS){
                    arreglo[total].codigo_producto=prod->codigo;
                    strcpy(arreglo[total].nombreProducto, prod->nombre);
                    arreglo[total].cantidad=v->cantidad;
                    arreglo[total].monto=v->monto;
                    total++;
                }
            }
            v=v->prventas;
        }
        aux=aux->pnext;
    }

    if(total==0){
        printf("No hay ventas registradas para esa marca\n");
        return;
    }

    //Ordenado Bubble Sort descendente por monto total facturado
    for(i=0;i<total-1;i++){
        for(j=0;j<total-1-i;j++){
            if(arreglo[j].monto<arreglo[j+1].monto){
                temp=arreglo[j];
                arreglo[j]=arreglo[j+1];
                arreglo[j+1]=temp;
            }
        }
    }

    printf("Ventas de la marca %s:\n", marca);
    for(i=0;i<total;i++){
        printf("Producto: %s | Cantidad de unidades: %d | Monto total: %.2f\n",
            arreglo[i].nombreProducto, arreglo[i].cantidad, arreglo[i].monto);
    }
}

//Estadisticas de ventas directas de un Asociado (vendedor)
struct SReporteVendedor{
    char nombreProducto[30];
    char marca[30];
    int cantidad;
    float monto;
};

void Reporte3_Vendedor(Sasociado *listaAsociados, Sproducto *ListaProductos){
    int codigo_vendedor;
    Sasociado *vendedor;
    SReporteVendedor arreglo[MAX_VENTAS];
    int total=0;
    Sventa *v;
    Sproducto *prod;
    int i, j;
    SReporteVendedor temp;

    //Consulta el asociado deseado
    printf("Ingrese el codigo del vendedor que desea consultar: ");
    fflush(stdin);scanf("%d", &codigo_vendedor);fflush(stdin);
    vendedor=ExisteAsociado(listaAsociados, codigo_vendedor);
    while(vendedor==NULL){
        printf("El vendedor no existe, por favor introduzca un codigo valido: ");
        fflush(stdin);scanf("%d", &codigo_vendedor);fflush(stdin);
        vendedor=ExisteAsociado(listaAsociados, codigo_vendedor);
    }

    //Recopila toda la venta conectada al nodo del vendedor iterando
    v=vendedor->pventas;
    while(v && total<MAX_VENTAS){
        prod=ExisteProducto(ListaProductos, v->codigo_producto);
        if(prod!=NULL){
            strcpy(arreglo[total].nombreProducto, prod->nombre);
            strcpy(arreglo[total].marca, prod->marca);
        }
        else{
            strcpy(arreglo[total].nombreProducto, "(desconocido)");
            strcpy(arreglo[total].marca, "(desconocido)");
        }
        arreglo[total].cantidad=v->cantidad;
        arreglo[total].monto=v->monto;
        total++;
        v=v->prventas;
    }

    if(total==0){
        printf("No hay ventas registradas para ese vendedor\n");
        return;
    }

    //Ordenado por nombre de producto y luego sub-marca (Usando strcmp)
    for(i=0;i<total-1;i++){
        for(j=0;j<total-1-i;j++){
            //Compara alfabeticamente para ordenar
            int cmp=strcmp(arreglo[j].nombreProducto, arreglo[j+1].nombreProducto);
            if(cmp>0 || (cmp==0 && strcmp(arreglo[j].marca, arreglo[j+1].marca)>0)){
                temp=arreglo[j];
                arreglo[j]=arreglo[j+1];
                arreglo[j+1]=temp;
            }
        }
    }

    printf("Ventas del vendedor %s:\n", vendedor->nombre);
    for(i=0;i<total;i++){
        printf("Producto: %s | Marca: %s | Cantidad de unidades: %d | Monto total: %.2f\n",
            arreglo[i].nombreProducto, arreglo[i].marca, arreglo[i].cantidad, arreglo[i].monto);
    }
}

//Agrupacion de todas las ventas generales que esten dentro de un limite
struct SReporteFecha{
    char nombreVendedor[50];
    char nombreProducto[30];
    char marca[30];
    int cantidad;
    float monto;
    int fecha;
};

void Reporte4_Fechas(Sasociado *listaAsociados, Sproducto *ListaProductos){
    int fechaIni, fechaFin;
    SReporteFecha arreglo[MAX_VENTAS];
    int total=0;
    Sasociado *aux;
    Sventa *v;
    Sproducto *prod;
    int i, j;
    SReporteFecha temp;

    //Obtiene del usuario el umbral inicial y final y valida formato AAAAMMDD
    printf("Ingrese la fecha inicial (formato AAAAMMDD): ");
    fflush(stdin);scanf("%d", &fechaIni);fflush(stdin);
    while(!EsFechaValida(fechaIni)){
        printf("La fecha no es valida, por favor introduzca de nuevo: ");
        fflush(stdin);scanf("%d", &fechaIni);fflush(stdin);
    }

    printf("Ingrese la fecha final (formato AAAAMMDD): ");
    fflush(stdin);scanf("%d", &fechaFin);fflush(stdin);
    while(!EsFechaValida(fechaFin)){
        printf("La fecha no es valida, por favor introduzca de nuevo: ");
        fflush(stdin);scanf("%d", &fechaFin);fflush(stdin);
    }

    //Navega extrayendo en el rango todos los historiales a un array de SReporteFecha
    aux=listaAsociados;
    while(aux){
        v=aux->pventas;
        while(v && total<MAX_VENTAS){
            if(v->fecha>=fechaIni && v->fecha<=fechaFin){
                prod=ExisteProducto(ListaProductos, v->codigo_producto);
                strcpy(arreglo[total].nombreVendedor, aux->nombre);
                if(prod!=NULL){
                    strcpy(arreglo[total].nombreProducto, prod->nombre);
                    strcpy(arreglo[total].marca, prod->marca);
                }
                else{
                    strcpy(arreglo[total].nombreProducto, "(desconocido)");
                    strcpy(arreglo[total].marca, "(desconocido)");
                }
                arreglo[total].cantidad=v->cantidad;
                arreglo[total].monto=v->monto;
                arreglo[total].fecha=v->fecha;
                total++;
            }
            v=v->prventas;
        }
        aux=aux->pnext;
    }

    if(total==0){
        printf("No hay ventas registradas entre esas fechas\n");
        return;
    }

    //Ordenado por fecha (mas nuevo a mas antigua, menor int es mas viejo)
    for(i=0;i<total-1;i++){
        for(j=0;j<total-1-i;j++){
            if(arreglo[j].fecha<arreglo[j+1].fecha){
                temp=arreglo[j];
                arreglo[j]=arreglo[j+1];
                arreglo[j+1]=temp;
            }
        }
    }

    printf("Ventas entre las fechas indicadas:\n");
    for(i=0;i<total;i++){
        printf("Vendedor: %s | Producto: %s | Marca: %s | Cantidad: %d | Monto: %.2f | Fecha: ",
            arreglo[i].nombreVendedor, arreglo[i].nombreProducto, arreglo[i].marca,
            arreglo[i].cantidad, arreglo[i].monto);
        MostrarFecha(arreglo[i].fecha);
        printf("\n");
    }
}

//Bucle principal de la aplicacion que organiza el acceso interactivo al sistema C++ DirVen
int main(){
    int menu=1;
    int option=1;
    int codigo;
    int CodigoAsociados;
    int contador_operacion=1;
    
    //Se inician los punteros en NULO
    Sproducto* ListaProductos=NULL;
    Sasociado* ListaAsociados=NULL;
	Sproducto* prueba;
    Sasociado* pruebasociado;
    
    //Carga automatica inicial de archivos
	CargarProductos(&ListaProductos);
	CargarAsociados(&ListaAsociados);
	CargarVentas(ListaAsociados, &contador_operacion);
	
    while (menu!=0){
    system("cls");
    printf("\n\n\t\tSistema de ventas DirVen\n\n");  
    printf("1. Asociados \n");  
    printf("2. Productos \n"); 
    printf("3. Ventas \n");
    printf("4. Reportes \n");  
    printf("\n0. Salir\n");  
    if (scanf("%d", &menu) != 1) {
            menu = -1; 
    }
    while (getchar() != '\n');
//======================================MENU ASOCIADOS======================================
    switch (menu){
        case 0: {
            system("cls");
            printf("Saliendo, hasta luego! \n");
            system("pause");
            return 0;
        }
        
        case 1: 
        while (option!=0){
        system("cls");
        printf("\n\n1.1.1.1 Agregar \n"); // CAMBIAR
        printf("1.1.1.2 Consultar por codigo \n");
        printf("1.1.1.3 Consultar por nombre \n");
        printf("1.1.1.4 Modificar por codigo \n");
        printf("1.1.1.5 Eliminar por codigo \n");
        printf("1.1.1.6 Mostrar todos los asociados \n");
        printf("1.1.1.0 Salir \n");
        fflush(stdin);
        if (scanf("%d", &option) != 1) {
            option = -1; 
        }
        while (getchar() != '\n');
            switch(option){
                case 0: {
                    break;
                }
                case 1: {
                    system("cls");
                    Sasociado* NewAsociado=NuevoAsociado(ListaAsociados);
                    AgregarAsociados(&ListaAsociados, &NewAsociado);
					GuardarAsociados(ListaAsociados);
                    printf("\n");
                    system("pause");
                    break;
                }
                case 2: {
                    system("cls");
                    printf("Introduzca el codigo que desea consultar: ");
                    fflush(stdin);scanf("%d", &codigo);fflush(stdin);
                    ConsultarCodigoAsociado(ListaAsociados, codigo);
                    printf("\n");
                    system("pause");
                    break;
                }
                case 3: {
                    char nombre[100];
                    system("cls");  
                    printf("Introduzca el nombre del asociado que desea consultar: \n");
                    fflush(stdin);scanf(" %99[^\n]", nombre);fflush(stdin);
                    while(strlen(nombre)>sizeof(ListaAsociados->nombre)){
                        printf("El nombre excede los 30 caracteres, por favor introduzca uno mas corto");
                        fflush(stdin);fflush(stdin);scanf(" %99[^\n]", nombre);fflush(stdin);
                    }
                    BuscarPorNombreAsociado(ListaAsociados, nombre);
                    printf("\n");
                    system("pause");
                    break;
                }
                case 4: {
                    system("cls");  
                    printf("Que codigo deseas modificar: ");
                    fflush(stdin);scanf("%d", &codigo);fflush(stdin);
                    pruebasociado=ExisteAsociado(ListaAsociados, codigo);
                    if(pruebasociado != NULL){
                        ModificarCodigoAsociado(pruebasociado);
                    } 
                    else {
                        printf("No se encontro el producto con ese codigo.\n");
                    }
                    break;
                }
                case 5: {
                    system("cls");  
                    printf("Que codigo desea eliminar?: ");
                    fflush(stdin);scanf("%d", &codigo);fflush(stdin);
                    EliminarPorCodigoAsociado(&ListaAsociados, codigo);
					GuardarAsociados(ListaAsociados);
                    printf("\n");
                    system("pause");
                    break;
                }
                case 6: {
                    system("cls");  
                    MostrarAsociados(ListaAsociados);
                    printf("\n");
                    printf("");
                    system("pause");
                    break;
                }
                default: {
                    system("cls");
                    fflush(stdin);
                    printf("Por favor introduzca una opcion valida. \n");
                    printf("\n");
                    system("pause");
                }
            }
        }
        option=-1;
        break;
        
//======================================MENU PRODUCTOS======================================
        case 2: 
        while (option!=0){
        system("cls");
        printf("\n\n1.1.1.1 Agregar \n"); 
        printf("1.1.1.2 Consultar por codigo \n"); 
        printf("1.1.1.3 Consultar por nombre \n"); 
        printf("1.1.1.4 Modificar por codigo \n"); 
        printf("1.1.1.5 Eliminar por codigo \n");  
        printf("1.1.1.6 Mostrar todos los productos \n"); 
        printf("1.1.1.0 Salir \n");
        if (scanf("%d", &option) != 1) {
            option = -1; 
        }
        while (getchar() != '\n');
            switch(option){
                case 0:{
                    break;
                }
                case 1: {
                    system("cls");
                    Sproducto* NewProducto=NuevoProducto(ListaProductos);
                    AgregarProductos(&ListaProductos, &NewProducto);
					GuardarProductos(ListaProductos);
                    printf("\n");
                    system("pause");
                    break;
                }

                case 2: {
                    system("cls");
                    printf("Introduzca el codigo que desea consultar: ");
                    fflush(stdin);scanf("%d", &codigo);fflush(stdin);
                    ConsultarCodigo(ListaProductos, codigo);
                    printf("\n");
                    system("pause");
                    break;
                }

                case 3: {
                    char nombre[100];
                    system("cls");
                    printf("Introduzca el nombre del producto que desea consultar: \n");
                    fflush(stdin);scanf(" %99[^\n]", nombre);fflush(stdin);
                    while(strlen(nombre)>sizeof(ListaProductos->nombre)){
                        printf("El nombre excede los 30 caracteres, por favor introduzca uno mas corto");
                        fflush(stdin);fflush(stdin);scanf(" %99[^\n]", nombre);fflush(stdin);
                    }
                    BuscarPorNombre(ListaProductos, nombre);
                    printf("\n");
                    system("pause");
                    break;
                }

                case 4: {
                    system("cls");
                    printf("Que codigo deseas modificar");
                    fflush(stdin);scanf("%d", &codigo);fflush(stdin);
                    prueba = ExisteProducto(ListaProductos, codigo);
                    if(prueba != NULL){
                        ModificarCodigo(prueba);
                    } 
                    else {
                        printf("No se encontro el producto con ese codigo.\n");
                    }
                    break;
                    
                }
                
                case 5: {
                    system("cls");
                    printf("Que codigo desea eliminar?: ");
                    fflush(stdin);scanf("%d", &codigo);fflush(stdin);
                    EliminarPorCodigo(&ListaProductos, codigo);
					GuardarProductos(ListaProductos);
                    printf("\n");
                    system("pause");
                    break;
                }
                case 6: {
                    system("cls");
                    MostrarProductos(ListaProductos);
                    printf("\n");
                    system("pause");
                    break;
                }
                default: {
                    system("cls");
                    printf("Por favor introduzca una opcion valida. ");
                    printf("\n");
                    system("pause");
                    break;
                }
            }
        }
        option=-1;
        break;
//======================================MENU VENTAS======================================
        case 3: {
        while (option!=0){
        system("cls");
        printf("\n\n3.1 Agregar venta \n");
        printf("3.2 Consultar por numero de operacion \n");
        printf("3.3 Eliminar venta (por codigo de operacion) \n");
        printf("3.4 Mostrar todas las ventas entre dos fechas \n");
        printf("3.0 Salir \n");
        fflush(stdin);
        if (scanf("%d", &option) != 1) {
            option = -1; 
        }
        while (getchar() != '\n');
            switch(option){
                case 0: {
                    break;
                }
                case 1: {
                    int codigo_vendedor;
                    int seguir;
                    system("cls");
                    printf("Ingrese el codigo del vendedor: ");
                    fflush(stdin);scanf("%d", &codigo_vendedor);fflush(stdin);
                    pruebasociado=ExisteAsociado(ListaAsociados, codigo_vendedor);
                    while(pruebasociado==NULL){
                        printf("El vendedor no existe, por favor introduzca un codigo valido: ");
                        fflush(stdin);scanf("%d", &codigo_vendedor);fflush(stdin);
                        pruebasociado=ExisteAsociado(ListaAsociados, codigo_vendedor);
                    }
                    seguir=1;
                    while(seguir==1){
                        system("cls");
                        Sventa* NewVenta=NuevaVenta(ListaProductos, &contador_operacion);
                        AgregarVenta(pruebasociado, NewVenta);
                        GuardarVentas(ListaAsociados);
                        printf("\n");
                        printf("Desea agregar otra venta para este vendedor? (1-Si / 0-No): ");
                        fflush(stdin);scanf("%d", &seguir);fflush(stdin);
                    }
                    break;
                }
                case 2: {
                    int numOp;
                    system("cls");
                    printf("Introduzca el numero de operacion que desea consultar: ");
                    fflush(stdin);scanf("%d", &numOp);fflush(stdin);
                    ConsultarVentaPorOperacion(ListaAsociados, ListaProductos, numOp);
                    printf("\n");
                    system("pause");
                    break;
                }
                case 3: {
                    int numOp;
                    system("cls");  
                    printf("Introduzca el numero de operacion que desea eliminar: ");
                    fflush(stdin);scanf("%d", &numOp);fflush(stdin);
                    EliminarVentaPorOperacion(ListaAsociados, numOp);
                    GuardarVentas(ListaAsociados);
                    printf("\n");
                    system("pause");
                    break;
                }
                case 4: {
                    int codigo_vendedor;
                    int fechaIni, fechaFin;
                    system("cls");  
                    printf("Ingrese el codigo del vendedor: ");
                    fflush(stdin);scanf("%d", &codigo_vendedor);fflush(stdin);
                    pruebasociado=ExisteAsociado(ListaAsociados, codigo_vendedor);
                    while(pruebasociado==NULL){
                        printf("El vendedor no existe, por favor introduzca un codigo valido: ");
                        fflush(stdin);scanf("%d", &codigo_vendedor);fflush(stdin);
                        pruebasociado=ExisteAsociado(ListaAsociados, codigo_vendedor);
                    }
                    printf("Ingrese la fecha inicial (formato AAAAMMDD): ");
                    fflush(stdin);scanf("%d", &fechaIni);fflush(stdin);
                    while(!EsFechaValida(fechaIni)){
                        printf("La fecha no es valida, por favor introduzca de nuevo: ");
                        fflush(stdin);scanf("%d", &fechaIni);fflush(stdin);
                    }
                    printf("Ingrese la fecha final (formato AAAAMMDD): ");
                    fflush(stdin);scanf("%d", &fechaFin);fflush(stdin);
                    while(!EsFechaValida(fechaFin)){
                        printf("La fecha no es valida, por favor introduzca de nuevo: ");
                        fflush(stdin);scanf("%d", &fechaFin);fflush(stdin);
                    }
                    MostrarVentasVendedorEntreFechas(pruebasociado, ListaProductos, fechaIni, fechaFin);
                    printf("\n");
                    system("pause");
                    break;
                }
                default: {
                    system("cls");
                    printf("Por favor introduzca una opcion valida. ");
                    printf("\n");
                    system("pause");
                    break;
                }
            }
        }
        option=-1;
        break;
        }
//======================================MENU REPORTES======================================
        case 4: {
        while (option!=0){
        system("cls");
        printf("\n\n4.1 Dado un producto mostrar todas sus ventas \n");
        printf("4.2 Dada una marca mostrar todas sus ventas \n");
        printf("4.3 Dado un vendedor mostrar todas sus ventas \n");
        printf("4.4 Mostrar todas las ventas entre dos fechas \n");
        printf("4.0 Salir \n");
        fflush(stdin);
        if (scanf("%d", &option) != 1) {
            option = -1; 
        }
        while (getchar() != '\n');
            switch(option){
                case 0: {
                    break;
                }
                case 1: {
                    system("cls");
                    Reporte1_Producto(ListaAsociados, ListaProductos);
                    printf("\n");
                    system("pause");
                    break;
                }
                case 2: {
                    system("cls");
                    Reporte2_Marca(ListaAsociados, ListaProductos);
                    printf("\n");
                    system("pause");
                    break;
                }
                case 3: {
                    system("cls");
                    Reporte3_Vendedor(ListaAsociados, ListaProductos);
                    printf("\n");
                    system("pause");
                    break;
                }
                case 4: {
                    system("cls");
                    Reporte4_Fechas(ListaAsociados, ListaProductos);
                    printf("\n");
                    system("pause");
                    break;
                }
                default: {
                    system("cls");
                    printf("Por favor introduzca una opcion valida. ");
                    printf("\n");
                    system("pause");
                    break;
                }
            }
        }
        option=-1;
        break;
        }
        default: {
            system("cls");
            printf("Por favor introduzca una opcion valida. SKIBIDI. \n");
            system("pause");
            break; 
        }
        }
    }
    return 0;
}