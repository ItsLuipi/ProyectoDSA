#include <string.h>
#include <iostream>
using namespace std;
//LUIS LARA Y GABRIEL FREAY

#define MAX_VENTAS 1000
    

//SIGMA SIGMA BOY 
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
    if(!(lista)){
        return true;
    }
    else{
        while(lista){
            if(lista->codigo==codigo){
                return false;
            }
            else{lista=lista->psig;}
        }
        return true;
    }
}

bool BuscarCodigoAsociado(Sasociado *lista, int codigo){
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

void ToLower(const char origen[], char destino[]) {
    size_t i = 0;
    while (origen[i] != '\0') {
        char c = origen[i];
        if (c >= 'A' && c <= 'Z') {
            destino[i] = c - 'A' + 'a';
        } else {
            destino[i] = c;
        }
        ++i;
    }
    destino[i] = '\0';
}

bool IsNumeric(const char origen []){
    int i = 0;
    while (origen[i] != '\0'){
        char c = origen[i];
        if(!isdigit(c)){
            return false;
        }
        i++;
    }
    return true;
}

static int ContainsIgnoreCase(const char texto[], const char patron[]) {
    char textoMinusculas[200];
    char patronMinusculas[200];
    ToLower(texto, textoMinusculas);
    ToLower(patron, patronMinusculas);
    return strstr(textoMinusculas, patronMinusculas) != NULL;
}

void GuardarProductos(Sproducto *lista){
	FILE *f = fopen("productos.txt", "w");
	if(!f) { 
		printf("Error al abrir el archivo .txt \n");
		return;
	}
	while(lista) {
		fprintf(f, "%d|%s|%s|%s|%.2f\n",
			lista->codigo,
			lista->nombre,
			lista->marca,
			lista->descripcion,
			lista->precio);
		lista=lista->psig;
	}
	fclose(f);
}

void CargarProductos(Sproducto **lista) {
	FILE *f = fopen("productos.txt", "r");
	if(!f) {return;}
	while(1) {
		Sproducto *nuevo = new Sproducto;
		if (fscanf(f, "%d|%29[^|]|%29[^|]|%99[^|]|%f\n",
			&nuevo->codigo,
			nuevo->nombre,
			nuevo->marca,
			nuevo->descripcion,
			&nuevo->precio) == 5) {
			nuevo->psig = *lista;
			*lista = nuevo;
		}
		else{
			delete nuevo;
			break;
		}
	}
	fclose(f);
}

void GuardarAsociados(Sasociado *lista){
	FILE *f = fopen("asociados.txt", "w");
	if (!f) {
	printf("error al abrir asociados.txt\n");
	return;
	}
	while (lista){
	fprintf(f,  "%d|%s|%s|%s\n",
		lista->codigo,
		lista->nombre,
		lista->direccion,
		lista->telefono);
	lista = lista->pnext;
	}
	fclose(f);
}

void CargarAsociados(Sasociado **lista) {
    FILE *f = fopen("asociados.txt", "r");
    if (!f) return;
    Sasociado *nuevo;
    while (1) {
        nuevo = new Sasociado;
        if (fscanf(f, "%d|%49[^|]|%99[^|]|%20[^\n]\n",
            &nuevo->codigo,
            nuevo->nombre,
            nuevo->direccion,
            &nuevo->telefono) == 4) {
            nuevo->pventas = NULL;
            nuevo->pnext = *lista;
            *lista = nuevo;
        } else {
            delete nuevo;
            break;
        }
    }
    fclose(f);
}

//============================ASOCIADOS============================
void AgregarAsociados(Sasociado **lista, Sasociado** asociado){
        (*asociado)->pnext=*lista;
        *lista=*asociado;
        printf("Codigo agregado con exito!");
}

Sasociado* NuevoAsociado(Sasociado *ListaAsociados){
    Sasociado *nuevo=new Sasociado;
    char nombre[100];
    char direccion[150];
    char telefono[21];
    int codigo;
	int valido=0;
    
    printf("Ingrese el codigo del Asociado: ");
    fflush(stdin);
    while(scanf("%d", &codigo) != 1){
        printf("El codigo es invalido, porfavor intente de nuevo. \n");
        while (getchar() != '\n');
        ("%d", &codigo);
    }
    while(!(BuscarCodigoAsociado(ListaAsociados, codigo))){
        printf("El codigo ya se encuentra en la lista, por favor introduzca uno distinto: ");
        fflush(stdin);scanf("%d", &codigo);fflush(stdin);
    }

    while(!(BuscarCodigoAsociado(ListaAsociados, codigo))){
        printf("El codigo ya se encuentra en la lista, por favor introduzca uno distinto: ");
        fflush(stdin);scanf("%d", &codigo);fflush(stdin);
    }
    nuevo->codigo=codigo;

    printf("Ingrese el nombre del Asociado (No puede ser mayor de 30 caracteres): \n");
    fflush(stdin);fflush(stdin);scanf(" %99[^\n]", nombre);fflush(stdin);
    while(strlen(nombre)>sizeof(nuevo->nombre)){
        printf("El nombre excede los 30 caracteres, por favor introduzca uno mas corto");
        fflush(stdin);fflush(stdin);scanf(" %99[^\n]", nombre);fflush(stdin);
    }
    strcpy(nuevo->nombre, nombre);

    printf("Ingrese la direcccion del Asociado (No puede ser mayor de 100 caracteres): \n");
    fflush(stdin);fflush(stdin);scanf(" %149[^\n]", direccion);fflush(stdin);
    while(strlen(direccion)>sizeof(nuevo->direccion)){
        printf("La direccion excede los 100 caracteres, por favor introduzca uno mas corto");
        fflush(stdin);fflush(stdin);scanf(" %149[^\n]", direccion);fflush(stdin);
    }
    strcpy(nuevo->direccion, direccion);

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

    nuevo->pnext=NULL;
    nuevo->pventas=NULL;
    return nuevo;
}

void MostrarAsociado(Sasociado *p){
    if(!p) return;
    printf("Nombre: %s | Direccion: %s | Numero de telefono: %s | Codigo del producto: %d\n", 
    p->nombre, p->direccion, p->telefono, p->codigo);
}

void ConsultarCodigoAsociado(Sasociado *lista, int n){
    if(!(lista)){
        printf("No hay asociados en la lista\n");
    }

    else{
        Sasociado *aux = lista;
        while(aux && aux->codigo != n){
            aux = aux->pnext;
        }

        if(aux && aux->codigo == n){
            MostrarAsociado(aux);
        }
        else{
            printf("El codigo no esta en la lista\n");
        }
    }
}

void BuscarPorNombreAsociado(Sasociado *lista, const char nombre[]){
    if(!lista){
        printf("No hay productos en la lista\n");
        return;
    }

    bool encontrado = false;
    while(lista){
        if(ContainsIgnoreCase(lista->nombre, nombre)){
            MostrarAsociado(lista);
            encontrado = true;
        }
        lista = lista->pnext;
    }

    if(!encontrado){
        printf("No se encontro ningun producto con ese nombre\n");
    }
}

Sasociado* ExisteAsociado(Sasociado *lista, int n){
    Sasociado* ax = lista;
    while(ax && ax->codigo != n){
        ax = ax->pnext;
    }
    return ax;
}

void ModificarCodigoAsociado(Sasociado *lista){
    int CambioNum;
    int menu;
    char Cambio[150];

    if(lista==NULL){
        printf("La lista esta vacia. ");
        return;
    }
    else{
    printf("Que deseas modificar: \n");
    printf("1- Nombre. \n");
    printf("2- Direccion. \n");
    printf("3- Telefono. \n");
    printf("4- Codigo. \n");
    fflush(stdin);scanf("%d", &menu);fflush(stdin);
    switch(menu){
        case 1:
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

void EliminarPorCodigoAsociado(Sasociado **a, int n){
    Sasociado *aux;
    Sasociado *t;
    if((*a)->codigo==n){
        aux=*a;
        *a=(*a)->pnext;
        delete aux;
    }
    else{
        aux=*a;
        while(aux->pnext && aux->pnext->codigo!=n){
            aux=aux->pnext;
        }
            if(aux->pnext!=NULL){
                t=aux->pnext;
                aux->pnext=t->pnext;
                delete t;
                printf("El codigo fue eliminado con exito!");
            }
            else{
                printf("El codigo no esta en la lista");
            }
    }
}

void MostrarAsociados(Sasociado *lista){
    while(lista){
        printf("Nombre: %s | Direccion: %s | Numero de telefono: %s | Codigo del producto: %d\n", 
        lista->nombre, lista->direccion, lista->telefono, lista->codigo);
        lista=lista->pnext;
    }
}
//============================PRODUCTO============================
Sproducto* NuevoProducto(Sproducto *ListaProductos){ 
    Sproducto *nuevo=new Sproducto;
    char temp[100];
    char marca[100];
    char descripcion[150];
    int codigo;
	int entrada_valida;

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

    printf("Ingrese el precio del producto: ");
    fflush(stdin);scanf("%f", &nuevo->precio);fflush(stdin);
    while(nuevo->precio<=0){
        printf("Por favor introduzca un numero mayor a 0: \n");
        fflush(stdin);scanf("%f", &nuevo->precio);fflush(stdin);
    }

    nuevo->codigo=codigo;
    nuevo->psig=NULL;
    return nuevo;
    
}

void AgregarProductos(Sproducto **lista, Sproducto** producto){
        (*producto)->psig=*lista;
        *lista=*producto;
        printf("Codigo agregado con exito!");
}

void MostrarProductos(Sproducto *lista){
    while(lista){
        printf("Nombre: %s | Nombre de la marca: %s | Descripcion del producto: %s | Codigo del producto: %d\n", 
        lista->nombre, lista->marca, lista->descripcion, lista->codigo);
        lista=lista->psig;
    }
}

void MostrarProducto(Sproducto *p){
    if(!p) return;
        printf("Nombre: %s | Nombre de la marca: %s | Descripcion del producto: %s | Codigo del producto: %d\n", 
        p->nombre, p->marca, p->descripcion, p->codigo);
}

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

void BuscarPorNombre(Sproducto *lista, const char nombre[]){
    if(!lista){
        printf("No hay productos en la lista\n");
        return;
    }

    bool encontrado = false;
    while(lista){
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

Sproducto* ExisteProducto(Sproducto *lista, int n){
    Sproducto* ax = lista;
    while(ax && ax->codigo != n){
        ax = ax->psig;
    }
    return ax;
}

void ModificarCodigo(Sproducto *lista){
    int mod;
    int menu;
    char Cambio[150];

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
    fflush(stdin);scanf("%d", &menu);fflush(stdin); //letras
    switch(menu){
        case 1:
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

void EliminarPorCodigo(Sproducto **a, int n){
    Sproducto *aux;
    Sproducto *t;
    if((*a)->codigo==n){
        aux=*a;
        *a=(*a)->psig;
        delete aux;
    }
    else{
        aux=*a;
        while(aux->psig && aux->psig->codigo!=n){
            aux=aux->psig;
        }
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
bool EsFechaValida(int fecha){
    int anio = fecha/10000;
    int mes = (fecha/100)%100;
    int dia = fecha%100;
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

Sventa* NuevaVenta(Sproducto *ListaProductos, int *contador){
    Sventa *nueva=new Sventa;
    Sproducto *prod;
    int codigo_producto;
    int cantidad;
    int fecha;

    printf("Ingrese el codigo del producto vendido: ");
    fflush(stdin);scanf("%d", &codigo_producto);fflush(stdin);
    prod=ExisteProducto(ListaProductos, codigo_producto);
    while(prod==NULL){
        printf("El producto no existe, por favor introduzca un codigo valido: ");
        fflush(stdin);scanf("%d", &codigo_producto);fflush(stdin);
        prod=ExisteProducto(ListaProductos, codigo_producto);
    }

    printf("Ingrese la cantidad vendida: ");
    fflush(stdin);scanf("%d", &cantidad);fflush(stdin);
    while(cantidad<=0){
        printf("La cantidad debe ser mayor a 0, por favor introduzca de nuevo: ");
        fflush(stdin);scanf("%d", &cantidad);fflush(stdin);
    }

    printf("Ingrese la fecha de la venta (formato AAAAMMDD): ");
    fflush(stdin);scanf("%d", &fecha);fflush(stdin);
    while(!EsFechaValida(fecha)){
        printf("La fecha no es valida, por favor introduzca de nuevo (formato AAAAMMDD): ");
        fflush(stdin);scanf("%d", &fecha);fflush(stdin);
    }

    nueva->num_operacion=*contador;
    *contador=*contador+1;
    nueva->codigo_producto=codigo_producto;
    nueva->cantidad=cantidad;
    nueva->precio_unidad=prod->precio;
    nueva->monto=prod->precio*cantidad;
    nueva->fecha=fecha;
    nueva->prventas=NULL;

    return nueva;
}

void AgregarVenta(Sasociado *vendedor, Sventa *venta){
    venta->prventas=vendedor->pventas;
    vendedor->pventas=venta;
    printf("Venta agregada con exito!");
}

void GuardarVentas(Sasociado *lista){
	FILE *f = fopen("ventas.txt", "w");
	Sventa *v;
	if(!f) { 
		printf("Error al abrir el archivo ventas.txt \n");
		return;
	}
	while(lista) {
		v=lista->pventas;
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

void CargarVentas(Sasociado *listaAsociados, int *contador) {
	FILE *f = fopen("ventas.txt", "r");
	int codigo_asociado;
	Sasociado *vendedor;
	Sventa *nueva;
	if(!f) {return;}
	while(1) {
		nueva = new Sventa;
		if (fscanf(f, "%d|%d|%d|%d|%f|%f|%d\n",
			&codigo_asociado,
			&nueva->num_operacion,
			&nueva->codigo_producto,
			&nueva->cantidad,
			&nueva->precio_unidad,
			&nueva->monto,
			&nueva->fecha) == 7) {
			vendedor=ExisteAsociado(listaAsociados, codigo_asociado);
			if(vendedor!=NULL){
				nueva->prventas=vendedor->pventas;
				vendedor->pventas=nueva;
				if(nueva->num_operacion>=*contador){
					*contador=nueva->num_operacion+1;
				}
			}
			else{
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

void MostrarFecha(int fecha){
    int anio = fecha/10000;
    int mes = (fecha/100)%100;
    int dia = fecha%100;
    printf("%02d/%02d/%d", dia, mes, anio);
}

void MostrarVentaDetalle(Sventa *v, Sasociado *vendedor, Sproducto *ListaProductos){
    Sproducto *prod = ExisteProducto(ListaProductos, v->codigo_producto);
    printf("Numero de operacion: %d\n", v->num_operacion);
    printf("Vendedor: %s\n", vendedor->nombre);
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

void ConsultarVentaPorOperacion(Sasociado *listaAsociados, Sproducto *ListaProductos, int numOp){
    Sasociado *aux=listaAsociados;
    Sventa *v;
    bool encontrado=false;

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

void EliminarVentaPorOperacion(Sasociado *listaAsociados, int numOp){
    Sasociado *aux=listaAsociados;
    Sventa *v, *t;
    bool encontrado=false;

    while(aux && !encontrado){
        if(aux->pventas!=NULL && aux->pventas->num_operacion==numOp){
            t=aux->pventas;
            aux->pventas=t->prventas;
            delete t;
            encontrado=true;
            printf("La venta fue eliminada con exito!");
        }
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

void MostrarVentasVendedorEntreFechas(Sasociado *vendedor, Sproducto *ListaProductos, int fechaIni, int fechaFin){
    Sventa *arreglo[MAX_VENTAS];
    int total=0;
    Sventa *v=vendedor->pventas;
    Sproducto *prod;
    int i, j;
    Sventa *temp;

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

    //Ordenado por numero de operacion
    for(i=0;i<total-1;i++){
        for(j=0;j<total-1-i;j++){
            if(arreglo[j]->num_operacion>arreglo[j+1]->num_operacion){
                temp=arreglo[j];
                arreglo[j]=arreglo[j+1];
                arreglo[j+1]=temp;
            }
        }
    }

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
struct SReporteProducto{
    char marca[30];
    int cantidad;
    float monto;
};

void Reporte1_Producto(Sasociado *listaAsociados, Sproducto *ListaProductos){
    int codigo_producto;
    Sproducto *prod;
    SReporteProducto arreglo[MAX_VENTAS];
    int total=0;
    Sasociado *aux;
    Sventa *v;
    int i, j;
    SReporteProducto temp;

    printf("Ingrese el codigo del producto que desea consultar: ");
    fflush(stdin);scanf("%d", &codigo_producto);fflush(stdin);
    prod=ExisteProducto(ListaProductos, codigo_producto);
    while(prod==NULL){
        printf("El producto no existe, por favor introduzca un codigo valido: ");
        fflush(stdin);scanf("%d", &codigo_producto);fflush(stdin);
        prod=ExisteProducto(ListaProductos, codigo_producto);
    }

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

    //Ordenado por cantidad de productos (de mayor a menor)
    for(i=0;i<total-1;i++){
        for(j=0;j<total-1-i;j++){
            if(arreglo[j].cantidad<arreglo[j+1].cantidad){
                temp=arreglo[j];
                arreglo[j]=arreglo[j+1];
                arreglo[j+1]=temp;
            }
        }
    }

    printf("Ventas del producto %s:\n", prod->nombre);
    for(i=0;i<total;i++){
        printf("Marca: %s | Cantidad de unidades: %d | Monto total: %.2f\n",
            arreglo[i].marca, arreglo[i].cantidad, arreglo[i].monto);
    }
}

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

    printf("Ingrese la marca que desea consultar: \n");
    fflush(stdin);scanf(" %99[^\n]", marca);fflush(stdin);

    aux=listaAsociados;
    while(aux){
        v=aux->pventas;
        while(v){
            prod=ExisteProducto(ListaProductos, v->codigo_producto);
            if(prod!=NULL && ContainsIgnoreCase(prod->marca, marca)){
                encontrado_prod=false;
                for(k=0;k<total;k++){
                    if(arreglo[k].codigo_producto==prod->codigo){
                        arreglo[k].cantidad=arreglo[k].cantidad+v->cantidad;
                        arreglo[k].monto=arreglo[k].monto+v->monto;
                        encontrado_prod=true;
                        break;
                    }
                }
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

    //Ordenado por monto total vendido en cada producto (de mayor a menor)
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

    printf("Ingrese el codigo del vendedor que desea consultar: ");
    fflush(stdin);scanf("%d", &codigo_vendedor);fflush(stdin);
    vendedor=ExisteAsociado(listaAsociados, codigo_vendedor);
    while(vendedor==NULL){
        printf("El vendedor no existe, por favor introduzca un codigo valido: ");
        fflush(stdin);scanf("%d", &codigo_vendedor);fflush(stdin);
        vendedor=ExisteAsociado(listaAsociados, codigo_vendedor);
    }

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

    //Ordenado por producto/marca
    for(i=0;i<total-1;i++){
        for(j=0;j<total-1-i;j++){
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

    //Ordenado por fecha (mas nuevo a mas antigua)
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


int main(){
    int menu=1;
    int option=1;
    int codigo;
    int CodigoAsociados;
    int contador_operacion=1;
    Sproducto* ListaProductos=NULL;
    Sasociado* ListaAsociados=NULL;
	Sproducto* prueba;
    Sasociado* pruebasociado;
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
