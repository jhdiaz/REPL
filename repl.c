#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void repl();
int eval(char* string);
int evalCommand(char* string);
char* parseLine(char* string);
char* nextWord(char* string);

int main(){
	repl();
	return 0;
}

/***********************************************\
**----------repl by Jeffrey H. Diaz-------------**
************-----------------------***************
**************************************************
***********************************************
****************************************
*********************************
****************************
*********************
****************
**********
******
***
*/

void repl(){
	int stack_max = 4096;
	int nameValueStack_max = 4096;
	char **stack = malloc(sizeof(char*)*stack_max);
	*stack = "";
	char **nameStack = malloc(sizeof(char*)*nameValueStack_max);
	char **valueStack = malloc(sizeof(char*)*nameValueStack_max);
	int top = 0;
	int nameValueTop = 0; //Top of both name and value stack points to empty position after top values, not to top values.
	printf("repl> ");
	int line_max = 4096;
	int loop = 1;
	int mode = 0; //0: reading from stdin, 1: reading from file, 2: reading from closure
	FILE* fPointer;
	char* closure;

	while(loop==1){
		char *line = malloc(sizeof(char)*line_max);
		int endOfFile = 0;

		if(mode == 0){
			fgets(line, line_max, stdin);
		}
		else if(mode == 2){
			strncpy(line, closure, strlen(closure));
			line[strlen(line)] = '\n';
			mode = 0;
		}
		else{
			if(fgets(line, line_max, fPointer)==NULL){
				endOfFile = 1;
				mode = 0;
				*line = '\n';
				top++;
				stack[top] = ":true:";
			}
		}

		//Check for buffer overflow
		int z = 0;
		while(z<=line_max-2){
			if(line[z]=='\n'){
				break;
			}
			if(z==line_max-2){
				line_max *= 2;
				char *temp = realloc(line, sizeof(char)*line_max);
				line = temp;
				break;
			}
			z++;
		}

		char* next = line;

		while(next != NULL){
			//Resizing stack if full
			if(stack[stack_max-1]!=NULL){
				stack_max *= 2;
				char **temp = realloc(stack, sizeof(char*)*stack_max);
				stack = temp;
			}

			if(nameStack[nameValueStack_max-1]!=NULL || valueStack[nameValueStack_max-1]!=NULL){
				nameValueStack_max *= 2;

				char **temp = realloc(nameStack, sizeof(char*)*nameValueStack_max);
				nameStack = temp;

				char **temp2 = realloc(valueStack, sizeof(char*)*nameValueStack_max);
				valueStack = temp2;
			}

			//Parsing current word out of line.
			char* temp = (char*)malloc(sizeof(char)*line_max);
			temp = parseLine(next);
			char* word = (char*)malloc(sizeof(char)*(strlen(temp)));
			strncpy(word, temp, strlen(temp));

			//Removing any leading white spaces
        		while(*next == ' ' || *next == '\t'){
				next++;
		        }

			//Pointer to next word in line. NULL if this is the last word.
			next = nextWord(next);

			//Removing newline character
			if(word[strlen(word)-1] == '\n'){
				if(*word != '\n'){
					word[strlen(word)-1] = '\0';
				}
			}

			if(strcmp("quit", word) != 0){
				int x = eval(word);

				if(x==0){ //Positive number
					if(stack[top]==NULL || stack[top]==""){
						if(atoi(word)==0){
							stack[top] = "0";
						}
						else{
							stack[top] = word;
						}
					}
					else{
						top++;
						if(atoi(word)==0){
							stack[top] = "0";
						}
						else{
							stack[top] = word;
						}
					}
				}
				else if(x==1){ // Negative number
                                        if(stack[top]==NULL || stack[top]==""){
						if(atoi(word)==0){
							stack[top] = "0";
						}
						else{
                                                	stack[top] = word;
						}
                                        }
                                        else{
						top++;
						if(atoi(word)==0){
							stack[top] = "0";
						}
                                                else{
                                                	stack[top] = word;
						}
                                        }
				}
				else if(x==2){//bool true
                                        if(stack[top]==NULL || stack[top]==""){
                                                stack[top] = ":true:";
                                        }
                                        else{
                                                top++;
                                                stack[top] = ":true:";
                                        }
				}
				else if(x==3){//bool false
                                        if(stack[top]==NULL || stack[top]==""){
                                                stack[top] = ":false:";
                                        }
                                        else{
                                                top++;
                                                stack[top] = ":false:";
                                        }
				}
				else if(x==4){//command
					int y = evalCommand(word);
					if(y==0){
						// add()
                                                if(stack[top]!="" && stack[top]!=NULL && top>=1){
                                                        if(eval(stack[top])<=1 && eval(stack[top-1])<=1){
                                                                char* temp1 = malloc(sizeof(char)*strlen(stack[top]));
                                                                temp1 = stack[top];
                                                                stack[top] = "";
                                                                top--;
                                                                char* temp2 = malloc(sizeof(char)*strlen(stack[top]));
                                                                temp2 = stack[top];
								//convert temp1 and temp2, then add them and convert back
								int b = atoi(temp1);
								int a = atoi(temp2);
								int sum = a + b;
								char* str = malloc(sizeof(char)*line_max);
								snprintf(str, line_max, "%d", sum);
                                                                stack[top] = str;
                                                        }
                                                        else{
                                                                if(stack[top]==NULL || stack[top]==""){
                                                                        stack[top] = ":error:";
                                                                }
                                                                else{
                                                                        top++;
                                                                        stack[top] = ":error:";
                                                                }
                                                        }
                                                }
                                                else{
                                                        if(stack[top]==NULL || stack[top]==""){
                                                                stack[top] = ":error:";
                                                        }
                                                        else{
                                                                top++;
                                                                stack[top] = ":error:";
                                                        }
                                                }

					}
					else if(y==1){
						//sub()
                                                if(stack[top]!="" && stack[top]!=NULL && top>=1){
                                                        if(eval(stack[top])<=1 && eval(stack[top-1])<=1){
                                                                char* temp1 = malloc(sizeof(char)*strlen(stack[top]));
                                                                temp1 = stack[top];
                                                                stack[top] = "";
                                                                top--;
                                                                char* temp2 = malloc(sizeof(char)*strlen(stack[top]));
                                                                temp2 = stack[top];
                                                                int b = atoi(temp1);
                                                                int a = atoi(temp2);
                                                                int dif = a - b;
                                                                char* str = malloc(sizeof(char)*line_max);
                                                                snprintf(str, line_max, "%d", dif);
                                                                stack[top] = str;
                                                        }
                                                        else{
                                                                if(stack[top]==NULL || stack[top]==""){
                                                                        stack[top] = ":error:";
                                                                }
                                                                else{
                                                                        top++;
                                                                        stack[top] = ":error:";
                                                                }
                                                        }
                                                }
                                                else{
                                                        if(stack[top]==NULL || stack[top]==""){
                                                                stack[top] = ":error:";
                                                        }
                                                        else{
                                                                top++;
                                                                stack[top] = ":error:";
                                                        }
                                                }

					}
					else if(y==2){
						// mul()
                                                if(stack[top]!="" && stack[top]!=NULL && top>=1){
                                                        if(eval(stack[top])<=1 && eval(stack[top-1])<=1){
                                                                char* temp1 = malloc(sizeof(char)*strlen(stack[top]));
                                                                temp1 = stack[top];
                                                                stack[top] = "";
                                                                top--;
                                                                char* temp2 = malloc(sizeof(char)*strlen(stack[top]));
                                                                temp2 = stack[top];
                                                                int b = atoi(temp1);
                                                                int a = atoi(temp2);
                                                                int prod = a * b;
                                                                char* str = malloc(sizeof(char)*line_max);
                                                                snprintf(str, line_max, "%d", prod);
                                                                stack[top] = str;
                                                        }
                                                        else{
                                                                if(stack[top]==NULL || stack[top]==""){
                                                                        stack[top] = ":error:";
                                                                }
                                                                else{
                                                                        top++;
                                                                        stack[top] = ":error:";
                                                                }
                                                        }
                                                }
                                                else{
                                                        if(stack[top]==NULL || stack[top]==""){
                                                                stack[top] = ":error:";
                                                        }
                                                        else{
                                                                top++;
                                                                stack[top] = ":error:";
                                                        }
                                                }

					}
					else if(y==3){
						// div()
                                                if(stack[top]!="" && stack[top]!=NULL && top>=1){
                                                        if(eval(stack[top])<=1 && eval(stack[top-1])<=1){
                                                                char* temp1 = malloc(sizeof(char)*strlen(stack[top]));
                                                                temp1 = stack[top];
                                                                stack[top] = "";
                                                                top--;
                                                                char* temp2 = malloc(sizeof(char)*strlen(stack[top]));
                                                                temp2 = stack[top];
                                                                int b = atoi(temp1);
								if(b==0){
									stack[top] = temp2;
									top++;
									stack[top] = temp1;
									top++;
									stack[top] = ":error:";
								}
								else{
	                                                                int a = atoi(temp2);
	                                                                int quotient = 0;
									if(a<0){
										int x = -1;
										if(b<0){
											x = 1;
										}
										quotient = (a/b) + x;
									}
									else{
										quotient = a/b;
									}
	                                                                char* str = malloc(sizeof(char)*line_max);
	                                                                snprintf(str, line_max, "%d", quotient);
	                                                                stack[top] = str;
								}

                                                        }
                                                        else{
                                                                if(stack[top]==NULL || stack[top]==""){
                                                                        stack[top] = ":error:";
                                                                }
                                                                else{
                                                                        top++;
                                                                        stack[top] = ":error:";
                                                                }
                                                        }
                                                }
                                                else{
                                                        if(stack[top]==NULL || stack[top]==""){
                                                                stack[top] = ":error:";
                                                        }
                                                        else{
                                                                top++;
                                                                stack[top] = ":error:";
                                                        }
                                                }

					}
					else if(y==4){
						// rem()
                                                if(stack[top]!="" && stack[top]!=NULL && top>=1){
                                                        if(eval(stack[top])<=1 && eval(stack[top-1])<=1){
                                                                char* temp1 = malloc(sizeof(char)*strlen(stack[top]));
                                                                temp1 = stack[top];
                                                                stack[top] = "";
                                                                top--;
                                                                char* temp2 = malloc(sizeof(char)*strlen(stack[top]));
                                                                temp2 = stack[top];
                                                                int b = atoi(temp1);
								if(b==0){
									stack[top] = temp2;
									top++;
									stack[top] = temp1;
									top++;
									stack[top] = ":error:";
								}
								else{
	                                                                int a = atoi(temp2);
									int quotient = 0;

									if(a<0){
                                                                                int x = -1;
                                                                                if(b<0){
                                                                                        x = 1;
                                                                                }
                                                                                quotient = (a/b) + x;
                                                                        }
                                                                        else{
                                                                                quotient = a/b;
                                                                        }

	                                                                int rem = a-b*quotient;
	                                                                char* str = malloc(sizeof(char)*line_max);
	                                                                snprintf(str, line_max, "%d", rem);
	                                                                stack[top] = str;
								}
                                                        }
                                                        else{
                                                                if(stack[top]==NULL || stack[top]==""){
                                                                        stack[top] = ":error:";
                                                                }
                                                                else{
                                                                        top++;
                                                                        stack[top] = ":error:";
                                                                }
                                                        }
                                                }
                                                else{
                                                        if(stack[top]==NULL || stack[top]==""){
                                                                stack[top] = ":error:";
                                                        }
                                                        else{
                                                                top++;
                                                                stack[top] = ":error:";
                                                        }
                                                }
					}
					else if(y==5){
						// neg()
						if(stack[top]!="" && stack[top]!=NULL){
							if(eval(stack[top])<=1){
								char *temp = malloc(sizeof(char)*strlen(stack[top]));
								int a = atoi(stack[top]);
								a = a*-1;
								snprintf(temp, line_max, "%d", a);
								stack[top] = temp;
							}
							else{
								top++;
								stack[top] = ":error:";
							}
						}
						else{
							stack[top] = ":error:";
						}
					}
					else if(y==6){
						// pop()
						if(stack[top]!="" && stack[top]!=NULL){
							stack[top] = "";
							if(top>0){
								top--;
							}
						}
						else{
							stack[top] = ":error:";
						}
					}
					else if(y==7){
						// exc()
						if(stack[top]!="" && stack[top]!=NULL && top>=1){
							char* temp1 = malloc(sizeof(char)*strlen(stack[top]));
							temp1 = stack[top];
							char* temp2 = malloc(sizeof(char)*strlen(stack[top-1]));
							temp2 = stack[top-1];
							stack[top-1] = temp1;
							stack[top] = temp2;
						}
						else{
							if(stack[top]==NULL || stack[top]==""){
								stack[top] = ":error:";
							}
							else{
								top++;
								stack[top] = ":error:";
							}
						}
						
					}
					else if(y==8){//and
						if(stack[top]!="" && stack[top]!=NULL && top>=1){
							if( (eval(stack[top]) == 2 || eval(stack[top]) == 3) //If elements are bool 
							&& (eval(stack[top-1]) == 2 || eval(stack[top-1]) ==3)){
								if(stack[top] == ":true:" && stack[top-1] == ":true:"){
									stack[top] = "";
									top--;
									stack[top] = ":true:";
								}
								else{
									stack[top] = "";
									top--;
									stack[top] = ":false:";
								}
							}
							else{
								top++;
								stack[top] = ":error:";
							}
						}
						else{
							if(stack[top]==NULL || stack[top]==""){
								stack[top] = ":error:";
							}
							else{
								top++;
								stack[top] = ":error:";
							}
						}
					}
					else if(y==9){//or
						if(stack[top]!="" && stack[top]!=NULL && top>=1){
                                                        if( (eval(stack[top]) == 2 || eval(stack[top]) == 3) //If elements are bool 
                                                        && (eval(stack[top-1]) == 2 || eval(stack[top-1]) ==3)){
                                                                if(stack[top] == ":true:" || stack[top-1] == ":true:"){
                                                                        stack[top] = "";
                                                                        top--;
                                                                        stack[top] = ":true:";
                                                                }
                                                                else{
                                                                        stack[top] = "";
                                                                        top--;
                                                                        stack[top] = ":false:";
                                                                }
                                                        }
                                                        else{
                                                                top++;
                                                                stack[top] = ":error:";
                                                        }
                                                }
						else{
                                                        if(stack[top]==NULL || stack[top]==""){
                                                                stack[top] = ":error:";
                                                        }
                                                        else{
                                                                top++;
                                                                stack[top] = ":error:";
                                                        }
                                                }
					}
					else if(y==10){//not
						if(stack[top]!="" && stack[top]!=NULL){
							if(eval(stack[top])==2){ //If :true:
								stack[top] = ":false:";
							}
							else if(eval(stack[top])==3){ //If :false:
								stack[top] = ":true:";
							}
							else{
								top++;
								stack[top] = ":error:";
							}
						}
						else{
							stack[top] = ":error:";
						}
					}
					else if(y==11){//equal
						if(stack[top]!="" && stack[top]!=NULL && top>=1){
							//Variables to decide whether the two element are of the same type
							int type1 = eval(stack[top]);
							int type2 = eval(stack[top-1]);

							//If either element is an error :error: is pushed
							if(type1==8 || type2==8){
								top++;
								stack[top] = ":error:";
							}
							//If either element is a closure :false: is pushed
							else if(type1==7 || type2==7){
								top--;
								stack[top] = ":false:";
							}
							//If both elements are numbers calculate whether they are equal
                                                        else if(type1<=1 && type2<=1){
                                                                char* temp1 = malloc(sizeof(char)*strlen(stack[top]));
                                                                temp1 = stack[top];
                                                                stack[top] = "";
                                                                top--;
                                                                char* temp2 = malloc(sizeof(char)*strlen(stack[top]));
                                                                temp2 = stack[top];
                                                                int b = atoi(temp1);
                                                                int a = atoi(temp2);
								if(a==b){
									stack[top] = ":true:";
								}
								else{
									stack[top] = ":false:";
								}
                                                        }
							//If both elements are different types they are not equal
							else if(type1 != type2){
								top--;
								stack[top] = ":false:";
							}
							//If both elements are strings or names compare them to see if they are equal
							else if((type1==5 && type2==5) || (type1==6 && type2==6)){
								if(strcmp(stack[top], stack[top-1])==0){
									top--;
									stack[top] = ":true:";
								}
								else{
									top--;
									stack[top] = ":false:";
								}
							}
							//If both are either both are true or false :true: is pushed on
							else if((type1==2 && type2==2) || (type1==3 && type2==3)){
								top--;
								stack[top] = ":true:";
							}
                                                        else{
                                                                if(stack[top]==NULL || stack[top]==""){
                                                                        stack[top] = ":error:";
                                                                }
                                                                else{
                                                                        top++;
                                                                        stack[top] = ":error:";
                                                                }
                                                        }
						}
						else{
                                                        if(stack[top]==NULL || stack[top]==""){
                                                                stack[top] = ":error:";
                                                        }
                                                        else{
                                                                top++;
                                                                stack[top] = ":error:";
                                                        }
                                                }
					}
					else if(y==12){//lessThan
                                                if(stack[top]!="" && stack[top]!=NULL && top>=1){
                                                        if(eval(stack[top])<=1 && eval(stack[top-1])<=1){
                                                                char* temp1 = malloc(sizeof(char)*strlen(stack[top]));
                                                                temp1 = stack[top];
                                                                stack[top] = "";
                                                                top--;
                                                                char* temp2 = malloc(sizeof(char)*strlen(stack[top]));
                                                                temp2 = stack[top];
                                                                int b = atoi(temp1);
                                                                int a = atoi(temp2);
								if(a<b){
									stack[top] = ":true:";
								}
								else{
									stack[top] = ":false:";
								}
                                                        }
                                                        else{
                                                                if(stack[top]==NULL || stack[top]==""){
                                                                        stack[top] = ":error:";
                                                                }
                                                                else{
                                                                        top++;
                                                                        stack[top] = ":error:";
                                                                }
                                                        }
                                                }
                                                else{
                                                        if(stack[top]==NULL || stack[top]==""){
                                                                stack[top] = ":error:";
                                                        }
                                                        else{
                                                                top++;
                                                                stack[top] = ":error:";
                                                        }
                                                }

					}
					else if(y==13){//bind
						if(stack[top]!=NULL && stack[top]!="" && top>=1){
							if(eval(stack[top-1])==6 && eval(stack[top])!=8){

								int i = 0;
								int bound = 0;

								for(i=0;i<nameValueTop;i++){
									if(strcmp(nameStack[i], stack[top-1]) == 0){
										bound = 1;
										break;
									}
								}

								if(bound){
									if(stack[top]==NULL || stack[top]==""){
										stack[top] = ":error:";
									}
									else{
										top++;
										stack[top] = ":error:";
									}
								}
								else{
									char* name = malloc(sizeof(char*)*strlen(stack[top-1]));
									char* value = malloc(sizeof(char*)*strlen(stack[top]));
									name = stack[top-1];
									value = stack[top];
									stack[top] = "";
									top--;
									stack[top] = value;
									nameStack[nameValueTop] = name;
									valueStack[nameValueTop] = value;
									nameValueTop++;
								}
							}
							else{
								if(stack[top]==NULL || stack[top]==""){
									stack[top] = ":error:";
								}
								else{
									top++;
									stack[top] = ":error:";
								}
							}
						}
						else{
							if(stack[top]==NULL || stack[top]==""){
								stack[top] = ":error:";
							}
							else{
								top++;
								stack[top] = ":error:";
							}
						}
					}
					else if(y==14){//load
                                                if(stack[top]!="" && stack[top]!=NULL){
							if(eval(stack[top]) == 5){
								char* fileName = malloc(sizeof(char*)*strlen(stack[top]));
								strncpy(fileName, stack[top], strlen(stack[top])-1);
								fileName++;
								fPointer = fopen(fileName, "r");
								stack[top] = "";
								top--;

								if(fPointer != NULL){
									mode = 1;
									//top++;
									//stack[top] = ":true:";
								}
								else{
									top++;
									stack[top] = ":false:";
								}
							}
							else{
								top++;
								stack[top] = ":error:";
							}
                                                }
                                                else{
                                                        stack[top] = ":error:";
                                                }
					}
					else if(y==15){//apply
						if(stack[top] != NULL && stack[top] != ""){
							if(eval(stack[top]) == 7){
								mode = 2;
								closure = malloc(sizeof(char*)*strlen(stack[top]));
								strncpy(closure, stack[top], strlen(stack[top])-1);
								closure++;
								stack[top] = "";
								top--;
							}
							else{
								top++;
								stack[top] = ":error:";
							}
						}
						else{
							stack[top] = ":error:";
						}
					}
					else if(y==16){//if
						if(top >= 2){
							if(eval(stack[top-2]) != 8 && eval(stack[top-1]) != 8 
							&& (eval(stack[top]) == 2 || eval(stack[top]) == 3)){
								char* x = malloc(sizeof(char*)*strlen(stack[top-2]));
								char* y = malloc(sizeof(char*)*strlen(stack[top-1]));
								strncpy(x, stack[top-2], strlen(stack[top-2]));
								strncpy(y, stack[top-1], strlen(stack[top-1]));

								int true = 0;
								if(eval(stack[top])==2){
									true = 1;
								}
								
								stack[top] = "";
								top--;
								stack[top] = "";
								top--;

								if(true){
									stack[top] = x;
								}
								else{
									stack[top] = y;
								}
							}
							else{
								top++;
								stack[top] = ":error:";
							}
						}
						else{
							if(stack[top]==NULL || stack[top]==""){
								stack[top] = ":error:";
							}
							else{
								top++;
								stack[top] = ":error:";
							}
						}
					}
					else{
	                                        if(stack[top]==NULL || stack[top]==""){
	                                                stack[top] = ":error:";
	                                        }
                                       	 	else{
                                                	top++;
                                                	stack[top] = ":error:";
                                        	}
					}
				}
				else if(x==5){//String
					if(stack[top]==NULL || stack[top]==""){
                                        	stack[top] = word;
                                        }
                                        else{
                                        	top++;
                                        	stack[top] = word;
                                        }

				}
				else if(x==6){//Name
					int nameIndex = 0;
					int i = 0;
					int nameFound = 0;
					for(i=0;i<nameValueTop;i++){
						if(strcmp(nameStack[i], word) == 0){
							nameFound = 1;
							nameIndex = i;
							break;
						}
					}
					if(nameFound){
						if(stack[top]==NULL || stack[top]==""){
							stack[top] = valueStack[nameIndex];
						}
						else{
							top++;
							stack[top] = valueStack[nameIndex];
						}
					}
					else{
						if(stack[top]==NULL || stack[top]==""){
							stack[top] = word;
						}
						else{
							top++;
							stack[top] = word;
						}
					}
				}
				else if(x==7){//Valid closure
                                        if(stack[top]==NULL || stack[top]==""){
                                                stack[top] = word;
                                        }
                                        else{
                                                top++;
                                                stack[top] = word;
                                        }

				}
				else{//Error
					if(*word!='\n'){
						if(stack[top]==NULL || stack[top]==""){
							stack[top] = ":error:";
						}
						else{
							top++;
							stack[top] = ":error:";
						}
					}
				}

				//Print stack only when reaching end of line, or EOF in file mode.
				if(mode == 0){
					if(next == NULL){
						int index = 0;
						for(index=top;index>=0;index--){
							if(stack[index]!=""){
								if(eval(stack[index]) == 7){
									printf(":closure:\n");
								}
								else{
									printf("%s\n", stack[index]);
								}
							}
						}
						printf("repl> ");
						break;
					}
				}
				else if(mode == 1){
					if(next == NULL){
						if(endOfFile){
							int index = 0;
							for(index=top;index>=0;index--){
								if(stack[index]!=""){
									printf("%s\n", stack[index]);
								}
							}
							printf("repl> ");
							fclose(fPointer);
							break;
						}
						else{
							break;
						}
					}
				}
				else{
					if(next==NULL){
						break;
						mode = 0;
					}
				}
			}
			else{
				loop = 0;
				break;
			}
		}
	}
}

