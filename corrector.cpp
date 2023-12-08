/*****************************************************************************************************************
	UNIVERSIDAD NACIONAL AUTONOMA DE MEXICO
	FACULTAD DE ESTUDIOS SUPERIORES -ARAGON-

	Computadoras y programacion.
	(c) Fernando Manjarrez Alvarez 321264934

	Quiso decir: Programa principal de la aplicacion de la distancia de Levenstein.

******************************************************************************************************************/
#include "stdafx.h"
#include <string.h>
#include "corrector.h"
#include <stdlib.h>
#include <ctype.h>
#define LONGI 32
//#define DEPURAR 1
char abecedario[LONGI + 6] = "abcdefghijklmnñopqrstuvwxyzáéíóú";//tambien probe con las vocales en ascii pero el resultado era el mismo por lo que no er tan necesario
//Funciones publicas del proyecto
/*****************************************************************************************************************
	DICCIONARIO: Esta funcion crea el diccionario completo
	char *	szNombre				:	Nombre del archivo de donde se sacaran las palabras del diccionario
	char	szPalabras[][TAMTOKEN]	:	Arreglo con las palabras completas del diccionario
	int		iEstadisticas[]			:	Arreglo con el numero de veces que aparecen las palabras en el diccionario
	int &	iNumElementos			:	Numero de elementos en el diccionario
******************************************************************************************************************/
//void donde se haran los procesos para el diccionario 
void Diccionario(char* szNombre, 
	char szPalabras[][TAMTOKEN],
	int iEstadisticas[], 
	int& iNumElementos)
{
   //variable del archivo
	FILE* fp;
	iNumElementos = 0;
	int i;
	//Guardar datos 
	char lectura, array[TAMTOKEN];

	//arreglo estadisticas
	for (i = 0; i < NUMPALABRAS; i++)
		iEstadisticas[i] = 0;

	//apertura archivo
	fopen_s(&fp, szNombre, "r");

	//variable para el contador 
	int contador = 0;

	//metodo mas eficiente para los signos de puntuacion el anterior era muy extenso pero con este se resume en gran medida
	char puntuacion[] = " \t\n\r.,;() ";
	

	//verificar la apertura del archivo 
	if (fp == NULL)
		return;

	
	while ((lectura = fgetc(fp)) != EOF) 
	{
		//banderazo
		bool bandera = true;

		//palabras a minusculas 
		lectura = tolower(lectura);

		
		for (i = 0; i < strlen(puntuacion); i++)

			//signos de puntuacion 
			if (puntuacion[i] == lectura)
				bandera = false;

		
		if (contador < TAMTOKEN && bandera) {
			//agregar al arreglo 
			array[contador++] = lectura;
			continue;
		}
		//verificar continuidad 
		else if (contador == 0)
			continue;

		
		else if (contador == 1 && bandera) 
		{
			contador = 0;
			continue;
		}

		array[contador] = '\0';

		//comprobar el abecedario 
		for (i = 0; i < iNumElementos && !bandera; i++) 
		{

			//compara palabras 
			if (strcmp(szPalabras[i], array) == 0) 
			{
				iEstadisticas[i]++;
				bandera = true;
			}
		}
		
		if (!bandera) {
		//pasa palabras a szPalabras 
			strcpy_s(szPalabras[iNumElementos], array);
			iEstadisticas[iNumElementos++]++;

		}
		contador = 0;
	}//fin del documento
	fclose(fp);


	//Burbujazo
	for (int j = 0; j < iNumElementos - 1; j++) 
	{

		for (i = j + 1; i < iNumElementos; i++) 
		{

			if (strcmp(szPalabras[j], szPalabras[i]) > 0) 
			{
				strcpy_s(array, szPalabras[j]);
				strcpy_s(szPalabras[j], szPalabras[i]);
				strcpy_s(szPalabras[i], array);
				contador = iEstadisticas[j];
				iEstadisticas[j] = iEstadisticas[i];
				iEstadisticas[i] = contador;
			}
		}
	}
	//Fin burbujazo
}
//fin del void Diccionario 
/*****************************************************************************************************************
	ListaCandidatas: Esta funcion recupera desde el diccionario las palabras validas y su peso
	Regresa las palabras ordenadas por su peso
	char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Lista de palabras clonadas
	char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
	int		iEstadisticas[],					//Lista de las frecuencias de las palabras
	int		iNumElementos,						//Numero de elementos en el diccionario
	char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
	int		iPeso[],							//Peso de las palabras en la lista final
	int &	iNumLista)							//Numero de elementos en la szListaFinal
******************************************************************************************************************/
//Lista candidatas es la parte final del codigo pero debido a que lo hice en desorden si le muevo me da miedo que no funcione 
void	ListaCandidatas(
	char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Lista de palabras clonadas
	char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
	int		iEstadisticas[],					//Lista de las frecuencias de las palabras
	int		iNumElementos,						//Numero de elementos en el diccionario
	char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
	int		iPeso[],							//Peso de las palabras en la lista final
	int& iNumLista)							    //Numero de elementos en la szListaFinal
{
	//comienzo de los procesos 
	iNumLista = 0;
	for (int i = 0; i < iNumSugeridas; i++)
	{
		for (int j = 0; j < iNumElementos; j++)
		{

			//compara palabras 
			if (strcmp(szPalabrasSugeridas[i], szPalabras[j]) == 0)
			{
				//declara bandera 
				bool bandera = false;
				for (int k = 0; k < iNumLista && !bandera; k++)

					if (strcmp(szListaFinal[k], szPalabras[j]) == 0)
						bandera = true;

				if (bandera) continue;
				//lista para las candidatas 
				strcpy_s(szListaFinal[iNumLista], szPalabrasSugeridas[i]);
				iPeso[iNumLista++] = iEstadisticas[j];
			}
		}
	}
	//Elemntos en la lista final 
	for (int i = 0; i < iNumLista; i++)
	{
		for (int j = 0; j < iNumLista - 1; j++)
		{
			if (iPeso[j] < iPeso[j + 1])
			{

				int iaux; char caux[50];
				strcpy_s(caux, szListaFinal[j + 1]); iaux = iPeso[j + 1];
				strcpy_s(szListaFinal[j + 1], szListaFinal[j]); iPeso[j + 1] = iPeso[j];
				strcpy_s(szListaFinal[j], caux); iPeso[j] = iaux;
			}
		}
	}
}

