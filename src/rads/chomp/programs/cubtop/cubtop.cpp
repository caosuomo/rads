#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <iomanip>
#include <stdlib.h> 
#include <time.h>
using namespace std;
 
#include "ptkrepresentableset.h"

//#include "options.h" 
//#include "textfile.h" 
//#include "euclidom.h" 
//#include "chains.h" 
//#include "ptkbitmap.hh"

//char * copie_chaine_for(char *cible, const char *origine);

void minusdot(char *ch)
{
 	int pos=0;
 	while( (ch[pos] && (ch[pos]!='.')))
  		pos++;
	ch[pos]=0;
}

bool hasadot (char *ch)
{
	bool retour = false;
	int pos=0;
 	while( (ch[pos] )){
		if (ch[pos]=='.'){
			retour = true;
			}
		pos++;
	}
	return retour;
}
	
void minusblank(char *ch)
{
 	int pos=0;
 	while( ch[pos] )
  		pos++;
	ch[pos]=0;
}

//strcat(dest,source);   "allo" (dest) "salut" (source) "allosalut" (dest)

const char *title = "CUBTOP, ver. Alpha, 2002 by S. Bérubé et A. Trahan\n"
	"Copyright (C) 2002-2005 by Universite de Sherbrooke.\n";
 
const char *help0=
"Help of the cubtop program \n\n\
    CubTop contains the following functions :\n\
      - union;\n\
      - intersection;\n\
      - disjoined union;\n\
      - freefaces;\n\
      - properfaces;\n\
      - maximalfaces;\n\
      - topobounds;\n\
      - getdimension;\n\
      - openhull;\n\
      - closedhull.\n\
\n\
    All these functions serve to manipulate representable (or cubical) sets\n\
    stored on a file.\n\
\n\
\n\
    The 'representation' of a representable set on a file look like this :\n\
\n\
        Type of set : R\n\
        Space Dimension : 2\n\
        (4) x (4)\n\
        (0) x (0,1)\n\
        (3,4) x (2)\n\
        (1,2) x (2,3)\n\
\n\
    The 'representation' of a cubical set on a file look like this :\n\
\n\
        Type of set : C\n\
        Space Dimension : 2\n\
        [1] x [2]\n\
        [3] x [1,2]\n\
        [3] x [2,3]\n\
        [3,4] x [-1,0]\n\
\n\
      - The first line 'Type of set : R' indicate if this is a cubical or a\n\
        representable set.\n\
\n\
      - The second line 'Space Dimension : 2' indicate the embedding number of\n\
        the set.\n\
\n\
      - All the other lines indicate the elementary cell (or cube) which constitute\n\
        the set. So, the set is the union of these elementary cell or cute.\n\
\n\
\n\
\n\
    Execute the program CubTop without any parameters to see the general \n\
    help of the program.\n\
\n\
        server> cubtop  [enter]\n\
        server> *** you'll see the general help of cubtop ***\n\
\n\
    To see the help of a specific function, simply call the function\n\
    without any parameters. For exemple :\n\
\n\
        server> cubtop unio  [enter]\n\
        server> *** you'll see the help of the function union ***\n\
\n\
    You can call every function by only 4 caracters\n \n";
	
    

const char *help1= 
"Help of the function union \n\n\
Call: cubtop union X.txt Y.txt [-o [fileX] ] [-g [fileY]] [-c] [-b]\n\n\
This program computes the union of representable sets or cubical sets.  \n\
The program reads sets from input files, performs computations and  \n\
outputs the results to the screen ('cout').  If you want to make\n\
the union of cubical sets, the program converts the cubical sets in a \n\
representable set and the answer will be by default in a representable \n\
set. \n\n\
Parameters:\n\
-o [file] - create a output file //default file is unioX_Y.txt\n\
-g [file] - create a bmp file of a 1D or 2D set //default file is unioX_Y.bmp\n\
-c	  - show the result in a cubical set //if possible\n\
-b	  - results will not be printed on screen \n\
\n\
For help for the input files, look at the exemples or type: cubtop\n\
\n\
For more details ask the authors: \n\
berus00@dmi.usherb.ca or anik.trahan@dmi.usherb.ca.\n \n";

