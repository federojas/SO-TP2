#include <libc.h>
#include <syscalls.h>
#include <stdarg.h>
#include <shell.h>
#define BUFF_LEN 100
// #define PRINTF_FLOAT_PRECISION 4

static int buffSize = 0;
char buffer[BUFF_LEN]={0};
static int firstChange=1;

void putChar(char c){
    _syscall(SYS_WRITE_ID, (uint64_t)&c, 1, BLACK, WHITE, 0);
}

void sendUserData(char *userName, int len){
   _syscall(SYS_WRITE_ID, (uint64_t)userName, len+1, BLACK, WHITE, &len);
}

//https://stackoverflow.com/questions/54352400/implementation-of-printf-function
void printf(char *str, ...){
    va_list args;
    int i = 0, j = 0;
    char buff[BUFF_LEN] = {0}, tmp[20];
    char *str_arg;
    va_start(args, str);

    while (str && str[i]) {
        if (str[i] == '%')
        {
            i++;
            switch (str[i])
            {
                case 'c':
                {
                    buff[j] = (char)va_arg(args, int);
                    j++;
                    break;
                }
                case 'd':
                {
                    intToStr(va_arg(args, int),tmp,10);
                    strcpy(&buff[j], tmp);
                    j += strlen(tmp);
                    break;
                }
                case 'x':
                {
                    intToStr(va_arg(args, int),tmp,16);
                    strcpy(&buff[j], tmp);
                    j += strlen(tmp);
                    break;
                }
                case 's':
                {
                    str_arg = (char *)va_arg(args, char *);
                    strcpy(&buff[j], str_arg);
                    j += strlen(str_arg);
                    break;
                }
                // Atencion, printf imprime solo con precision de 4 decimales
                // case 'f':
                // {
                //     floatToStr(va_arg(args, float),tmp,PRINTF_FLOAT_PRECISION);
                //     strcpy(&buff[j], tmp);
                //     j += strlen(tmp);
                //     break;
                // }
                case '%':
                {
                    strcpy(&buff[j], "%");
                    j++;
                    break;
                }
            }
        } else if (str[i] == '\\') {
            i++;
            switch (str[i])
            {
                // case '\\':
                // {
                //     strcpy(&buff[j], "\\");
                //     j++;
                //     break;
                // }
                case 'n':
                {
                    newLine();
                    break;
                }
            }
        }
        else
        {
            buff[j] = str[i];
            j++;
        }
        i++;
    }

    _syscall(SYS_WRITE_ID, (uint64_t)buff, j, BLACK, WHITE, 0);
    va_end(args);
    return ;
}



// //https://stackoverflow.com/questions/26860574/pow-implementation-in-cmath-and-efficient-replacement
// int pow(int x, unsigned int y)
// {
//     if (y == 0)
//         return 1;
//     else if ((y % 2) == 0)
//         return pow (x, y / 2) * pow (x, y / 2);
//     else
//         return x * pow (x, y / 2) * pow (x, y / 2);

// }

// // Reverses a string 'str' of length 'len'
// static void reverseFtoa(char* str, int len)
// {
//     int i = 0, j = len - 1, temp;
//     while (i < j) {
//         temp = str[i];
//         str[i] = str[j];
//         str[j] = temp;
//         i++;
//         j--;
//     }
// }

// // Converts a given integer x to string str[]. 
// // d is the number of digits required in the output. 
// // If d is more than the number of digits in x, 
// // then 0s are added at the beginning.
// static int intToStrFtoa(int x, char str[], int d)
// {
//     int i = 0;
//     while (x) {
//         str[i++] = (x % 10) + '0';
//         x = x / 10;
//     }
  
//     // If number of digits required is more, then
//     // add 0s at the beginning
//     while (i < d)
//         str[i++] = '0';
  
//     reverseFtoa(str, i);
//     str[i] = '\0';
//     return i;
// }

// // https://www.geeksforgeeks.org/convert-floating-point-number-string/
// // Converts a floating-point/double number to a string.
// void floatToStr(float n, char* res, int afterpoint)
// {
//     // Extract integer part
//     int ipart = (int)n;
  
//     // Extract floating part
//     float fpart = n - (float)ipart;
  
//     // convert integer part to string
//     int i = intToStrFtoa(ipart, res, 0);
  
//     // check for display option after point
//     if (afterpoint != 0) {
//         res[i] = '.'; // add dot
  
//         // Get the value of fraction part upto given no.
//         // of points after dot. The third parameter 
//         // is needed to handle cases like 233.007
//         fpart = fpart * pow(10, afterpoint);
  