int eval(char* word){
                        int i = 0;
                        int isNumber = 1;
                        int isNegative = 0; // 0 = false, 1 = true

                        while(i < strlen(word)){
                                if(i == 0){
                                        if(word[0]=='-'){
                                                isNegative = 1;
                                        }
                                        else if(isdigit(word[0])==0){
                                                isNumber = 0;
                                                break;
                                        }
                                }
                                else{
                                        if(isdigit(word[i])==0){
                                                isNumber = 0;
                                                break;
                                        }
                                }
                                i++;
                        }
                        if(strlen(word)==1 && word[0]=='-'){
                                isNumber = 0;
                        }
                        if(isNumber == 1){
                                if(isNegative==0){
					return 0; //Number is positive
                                }
                                else{
					return 1; //Number is negative
                                }
                        }
                        else{
				if(strcmp(":true:", word) == 0){
					return 2; // :true:
				}
				else if(strcmp(":false:", word) ==0){
					return 3; // :false:
				}
				else if(strcmp("add", word) == 0 ||
					strcmp("sub", word) == 0 ||
					strcmp("mul", word) == 0 ||
					strcmp("div", word) == 0 ||
					strcmp("rem", word) == 0 ||
					strcmp("neg", word) == 0 ||
					strcmp("pop", word) == 0 ||
					strcmp("exc", word) == 0 ||
					strcmp("and", word) == 0 ||
					strcmp("or", word) == 0 ||
					strcmp("not", word) == 0 ||
					strcmp("equal", word) == 0 ||
					strcmp("lessThan", word) == 0 ||
					strcmp("bind", word) == 0 ||
					strcmp("load", word) == 0 ||
					strcmp("apply", word) == 0 ||
					strcmp("if", word) == 0){
					return 4; // command
				}
				else if(strlen(word) >= 2 && (word[0] == '\"' && word[strlen(word)-1] == '\"')){
					int i = 0;
					int quoteCount = 0;
					for(i=0;i<strlen(word);i++){ //A valid string begins and ends with a quote and has 2
						if(word[i]=='\"'){
							quoteCount++;
						}
					}
					if(quoteCount==2){
						return 5; //Valid string
					}
					return 8; //Invalid
				}
				else if(strlen(word) >= 3 && (word[0] == '{' && word[strlen(word)-1] == '}')){
					//A valid closure has an equal amount of closing and opening brackets.
					//A valid closure has white space after and before brackets.
					//A valid closure is at least 3 characters long
					int countOpenings = 0;
					int countClosings = 0;
					int i = 0;

					for(i=0;i<strlen(word);i++){
						if(word[i]=='{'){
							countOpenings++;
						}
						else if(word[i]=='}'){
							countClosings++;
						}
					}

					if(countOpenings==countClosings){
						if((word[1]==' ' || word[1]=='\t') && 
						(word[strlen(word)-2]==' ' || word[strlen(word)-2]=='\t')){
								return 7; //Valid closure
						}
						else{
							return 8;
						}
					}
					else{
						return 8; //Invalid
					}
				}
				else{
					int i = 0;
					for(i=0;i<strlen(word);i++){ //A valid name only has alphanumeric characters
						if(isalnum(word[i])==0){
							return 8; //Invalid
						}
					}
					return 6; //Valid name
				}
                        }
}