const char *help2 = 
"Help of the function intersection \n\n\
Call: cubtop intersection X.txt Y.txt [-o [fileX] ] [-g [fileY]] [-c] [-b]\n \n\
This program computes the intersection of representable sets or cubical \n\
sets.  The program reads sets from input files, performs computations \n\
and outputs the results to the screen ('cout').  If you want to make\n\
the intersection of cubical sets, the program converts the cubical sets \n\
in a representable set and the answer will be by default in a \n\
representable set. \n\n\
Parameters:\n\
-o [file] - create a output file //default file is inteX_Y.txt\n\
-g [file] - create a bmp file of a 2D set //default file is inteX_Y.bmp\n\
-c	  - show the result in a cubical set //if possible\n\
-b	  - results will not be printed on screen \n\
\n\
For help for the input files, look at the exemples or type: cubtop\n\
\n\
For more details ask the authors: \n\
berus00@dmi.usherb.ca or anik.trahan@dmi.usherb.ca.\n \n";

const char *help3 = 
"Help of the function disjoined union \n\n\
Call: cubtop disjunion X.txt Y.txt [-o [fileX] ] [-g [fileY]] [-c] [-b]\n \n\
This program computes the disjoined union of representable sets or \n\
cubical sets.  The program reads sets from input files, performs \n\
computations and outputs the results to the screen ('cout').  If you \n\
want to make the disjoined union of cubical sets the  program converts \n\
the cubical sets in a representable set and the answer will be by \n\
default in a representable set. \n\n\
Parameters:\n\
-o [file] - create a output file //default file is disjX_Y.txt\n\
-g [file] - create a bmp file of a 1D or 2D set //default file is disjX_Y.bmp\n\
-c	  - show the result in a cubical set //if possible\n\
-b	  - results will not be printed on screen \n\
\n\
For help for the input files, look at the exemples or type: cubtop\n\
\n\
For more details ask the authors: \n\
berus00@dmi.usherb.ca or anik.trahan@dmi.usherb.ca.\n \n";


const char *help4 = 
"Help of the function freefaces \n\n\
Call: cubtop freefaces X.txt [-o [fileX] ] [-g [fileY]] [-c] [-b]\n \n\
This program computes the free faces of a cubical set (or \n\
reprensatable set if the representable set is closed).  The \n\
program reads set from input file, performs computations and \n\
outputs the results to the screen ('cout'). The answer will be \n\
by default in a representable set. \n\n\
Parameters:\n\
-o [file] - create a output file //default file is freeX.txt\n\
-g [file] - create a bmp file of a 1D or 2D set //default file is freeX.bmp\n\
-c	  - show the result in a cubical set \n\
-b	  - results will not be printed on screen \n\
\n\
For help for the input files, look at the exemples or type: cubtop\n\
\n\
For more details ask the authors: \n\
berus00@dmi.usherb.ca or anik.trahan@dmi.usherb.ca.\n \n";

const char *help5 = 
"Help of the function properfaces \n\n\
Call: cubtop properfaces X.txt [-o [fileX] ] [-g [fileY]] [-c] [-b]\n \n\
This program computes the proper faces of a cubical set (or \n\
reprensatable set if the representable set is closed).  The \n\
program reads set from input file, performs computations and \n\
outputs the results to the screen ('cout'). The answer will be \n\
by default in a representable set. \n\n\
Parameters:\n\
-o [file] - create a output file //default file is propX.txt\n\
-g [file] - create a bmp file of a 1D or 2D set //default file is propX.bmp\n\
-c	  - show the result in a cubical set \n\
-b	  - results will not be printed on screen \n\
\n\
For help for the input files, look at the exemples or type: cubtop\n\
\n\
For more details ask the authors: \n\
berus00@dmi.usherb.ca or anik.trahan@dmi.usherb.ca.\n \n";

const char *help6 = 
"Help of the function maximalfaces \n\n\
Call: cubtop maximalfaces X.txt [-o [fileX] ] [-g [fileY]] [-c] [-b]\n \n\
This program computes the maximal faces of a cubical set (or \n\
reprensatable set if the representable set is closed).  The \n\
program reads set from input file, performs computations and \n\
outputs the results to the screen ('cout'). The answer will be \n\
by default in a representable set. \n\n\
Parameters:\n\
-o [file] - create a output file //default file is maxiX.txt\n\
-g [file] - create a bmp file of a 1D or 2D set //default file is maxiX.bmp\n\
-c	  - show the result in a cubical set \n\
-b	  - results will not be printed on screen \n\
\n\
For help for the input files, look at the exemples or type: cubtop\n\
\n\
For more details ask the authors: \n\
berus00@dmi.usherb.ca or anik.trahan@dmi.usherb.ca.\n \n";