//         intToStrFtoa((int)fpart, res + i + 1, afterpoint);
//     }
// }

//funcion auxiliar para leer palabras
// static char *strcpyScan(char *destino, const char *fuente)
// {

//     char *aux = destino;

//     while (*fuente != '\0' && *fuente != ' ')
//     {
//         *destino = *fuente;
//         fuente++;
//         destino++;
//     }

//     *destino = '\0';
//     return aux;
// }

// inspirado en https://iq.opengenus.org/how-printf-and-scanf-function-works-in-c-internally/
int scanf(char * str, ...)
{
    buffSize = 0;
    va_list vl;
    int i = 0, j = 0;
    int sizeNum = 0;
 	va_start( vl, str );
    char *str_arg;
    readText();

 	while (str && str[i] && buffer[j])
 	{
 	    if (str[i] == '%') 
 	    {
 	       i++;
 	       switch (str[i]) 
 	       {
 	            case 'c': 
 	            {
	 	            *(char *)va_arg( vl, char* ) = buffer[j];
	 	            j++;
	 	            break;
 	            }
 	            case 'd': 
 	            {
	 	            *(int *)va_arg( vl, int* ) = strToInt(&buffer[j], &sizeNum);
	 	            j+=sizeNum;
	 	            break;
 	            }
                case 's':
                { 
                    str_arg = (char *)va_arg(vl, char *);
                    strcpy(str_arg, &buffer[j]);
                    j += strlen(str_arg);      
                    break;
                }
                
 	        }
 	    } 
 	    else 
 	    {
 	        buffer[j] =str[i];
            j++;
        }
        i++;
    }
    va_end(vl);
    return j;
}


int strlen(const char *s){
    int i;
    for (i = 0; s[i] != 0; i++);
    return i;
}

char getChar(){
    // MODO ESPERA ACTIVA
    // char buff[2]={0};
    // int ret =_syscall(SYS_READ_ID,buff, 2, 0, 0,0);
    // if(ret<=0){
    //     return -1;
    // }
    // return buff[0]; 
    return _syscall(SYS_READ_ID, 0, 0, 0, 0, 0); //MODO HLT
}


//funcion encargada a actualizar el buffer de la shell y de imprimir en pantalla el char en cuestion
int readText(){
    int c;
    while((c=getChar()) !='\n'){
        
        if(c!=-1){
            if(buffSize < BUFF_LEN-1){
                buffer[buffSize++]=c;
            }
            if(c=='\t'){
                if(firstChange){
                    firstChange=0;
                    printUser();    
                }
                buffSize=0;
            }else{
                if(c=='\b'){
                    buffSize-=2;
                }
                putChar(c);
            }
        }
    }
    newLine();
    buffer[buffSize++]='\0';
    return buffSize++;
}

void newLine(){
    putChar('\n');
}



char *strcpy(char *destino, const char *fuente)
{

    char *aux = destino;

    while (*fuente != '\0')
    {
        *destino = *fuente;
        fuente++;
        destino++;
    }

    *destino = '\0';
    return aux;
 }




// http://vivi.dyndns.org/tech/cpp/atohex.html

static int isdigit(char ch) {
    return ch >= '0' && ch <= '9';
}

static int ishexdigit(char ch)
{
        return isdigit(ch) || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f');
}

//https://www.geeksforgeeks.org/write-your-own-atoi/
int strToInt(char* str, int * size)
{
    *size = 0;
    // Initialize result
    int res = 0;
 
    // Initialize sign as positive
    int sign = 1;
 
    // Initialize index of first digit
    int i = 0;
 
    // If number is negative,
    // then update sign
    if (str[0] == '-') {
        sign = -1;
        *size += 1;
        // Also update index of first digit
        i++;
    }
 
    // Iterate through all digits
    // and update the result
    for (; str[i] != '\0'; ++i) {
        if (str[i] < '0' || str[i] > '9')
             return res;
        res = res * 10 + str[i] - '0';
        *size += 1;
    }
        
 
    // Return result with sign
    return sign * res;
}

int strToHex(const char *str)
{
    int val = 0;      
    while( *str != 0 ) {  
        if( !ishexdigit(*str) ) { 
            return -1;
        }
        if( isdigit(*str) ) {    
            val = val * 16 + *str++ - '0';      
        } else if( *str >= 'A' && *str <= 'F' ) {    
            val = val * 16 + *str++ - 'A' + 10;      
        } else {   
            val = val * 16 + *str++ - 'a' + 10;      
        }
    }
    return val;   
}

// static float min(float x, float n) {
//     return n>x ? x : n;
// }