int evalCommand(char *word){
	if(strcmp("add", word) == 0){
		return 0;
	}
        else if(strcmp("sub", word) == 0){
		return 1;
	}
	else if(strcmp("mul", word) == 0){
		return 2;
	}
        else if(strcmp("div", word) == 0){
		return 3;
	}
        else if(strcmp("rem", word) == 0){
		return 4;
	}
        else if(strcmp("neg", word) == 0){
		return 5;
	}
        else if(strcmp("pop", word) == 0){
		return 6;
	}
        else if(strcmp("exc", word) == 0){
		return 7;
	}
	else if(strcmp("and", word) == 0){
		return 8;
	}
	else if(strcmp("or", word) == 0){
		return 9;
	}
	else if(strcmp("not", word) == 0){
		return 10;
	}
	else if(strcmp("equal", word) == 0){
		return 11;
	}
	else if(strcmp("lessThan", word) == 0){
		return 12;
	}
	else if(strcmp("bind", word) == 0){
		return 13;
	}
	else if(strcmp("load", word) == 0){
		return 14;
	}
	else if(strcmp("apply", word) == 0){
		return 15;
	}
	else if(strcmp("if", word) == 0){
		return 16;
	}
	else{
		return 17;
	}
}

char* parseLine(char* word){
	char* line = word;

	while(*line == ' ' || *line == '\t'){
		line++;
		word++;
	}

	if(*line == '\"'){
		int quoteFound = 0;
		while(1){
			line++;
			if(*line == '\"' && !quoteFound){
				quoteFound = 1;
				if(*(line+1) == ' ' || *(line+1) == '\0' ||  *(line+1) == '\t'){
					*(line+1) = '\0';
					break;
				}
			}
			else if(quoteFound){
				if(*line == ' ' || *line == '\0' || *line == '\t'){
					*line = '\0';
					break;
				}
			}

			if(*line == '\n'){
				break;
			}
		}
	}

	else if(*line == '{' && (*(line+1) == ' ' || *(line+1) == '\t')){
		int endOfClosure = 0;
		int openings = 1; //
		int closings = 0;
		line++;
		while(1){
			if(!endOfClosure){
				if(*line=='{' && (*(line-1)==' ' || *(line-1)=='\t') && (*(line+1)==' ' || *(line+1)=='\t')){
					openings++;
				}
				else if(*line=='}' && (*(line-1)==' ' || *(line-1)=='\t') && (*(line+1)==' ' || *(line+1)=='\t')){
					closings++;
				}
				else if(*line == '\n' || *line == '\0'){
					break;
				}

				if(openings == closings){
					endOfClosure = 1;
				}
			}
			else{
				if(*line == '\n'){
					break;
				}
				else if(*line == '\0' || *line == ' ' || *line == '\t'){
					*line = '\0';
					break;
				}
			}
			line++;
		}
	}
	else{
		while(1){
			line++;
			if(*line == ' ' || *line == '\0' || *line == '\t'){
				*line = '\0';
				break;
			}
		}
	}
	return word;
}