const char *help7 = 
"Help of the function topobounds \n\n\
Call: cubtop topobounds X.txt [-o [fileX] ] [-g [fileY]] [-c] [-b]\n \n\
This program computes the topologic bounds of a cubical set (or \n\
reprensatable set if the representable set is closed).  The \n\
program reads set from input file, performs computations and \n\
outputs the results to the screen ('cout'). The answer will be \n\
by default in a representable set. \n\n\
Parameters:\n\
-o [file] - create a output file //default file is topoX.txt\n\
-g [file] - create a bmp file of a 1D or 2D set //default file is topoX.bmp\n\
-c	  - show the result in a cubical set \n\
-b	  - results will not be printed on screen \n\
\n\
For help for the input files, look at the exemples or type: cubtop\n\
\n\
For more details ask the authors: \n\
berus00@dmi.usherb.ca or anik.trahan@dmi.usherb.ca.\n \n";

const char *help8 = 
"Help of the function getdimension \n\n\
Call: cubtop getdim #dim X.txt [-o [fileX] ] [-g [fileY]] [-c] [-b]\n \n\
This program get the elements of the representable set of a \n\
specific dimension.  If the function is called with a cubical\n\
set with the option -c, the program returns the closed of the\n\
elements for this dimension.  \n\n\
Parameters:\n\
-o [file] - create a output file //default file is getd#dimX.txt\n\
-g [file] - create a bmp file of a 1D or 2D set //default file is getd#dimX.bmp\n\
-c	  - show the result in a cubical set \n\
-b	  - results will not be printed on screen \n\
\n\
For help for the input files, look at the exemples or type: cubtop\n\
\n\
For more details ask the authors: \n\
berus00@dmi.usherb.ca or anik.trahan@dmi.usherb.ca.\n \n";

const char *help9 = 
"Help of the function openhull \n\n\
Call: cubtop openhull X.txt [-o [fileX] ] [-g [fileY]] [-c] [-b]\n \n\
This program computes the open hull of a representable set or \n\
a cubical.  The program reads set from input file, performs \n\
computations and outputs the results to the screen ('cout'). \n\
The answer will be in a representable set. \n\n\
Parameters:\n\
-o [file] - create a output file //default file is openX.txt\n\
-g [file] - create a bmp file of a 1D or 2D set //default file is openX.bmp\n\
-b	  - results will not be printed on screen \n\
\n\
For help for the input files, look at the exemples or type: cubtop\n\
\n\
For more details ask the authors: \n\
berus00@dmi.usherb.ca or anik.trahan@dmi.usherb.ca.\n \n";

const char *help10 = 
"Help of the function closedhull \n\n\
Call: cubtop closedhull X.txt [-o [fileX] ] [-g [fileY]] [-c] [-b]\n \n\
This program computes the closed hull of a representable set \n\
or a cubical.  The program reads set from input file, performs \n\
computations and outputs the results to the screen ('cout'). \n\
The answer will be by default in a representable set. \n\n\
Parameters:\n\
-o [file] - create a output file //default file is closX.txt\n\
-g [file] - create a bmp file of a 1D or 2D set //default file is closX.bmp\n\
-c	  - show the result in a cubical set \n\
-b	  - results will not be printed on screen \n\
\n\
For help for the input files, look at the exemples or type: cubtop\n\
\n\
For more details ask the authors: \n\
berus00@dmi.usherb.ca or anik.trahan@dmi.usherb.ca.\n \n";
// assume default fatal error handler 
void (*fatalerrorhandler) (const char *) = NULL; 