// static float max(float x, float n) {
//     return n>x ? n : x;
// }

// float sqrt(float n){
//   // Max and min are used to take into account numbers less than 1
//   float lo = min(1, n), hi = max(1, n), mid;

//   // Update the bounds to be off the target by a factor of 10
//   while(100 * lo * lo < n) lo *= 10;
//   while(100 * hi * hi > n) hi *= 0.1;

//   for(int i = 0 ; i < 100 ; i++){
//       mid = (lo+hi)/2;
//       if(mid*mid == n) return mid;
//       if(mid*mid > n) hi = mid;
//       else lo = mid;
//   }
//   return mid;
// }

// // https://www.codeproject.com/Articles/570700/SquareplusRootplusalgorithmplusforplusC
// static float powerOfTen(int num){
//     float rst = 1.0;
//     if(num >= 0){
//         for(int i = 0; i < num ; i++){
//             rst *= 10.0;
//         }
//     }else{
//         for(int i = 0; i < (0 - num ); i++){
//             rst *= 0.1;
//         }
//     }
//     return rst;
// }

// float sqrt(float a)
//  {
//     /*
//         find more detail of this method on wiki methods_of_computing_square_roots

//         *** Babylonian method cannot get exact zero but approximately value of the square_root
//     */
//     float z = a; 
//     float rst = 0.0;
//     int max = 8;     // to define maximum digit 
//     int i;
//     float j = 1.0;
//     for(i = max ; i > 0 ; i--){
//         // value must be bigger then 0
//         if(z - (( 2 * rst ) + ( j * powerOfTen(i)))*( j * powerOfTen(i)) >= 0)
//         {
//             while( z - (( 2 * rst ) + ( j * powerOfTen(i)))*( j * powerOfTen(i)) >= 0)
//             {
//                 j++;
//                 if(j >= 10) break;

//             }
//             j--; //correct the extra value by minus one to j
//             z -= (( 2 * rst ) + ( j * powerOfTen(i)))*( j * powerOfTen(i)); //find value of z

//             rst += j * powerOfTen(i);     // find sum of a

//             j = 1.0;
//         }

//     }
 
//     for(i = 0 ; i >= 0 - max ; i--){
//         if(z - (( 2 * rst ) + ( j * powerOfTen(i)))*( j * powerOfTen(i)) >= 0)
//         {
//             while( z - (( 2 * rst ) + ( j * powerOfTen(i)))*( j * powerOfTen(i)) >= 0)
//             {
//             j++;

//         }
//             j--;
//             z -= (( 2 * rst ) + ( j * powerOfTen(i)))*( j * powerOfTen(i)); //find value of z

//             rst += j * powerOfTen(i);     // find sum of a
//             j = 1.0;
//         }
//     }
//     // find the number on each digit
//     return rst;
//  }

// https://www.techiedelight.com/implement-itoa-function-in-c/

int abs(int num){
    return num < 0? -num : num;
}

// function to reverse buffer[i..j]
char *reverse(char *buffer, int i, int j)
{
    while (i < j)
        swap(&buffer[i++], &buffer[j--]);

    return buffer;
}

void swap(char *x, char *y)
{
    char t = *x;
    *x = *y;
    *y = t;
}

// Iterative function to implement itoa() function in C
char* intToStr(int value, char* buffer, int base)
{
    // invalid input
    if (base < 2 || base > 32) {
        return buffer;
    }
 
    // consider the absolute value of the number
    int n = abs(value);
 
    int i = 0;
    while (n)
    {
        int r = n % base;
 
        if (r >= 10) {
            buffer[i++] = 65 + (r - 10);
        }
        else {
            buffer[i++] = 48 + r;
        }
 
        n = n / base;
    }
 
    // if the number is 0
    if (i == 0) {
        buffer[i++] = '0';
    }
 
    // If the base is 10 and the value is negative, the resulting string
    // is preceded with a minus sign (-)
    // With any other base, value is always considered unsigned
    if (value < 0 && base == 10) {
        buffer[i++] = '-';
    }
 
    buffer[i] = '\0'; // null terminate string
 
    // reverse the string and return it
    return reverse(buffer, 0, i - 1);
}

int strcmp(char string1[], char string2[])
{
    int i = 0;   
    while(string1[i]!=0 && string2[i]!=0){
        if(string1[i] < string2[i])
            return -1;
        if(string1[i] > string2[i])
            return 1;
        i++;
    }
    if(string1[i]==0 && string2[i]==0)
        return 0;
    if(string1[i]!=0 && string2[i]==0)
        return 1;
    return -1;
}