#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <math.h>

#define MAX_CPR 1000
#define MIN_YEAR 1858
#define MAX_YEAR 2057


// int verMod11(char *cpr); // Maybe this one.
int sexCheck(int sex, char *cpr);
int genMod11Cipher(char *cpr9ciphers);
uint8_t gen7CipherList(int birthYear, uint8_t *poss7thCipher);
uint16_t generateCprs(char *birthDateAndSex, uint32_t *cprList);
uint8_t verifyInput(char *str);
void input2birthdateAndSex(char *str, char *birthDateAndSex);
uint8_t extractData(char *line, uint8_t);


int main(int argc, char const *argv[]) {
  uint32_t linenumber;
  uint8_t inputError, sex;
  
  uint16_t n_cpr;
  char birthDateAndSex[10];
  char *line;
  size_t lineBufsize = 32;
  size_t len;


  line = (char *)malloc(lineBufsize * sizeof(char));
  if( line == NULL)
  {
      perror("Unable to allocate buffer for line");
      exit(1);
  }
  
  linenumber = 0;

  while((len = getline(&line,&lineBufsize,stdin)) != -1) {
    linenumber++;
    /* Check formatting of input. Should be DDMMYYYYS */
    if(verifyInput(line) != 0) {
      // Todo: Change to an error?
      printf("Input on line %d could not be read. Expected input is on format DDMMYYYYS\n", linenumber);
    }
    
    
    /* Generate possible CPRs */
    n_cpr = generateCprs(line, cprList);
    
    /* Print possible CPRs to stdout */


    /* Print for debugging */
    //printf("%zu characters were read.\n",len);
    //printf("You typed: '%s'\n",buffer);
  }
  
  
  return 0;
}

uint16_t generateCprs(char *dstr, uint32_t *cprList) {
  uint8_t cprVect[10], poss7thCipher[6], len7Cipher;
  uint16_t birthYear, cprCount;
  
  /* Determine sex. 0 for female, 1 for male*/
  if(line[8] == 'f' || line[8] == 'F') {
    sex = 0;
  } else {
    sex = 1;
  }
  
  /* Extract birthyear */
  birthYear = 1000*(dstr[4]-'0') + 100*(dstr[5]-'0') + 10*(dstr[6]-'0') + (dstr[7]-'0');
  
  /* Convert char str to uint8 vect */
  cprVect[0] = dstr[0]-'0';
  cprVect[1] = dstr[1]-'0';
  cprVect[2] = dstr[2]-'0';
  cprVect[3] = dstr[3]-'0';
  cprVect[4] = dstr[5]-'0';
  cprVect[5] = dstr[6]-'0';
  
  len7Cipher = gen7CipherList(birthYear, *poss7thCipher);
  
  cprCount = 0;
  for(uint8_t i = 0; i < len7Cipher; i++) {
    cprVect[6] = poss7thCipher[i];
    for(uint8_t j = 0; j < 10; j++) {
      cprVect[7] = j;
      for(uint8_t k = 0; k < 10; k++) {
        cprVect[8] = k;
        cprVect[9] = genMod11Cipher(cprVect);
        if(cprVect[9] != -1 && sexCheck(cprVect[9])) {
          cprList[cprCount] = cprVect2uint32(cprVect);
        }
      }
    }
  }
}

uint32_t cprVect2uint32(uint8_t *cprVect) {
  uint32_t sum = 0;
  for(uint8_t i = 0; i < 10; i++) {
    sum += cprVect[i]*pow(10,10-i); // TODO : Continue here!
  }
}

uint8_t gen7CipherList(uint16_t birthYear, uint8_t *poss7thCipher) {
  if(birthYear >= MIN_YEAR && birthYear < 1900){
    poss7thCipher[0]=5;
    poss7thCipher[1]=6;
    poss7thCipher[2]=7;
    poss7thCipher[3]=8;
    return 4;
  } else if(birthYear >= 1900 && birthYear < 1937){
    poss7thCipher[0]=0;
    poss7thCipher[1]=1;
    poss7thCipher[2]=2;
    poss7thCipher[3]=3;
    return 4;
  } else if(birthYear >= 1937 && birthYear < 2000){
    poss7thCipher[0]=0;
    poss7thCipher[1]=1;
    poss7thCipher[2]=2;
    poss7thCipher[3]=3;
    poss7thCipher[4]=4;
    poss7thCipher[5]=9;
    return 6;
  } else if(birthYear >= 2000 && birthYear < 2037){
    poss7thCipher[0]=4;
    poss7thCipher[1]=5;
    poss7thCipher[2]=6;
    poss7thCipher[3]=7;
    poss7thCipher[4]=8;
    poss7thCipher[5]=9;
    return 6;
  } else if(birthYear >= 2037 && birthYear < MAX_YEAR){
    poss7thCipher[0]=5;
    poss7thCipher[1]=6;
    poss7thCipher[2]=7;
    poss7thCipher[3]=8;
    return 4;
  } else {
    // Unreachable
    return 0;
  }
}

uint8_t verifyInput(char *str) {
  uint8_t day, month, i;
  uint16_t year;
  for(i = 0; i < 8 && isdigit(str[i]); i++) {
    // No action
  }
  if(i != 8) {
    return 1;
  }
  if( !(str[i] == 'm' || str[i] == 'M' || str[i] == 'f' || str[i] == 'F')) {
    return 2;
  }
  /* Convert to numbers */
  day = 10*(str[0]-'0') + (str[1]-'0');
  month = 10*(str[2]-'0') + (str[3]-'0');
  year = 1000*(str[4]-'0') + 100*(str[5]-'0') + 10*(str[6]-'0') + (str[7]-'0');

  /* Check if range is reasonable */
  if(day > 31 || month > 12 || year < MIN_YEAR || year >= MAX_YEAR) {
    return 3;
  }
  
  return 0;
  
}