/*****************************************************************************************************************
	ClonaPalabras: toma una palabra y obtiene todas las combinaciones y permutaciones requeridas por el metodo
	char *	szPalabraLeida,						//Palabra a clonar   palabra
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int &	iNumSugeridas)						//Numero de elementos en la lista
******************************************************************************************************************/
//aqui se hacen los procesos donde la letra va a pasar por la palabra todas las letras del diccionario 
void	ClonaPalabras(
	char* szPalabraLeida,						// Palabra a clonar
	char  szPalabrasSugeridas[][TAMTOKEN], 	    //Lista de palabras clonadas
	int& iNumSugeridas)						    //Numero de elementos en la lista
{
    //variable para usar durante la clonacion
	char aux[TAMTOKEN]; iNumSugeridas = 0;

	//szPalabraleida a auxiliar 
	strcpy_s(aux, szPalabraLeida);

	//pasar el abecedario en los caracteres 
	for (int i = 0; i < strlen(szPalabraLeida); i++) {

		for (int j = 0; j < LONGI; j++) {
			aux[i] = abecedario[j];
		//Posicion a la lista de palabras
			strcpy_s(szPalabrasSugeridas[iNumSugeridas++], aux);
		} aux[i] = szPalabraLeida[i];
	}
	//Pasar el abecedario entre los caracteres
	int k;
	for (k = 1; k < strlen(szPalabraLeida) + 1; k++) {
		aux[k] = szPalabraLeida[k - 1];

		//Asignar espacios 
	} aux[k] = '\0';

	for (int i = 0; i < strlen(szPalabraLeida) + 1; i++) {

		for (int j = 0; j < LONGI; j++) {
			aux[i] = abecedario[j];
			
			strcpy_s(szPalabrasSugeridas[iNumSugeridas++], aux);
		}//comprobar que sea igual a aux 
		aux[i] = szPalabraLeida[i];
	}
	//Para suprimir los caracteres
	// j y i son usualmente las letras caractetisticas para los for 
	int contador = 0;
	for (int i = 0; i < strlen(szPalabraLeida) && strlen(szPalabraLeida) != 1; i++) {

		for (int j = 0; j < strlen(szPalabraLeida); j++) {
			if (j != i)
				aux[contador++] = szPalabraLeida[j];

			/*Para asignar espacios*/
		} aux[contador] = '\0';

		/*Numero de elementos en la lista a la Lista de palabras clonadas*/
		strcpy_s(szPalabrasSugeridas[iNumSugeridas++], aux);

		/*Palabra a clonar palabra al auxiliar*/
		strcpy_s(aux, szPalabraLeida);

		contador = 0;
	}
	//Transposición de los caracteres.
	for (int i = 0; i < strlen(szPalabraLeida) - 1; i++) {
		aux[i] = szPalabraLeida[i + 1];
		aux[i + 1] = szPalabraLeida[i];

		
		strcpy_s(szPalabrasSugeridas[iNumSugeridas++], aux);
		//clona palabras a aux
		strcpy_s(aux, szPalabraLeida);
	}
	strcpy_s(szPalabrasSugeridas[iNumSugeridas++], szPalabraLeida);


	//Burbujazo
	for (int j = 0; j < iNumSugeridas - 1; j++) {

		for (int i = j + 1; i < iNumSugeridas; i++) {

			if (strcmp(szPalabrasSugeridas[j], szPalabrasSugeridas[i]) > 0) {
			   //Paso de palabras 
				strcpy_s(aux, szPalabrasSugeridas[j]);
				strcpy_s(szPalabrasSugeridas[j], szPalabrasSugeridas[i]);
				strcpy_s(szPalabrasSugeridas[i], aux);
			}
		}
	}


}
/*¨final de todo el codigo con todas las funciones con las que ya funciona asi que se consider la 
version final despues de tantas depuradas posee algunas caracteristicas que trabaje con tutoriales 
para mejorar el codigo y obtener uno que funcione como lo solicita la asignacion*/
