#include<stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std ;

void usage(){
	printf("Usage:\n\t ./hw4 keygen -t=period\n") ;
	printf("\t ./hw4 crypt -k=keyfile [file]\n") ;
	printf("\t ./hw4 invkey keyfile\n") ;
	printf("\t ./hw4 histo -t=period -i=which [file]\n") ;
	printf("\t ./hw4 solve -l=max_t file\n") ;
	exit(0) ;
}


// Main function - command line parsing and 
// calling respective functions
int main(int argc, char *argv[]){


	int choice = 0, stdI = 0 ;
	char *period, *inpF ;
	char *inpFile, *inpKeyfile, *max_t, *keyfile, *which ;

	// Parse the command line 
	if (argc < 2){
		usage() ;
	}
	else {
		argv++ ;
		if (strcmp(*argv, "keygen") == 0) {
			choice = 1 ;
		} else if (strcmp(*argv, "crypt") == 0) {
			choice = 2 ;
		} else if (strcmp(*argv, "invkey") == 0) {
			choice = 3 ;
		} else if (strcmp(*argv, "histo") == 0) {
			choice = 4 ;
		} else if (strcmp(*argv, "solve") == 0) {
			choice = 5 ;
		} else {
			usage() ;
		}

		argv++ ;

		// keygen chosen
		if (choice == 1){
			if (argc < 3){
				usage() ;
			}
			for (int i = 0 ; i < 1 ; i++, argv++) {
				if (*argv[0] == '-') {
					if (argv[0][1] == 't') {
						/* read pphrase from *argv */
						period = strtok(*argv, "=") ;
						period = strtok(NULL, "=") ;
						if (period == NULL)
							usage() ;
					} 
					else{
						usage() ;
					}
				}
				else{
					usage() ;	
				}

			}
		}
		else if(choice == 2){
			if (argc < 3){
				usage() ;
			}
				if (*argv[0] == '-') {
					if (argv[0][1] == 'k') {
						/* read pphrase from *argv */
						keyfile = strtok(*argv, "=") ;
						keyfile = strtok(NULL, "=") ;
						if (keyfile == NULL)
							usage() ;
					} 
					else{
						usage() ;
					}
				}
				else{
					// It is the input file
				}
				++argv ;

			if (argc == 4){
				inpF = *argv ;
			}
			else{
				// read input from the stdin
				stdI = 1 ;
				FILE *fop ;
				char c;
				fop = fopen("input.tmp", "wb") ;
				while( !feof(stdin)   )   {
					c = fgetc(stdin) ;
					if (!feof(stdin))
						fputc(c, fop) ;
				}
				char inputFil[10] ;
				strncpy(inputFil, "input.tmp",9) ;
				inputFil[9] = '\0' ;
				inpF = inputFil ;
				fclose(fop) ;	
			}
		}
		// Invkey command line option
		else if(choice == 3){
			if (argc != 3){
				usage() ;
			}
			for (int i = 0 ; i < 1 ; i++, argv++) {
				inpKeyfile = *argv ;

			}
		}
		// Histo command line option
		else if(choice == 4){
			if (argc < 4){
				usage() ;
			}
			for (int i = 0 ; i < 2 ; i++, argv++) {
				if (*argv[0] == '-') {
					if (argv[0][1] == 't') {
						/* read pphrase from *argv */
						period = strtok(*argv, "=") ;
						period = strtok(NULL, "=") ;
						if (period == NULL)
							usage() ;
					} 
					else if (argv[0][1] == 'i') {
						/* read pphrase from *argv */
						which = strtok(*argv, "=") ;
						which = strtok(NULL, "=") ;
						if (which == NULL)
							usage() ;
					} 
					else{
						usage() ;
					}
				}
				else{
					usage() ;	
				}

			}
			if (argc == 5){
				inpFile = *argv ;
			}
			else{
				// Read from stdin
				// read input from the stdin
				stdI = 1 ;
				FILE *fop ;
				char c;
				fop = fopen("input.tmp", "wb") ;
				while( !feof(stdin)   )   {
					c = fgetc(stdin) ;
					if (!feof(stdin))
						fputc(c, fop) ;
				}
				char inputFil[10] ;
				strncpy(inputFil, "input.tmp",9) ;
				inputFil[9] = '\0' ;
				inpFile = inputFil ;
				fclose(fop) ;	
			}
		}
		else if(choice == 5){
			if (argc < 4){
				usage() ;
			}
			for (int i = 0 ; i < 1 ; i++, argv++) {
				if (*argv[0] == '-') {
					if (argv[0][1] == 'l') {
						/* read pphrase from *argv */
						max_t = strtok(*argv, "=") ;
						max_t = strtok(NULL, "=") ;
						if (max_t == NULL)
							usage() ;
					} 
					else{
						usage() ;
					}
				}
				else{
					usage() ;	
				}

			}
			inpFile = *argv ;
		}

	}

	// Command line parsing done, now call respective methods
	if (choice == 1){
		printf("Keygen selected with period: %s\n", period) ;
	}
	else if(choice == 2){
		printf("crypt selected with keyfile: %s and file %s\n", keyfile, inpF) ;
	}
	else if(choice == 3){
		printf("invkey selected with keyfile: %s\n", inpKeyfile) ;
	}
	else if(choice == 4){
		printf("histo selected with period: %s, which %s and file: %s\n", period, which, inpFile) ;
	}
	else if(choice == 5){
		printf("solve selected with l: %s, file: %s\n", max_t, inpFile) ;
	}
	else
		usage() ;





} // end of main function