int main (int argc, char *argv []) 
// Returns: 0 = Ok, -1 = Error, 1 = Help or example etc. displayed. 
{ 
	int showhelp = 0; 
	char *function = NULL, *func = NULL, *cxfilename = NULL, *cyfilename = NULL, *otherfilename = NULL; 
	int nofunction = 0;
	int nbfiles = 0;
	int dimk =-1;
	char chardimk[4];
	char *snamebitmap = NULL;
	char outputbitmap[25];
	char *outputfile = NULL;
	char outputtxt[25];
	bool graph = false;
	bool nonamegraph = true;
	bool nonamefile = true;
 	bool outfile = false;
	int outcub = 0;
	bool printscreen = true;
	//cout<<"Bonjour"<<endl;
	// interprete the command-line parameters 
	for (int i = 1; i < argc; i ++) 
		if (argv [i] [0] == '-') 
		switch (argv [i] [1]) 
		{ 
			case 'o': 
			case 'O': 
				outfile=true;
				if (i+1 < argc && argv [i+1] [0] != '-') {
					i++;
					outputfile = argv [i]; 
					nonamefile = false;
					}
				break; 
			case 'g': 
			case 'G':
				graph = true;
				if (i+1 < argc && argv [i+1] [0] != '-') {
					i++;
					snamebitmap = argv [i]; 
					nonamegraph = false;
					}
				break; 
			case 'c': 
			case 'C':
				outcub = 1;
				break; 
			case 'b': 
			case 'B':
				printscreen = false;
				break; 
		/*	case 'e': 
			case 'E': 
				i = atoi (argv [i] + 2) - 1; 
				if ((i < 0) || (i >= 12)) 
				{ 
					cout << "Available example " << 
						"numbers are: 1 - "; 
					cout << 12 << "." << 
						endl; 
				} 
				else 
					cout << "examples [i]"; 
				return 1; 
		*/
		}
		
		
		//Attention si nofunction ==0 alors cxfilename = (null) et function = "nom du fichier" 
		else 
			if (!function && !hasadot(argv[i])) {
				function = argv[i];
				strncpy (func, argv [i], 4);
				func[4]= '\0';
				if (strcmp(func, "UNIO") ==0 || strcmp(func, "unio") ==0 || strcmp(func, "Unio")==0){
					nofunction = 1;
					nbfiles = 2;
					}
				else if (strcmp(func, "INTE")  ==0|| strcmp(func, "inte") ==0 || strcmp(func, "Inte") ==0){
					nofunction = 2;
					nbfiles = 2;
					}
				else if (strcmp(func, "DISJ") ==0 || strcmp(func, "disj")  ==0|| strcmp(func, "Disj") ==0){
					nofunction = 3;
					nbfiles = 2;
					}
				else if (strcmp(func, "FREE")  ==0|| strcmp(func, "free")  ==0|| strcmp(func, "Free") ==0){
					nofunction = 4;
					nbfiles = 1;
					}
				else if (strcmp(func, "PROP")  ==0|| strcmp(func, "prop") ==0 || strcmp(func, "Prop") ==0){
					nofunction = 5;
					nbfiles = 1;
					}
				else if (strcmp(func, "MAXI")  ==0|| strcmp(func, "maxi")  ==0|| strcmp(func, "Maxi") ==0){
					nofunction = 6;
					nbfiles = 1;
					}
				else if (strcmp(func, "TOPO")  ==0|| strcmp(func, "topo")  ==0|| strcmp(func, "Topo") ==0){
					nofunction = 7;
					nbfiles = 1;
					}
				else if (strcmp(func, "GETD")  ==0|| strcmp(func, "getd")  ==0|| strcmp(func, "Getd") ==0){
					nofunction = 8;
					nbfiles = 1;
					i++;
					dimk = atoi (argv[i]);
					if (dimk==-1){
						return 8;
						}
					else {
						strcpy(chardimk, argv[i]);
						}
					
					}
				else if (strcmp(func, "OPEN")  ==0|| strcmp(func, "open")  ==0|| strcmp(func, "Open") ==0){
					nofunction = 9;
					nbfiles = 1;
					}
				else if (strcmp(func, "CLOS")  ==0|| strcmp(func, "clos")  ==0|| strcmp(func, "Clos") ==0){
					nofunction = 10;
					nbfiles = 1;
					}
				else {
					
					nbfiles = 1;
					
					}
				}
			else if (hasadot(argv [i])) {
				cxfilename = argv [i];
				cout<<cxfilename<<" et je suis heureux"<<endl;
				}
			else if (!cyfilename && nbfiles == 2) {
				cyfilename = argv [i];
				//cout<<cyfilename<<" = CY"<<endl; 
				}
			else if (!otherfilename) {
				otherfilename = argv [i]; 
				cout<<endl<<otherfilename<<"ERROR :: too many arguments"<<endl<<endl;
				showhelp = 1;
				}
	// show the program's title 
	
	cout << title<<endl<<endl;
	
	if (!cyfilename && nbfiles == 2) {
		cout<<endl<<"ERROR:: Not enought arguments"<<endl<<endl;
		showhelp = 1; 
		}
 
	// show help information if required 
	if (!function && !cxfilename ) {
		showhelp = 1; 
		}
		
	if (nofunction && !cxfilename ) {
		if (nofunction ==1){	
			cout <<help1;
			}
		else if (nofunction == 2){
			cout <<help2;	
			}
		else if (nofunction ==3){
			cout <<help3;	
			}
		else if (nofunction ==4){
			cout <<help4;	
			}
		else if (nofunction ==5){
			cout <<help5;	
			}
		else if (nofunction ==6){
			cout <<help6;	
			}
		else if (nofunction ==7){
			cout <<help7;	
			}
		else if (nofunction ==8){
			cout <<help8;	
			}
		else if (nofunction ==9){
			cout <<help9;	
			}
		else if (nofunction ==10){
			cout <<help10;	
			}
			
			return nofunction;
		}
	
	if (showhelp) { 
		cout << help0; 
		return 1; 
		} 
	// start measuring time of the program's work 
	time_t start_t; 
	time (&start_t); 
	// check if there was any input file name defined 

// Lecture des fichiers	

	PTKrepresentableset cx;
	ifstream cxfilein(cxfilename,ios::in);
	cx.read( cxfilein );
	cout<<"Load "<<cxfilename<<endl;
	
	
	PTKrepresentableset cy;
	if (nbfiles ==2){
		ifstream cyfilein( cyfilename,ios::in );
		cy.read( cyfilein );
		cout<<"Load "<<cxfilename<<endl;
		}
		
// Execute la fonction
	PTKrepresentableset result;
	if (nofunction == 1) {
		result = cx.set_union( cy ) ;
		}
	else if (nofunction == 2) {
		result = cx.set_intersection( cy );
		}
	else if (nofunction == 3) {
		result = cx.set_disjointunion( cy );
		}
	else if (nofunction == 4) {
		result = cx.get_freefaces() ;
		}
	else if (nofunction == 5) {
		result = cx.get_properfaces() ;
		}
	else if (nofunction == 6) {
		result = cx.get_maximalfaces();
		}
	else if (nofunction == 7) {
		result = cx.get_topologicalbound();
		}
	else if (nofunction == 8) {
		result = cx.get_elemcelldimk(dimk);
		if (cx==cx.closedhull() && outcub) {
			result = result.closedhull();
			}
		}
	else if (nofunction == 9) {
		result = cx.openhull();
		}
	else if (nofunction == 10) {
		result = cx.closedhull();
		}
	else {
		result=cx;
		}
	//cout << result << endl;	


// crée le graphe"<<endl;
	if (graph)
	{	
		
		if (nonamegraph) {
			if (nofunction!=0){
				strcpy(outputbitmap, func);
				if (nofunction ==8) { //getd
					strcat(outputbitmap, chardimk);
					}
				strcat(outputbitmap, cxfilename);
				if (nbfiles ==2) {
					minusdot(outputbitmap);
					strcat(outputbitmap,"_");
					strcat(outputbitmap,cyfilename);
				}
			}
			else {
				strcpy(outputbitmap, cxfilename);
				}
			minusdot(outputbitmap);
			strcat(outputbitmap,".bmp");
		
				
		}
		else {
			minusdot(snamebitmap);
			strcat(snamebitmap,".bmp");
			strcpy (outputbitmap, snamebitmap);
		}
		PTKbitmap bitmap_repset( result.get_bitmap() );
		ofstream sortiebmp_union (outputbitmap,ios::out);
		sortiebmp_union << bitmap_repset;
		cout<<"Save file "<<outputbitmap<<endl;
 	}
	
// Crée le fichier de sortie"<<endl;
	if (outfile)
	{
		if (nonamefile ) {
			if (nofunction){
				strcpy(outputtxt, func);
				if (nofunction ==8) { //getd
					strcat(outputtxt, chardimk);
					}
				strcat(outputtxt, cxfilename);
				if (nbfiles ==2) {
					minusdot(outputtxt);
					strcat(outputtxt,"_");
					strcat(outputtxt,cyfilename);
					}
				minusblank(outputtxt);
				}
			else {
				strcpy(outputtxt, cxfilename);
				}
		}
		else {
			strcpy (outputtxt, outputfile);
		}
	}	
	ofstream sortie (outputtxt, ios::out);
	if (outfile) {
		result.write (sortie, outcub);
		cout<<"Save file "<<outputtxt<<" as a ";
		if (outcub) {
			cout<<"cubical";
			}
		else {
			cout<<"representable";
			}
		cout<<" set ."<<endl;
		}
		
	
// Affichage"<<endl;
	if (printscreen) {
		cout<<endl;
		result.write (cout, outcub);
		}
// Fin du calcul du temps

	time_t stop_t; 
	time (&stop_t); 
	cout << "(Time used: " << (stop_t - start_t) << " seconds.)" << 
		endl; 
	return 0;
}