char* nextWord(char* temp){
	int quoteFound = 0;
	if(*temp == '{' && (*(temp+1) == ' ' || *(temp+1) == '\t')){
                int endOfClosure = 0;
                int openings = 1;
                int closings = 0;
                temp++;
                while(1){
                        if(!endOfClosure){
                                if(*temp=='{' && (*(temp-1)==' ' || *(temp-1)=='\t') && (*(temp+1)==' ' || *(temp+1)=='\t')){
                                        openings++;
                                }
                                else if(*temp=='}' && (*(temp-1)==' ' || *(temp-1)=='\t') && (*(temp+1)==' ' || *(temp+1)=='\t')){
                                        closings++;
                                }
                                else if(*temp == '\n' || *temp == '\0'){
                                        break;
                                }

                                if(openings == closings){
                                        endOfClosure = 1;
                                }
                        }
			else{
                                if(*temp == '\n'){
                                        break;
                                }
                                else if(*temp == '\0' || *temp == ' ' || *temp == '\t'){
                                        *temp = '\0';
                                        break;
                                }
                        }
                        temp++;
		}

		while(*temp == ' ' || *temp == '\t' || *temp == '\0'){
                        temp++;
                }

                if(*temp == '\n'){
                        return NULL;
                }
                else{
                        return temp;
                }
	}
	else if(*temp == '\"'){ //Parse a string.
		temp++;
		while(*temp != '\0' && *temp != '\n'){
			if(*temp == '\n'){
				return NULL; //End of line
			}
			if(*temp == '\"' && !quoteFound){
				temp++;
				quoteFound = 1;
			}
			else if(quoteFound){
				temp++;
				if(*temp == ' ' || *temp == '\0' || *temp == '\n'){
					if(*temp == '\n'){
						return NULL; //End of line
					}
					break;
				}
			}
			else{
				temp++;
			}
		}
                while(*temp == ' ' || *temp == '\t' || *temp == '\0'){
                        temp++;
                }
                if(*temp == '\n'){
                        return NULL;
                }
                else{
                        return temp;
                }
	}
	else{ //Parse a name
		while(*temp != ' ' && *temp != '\t' && *temp != '\n' && *temp != '\0'){
			temp++;
			if(*temp == '\n'){
				return NULL;
			}
		}
		while(*temp == ' ' || *temp == '\t' || *temp == '\0'){
			temp++;
		}	
		if(*temp == '\n'){
			return NULL;
		}
		else{
			return temp;
		}
	}
}
