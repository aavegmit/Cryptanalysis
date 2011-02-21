#include <stdio.h>
#include <iostream>

using namespace std ;

// Function declaration goes here
void Keygen(int period)	;		// Generates a keyfile with specified period to
					// be used by the full Vigenere Cipher


void InvKeygen(char *inpF) ;		// Reads the input key file and generates
					// its inverse

void Crypt(char *, char *) ;		// Encrypts (or decrypts) an input ASCII text 
					// file with the keys specified in keyfile. 
					// The output is an ASCII text file encrypted 
					// (or decrypted) using a full Vigenère cipher 
					// (with polyalphabetic substitution).

void Histo(int, int, char *);		// Produce a histogram of ciphertext character 
					// frequencies assuming the ciphertext is produced 
					// by a polyalphabetic substitution cipher with 
					// period equals period. which specifies a one-based 
					// index for the alphabets. The output must be sorted 
					// according to the frequencies of the ciphertext 
					// characters in descending order.

void Solve(int, char *);		// For this command, the output has 3 sections. 
					// The first section is related to the method of 
					// Kasiski. The 2nd section is related to the average 
					// index of coincidence. The 3rd section is related to 
					// the auto-correlation method. The output of each 
					// section with a banner indicating the method that 
					// is being used.
 
