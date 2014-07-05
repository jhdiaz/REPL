def repl():
	stack = []
	nameStack = []
	valueStack = []
	line = ""
	loop = True
	mode = 0 #0 to read from user, 1 to read from file, 2 to read from closure
	fileName = ""
	fileLineArray = [] #Array where file lines are stored to be interpreted
	countLines = 0 #Number of lines in file
	processed = False #Whether the file has been read
	currentLine = 0 #Current line of file
	closure = "" #Variable where closure is saved into for evaluation

	while loop:
		if mode==0:
			line = input("repl> ")
			line = line.lstrip()

		elif mode==1:
			if(not processed):
				for row in inputFile:
					fileLineArray.append(row.lstrip())
					countLines += 1
				processed = True
				inputFile.close()
				line = ""
			else:
				if(currentLine<countLines):
					line = fileLineArray[currentLine].rstrip()
					currentLine += 1
				else:
					line = ""
					processed = False
					stack.append(":true:")
					mode = 0

		else:
			line = closure
			mode = 0

		if(1):
			for word in parseWord(line):

				if word=="quit":
					loop = False;
					break
	
				if isNumber(word):
					if(int(word)==0):
						stack.append("0")
					else:
						stack.append(word)
				
				else:
					if(word=="add"): #done
						if len(stack)>=2:
							if(isNumber(stack[len(stack)-1]) and isNumber(stack[len(stack)-2])):
								result = int(stack[len(stack)-2]) + int(stack[len(stack)-1])
								stack.pop()
								stack.pop()
								stack.append(str(result))
							else:
								stack.append(":error:")
						else:
							stack.append(":error:")

					elif(word=="sub"): #done
                                        	if len(stack)>=2:
                                                	if(isNumber(stack[len(stack)-1]) and isNumber(stack[len(stack)-2])):
                                                        	result = int(stack[len(stack)-2]) - int(stack[len(stack)-1])
                                                        	stack.pop()
                                                        	stack.pop()
                                                        	stack.append(str(result))
                                                	else:
                                                	        stack.append(":error:")
                                        	else:
                                        	        stack.append(":error:")
						
					elif(word=="mul"): #done
						if len(stack)>=2:
							if(isNumber(stack[len(stack)-1]) and isNumber(stack[len(stack)-2])):
								result = int(stack[len(stack)-2]) * int(stack[len(stack)-1])
								stack.pop()
								stack.pop()
								stack.append(str(result))
							else:
								stack.append(":error:")
						else:
							stack.append(":error:")
	
	
					elif(word=="div"): #done
						if len(stack)>=2:
							x = stack[len(stack)-2]
							y = stack[len(stack)-1]
							if(isNumber(x) and isNumber(y) and int(y)!=0):
								x = int(x)
								y = int(y)
								result = 0

								if(x<0):
									a = -1
									if(y<0):
										a = 1

									result = (x/y) + a
									result = int(result)

								else:
									result = x/y
									result = int(result)

								stack.pop()
								stack.pop()
								stack.append(str(result))
							else:
	                                                        stack.append(":error:")
						else:
	                                                stack.append(":error:")
	
	
					elif(word=="rem"): #done
						if len(stack)>=2:
							x = stack[len(stack)-2]
							y = stack[len(stack)-1]
							if(isNumber(x) and isNumber(y) and int(y)!=0):
								result = 0
								quotient = 0;
								x = int(x)
								y = int(y)
								if(x<0):
									a = -1
									if(y<0):
										a = 1
									quotient = (x//y) + a

								else:
									quotient = x//y

								if (x>0 and y>0) or (x<0 and y<0):
									result = x-y*quotient
								elif (x>0 and y<0):
									y *= -1
									result = x%y
								else:
									result = x%y

								stack.pop()
								stack.pop()
								stack.append(str(result))
							else:
								stack.append(":error:")
						else:
							stack.append(":error:")
	
					elif(word=="neg"): #done
						if len(stack)>=1:
							result = int(stack[len(stack)-1]) * -1
							stack.pop()
							stack.append(str(result))
						else:
							stack.append(":error:")
	
					elif(word=="pop"): #done
						if len(stack)>=1:
							stack.pop()
						else:
							stack.append(":error:")
	
					elif(word=="exc"): #done
						if len(stack)>=2:
							temp = stack[len(stack)-1]
							stack[len(stack)-1] = stack[len(stack)-2]
							stack[len(stack)-2] = temp
						else:
							stack.append(":error:")

					elif(word=="and"): #done
						if len(stack)>=2:
							x = stack[len(stack)-2]
							y = stack[len(stack)-1]
							if isBoolean(x) and isBoolean(y):
								stack.pop()
								stack.pop()
								if(x==":true:" and y==":true:"):
									stack.append(":true:")
								else:
									stack.append(":false:")
							else:
								stack.append(":error:")
						else:
							stack.append(":error:")

					elif(word=="or"): #done
						if len(stack)>=2:
							x = stack[len(stack)-2]
							y = stack[len(stack)-1]
							if isBoolean(x) and isBoolean(y):
								stack.pop()
								stack.pop()
								if(x==":true:" or y==":true:"):
									stack.append(":true:")
								else:
									stack.append(":false:")
							else:
								stack.append(":error:")
						else:
							stack.append(":error:")

					elif(word=="not"): #done
						if len(stack)>=1:
							x = stack[len(stack)-1]
							if isBoolean(x):
								stack.pop()
								if(x==":true:"):
									stack.append(":false:")
								else:
									stack.append(":true:")
							else:
								stack.append(":error:")
								
						else:
							stack.append(":error:")

					elif(word=="equal"): #done
						if len(stack)>=2:
							x = stack[len(stack)-2]
							y = stack[len(stack)-1]
							if isNumber(x) and isNumber(y):
								stack.pop()
								stack.pop()
								if(int(x)==int(y)):
									stack.append(":true:")
								else:
									stack.append(":false:")

							elif(isBoolean(x) and isBoolean(y)):
								if(x==":true:" and y==":true:" or x==":false:" and y==":false:"):
									stack.pop()
									stack.pop()
									stack.append(":true:")
								else:
									stack.pop()
									stack.pop()
									stack.append(":false:")

							elif(isName(x) and isName(y) or isString(x) and isString(y)):
								if(x==y):
									stack.pop()
									stack.pop()
									stack.append(":true:")
								else:
									stack.pop()
									stack.pop()
									stack.append(":false:")

							elif(isError(x) or isError(y)):
								stack.append(":error:")

							elif(isClosure(x) or isClosure(y)):
								stack.pop()
								stack.pop()
								stack.append(":false:")

							else:
								stack.pop()
								stack.pop()
								stack.append(":false:")
						else:
							stack.append(":error:")

					elif(word=="lessThan"): #done
						if len(stack)>=2:
							x = stack[len(stack)-2]
							y = stack[len(stack)-1]
							if isNumber(x) and isNumber(y):
								stack.pop()
								stack.pop()
								if(int(x)<int(y)):
									stack.append(":true:")
								else:
									stack.append(":false:")
							else:
								stack.append(":error:")
						else:
							stack.append(":error:")

					elif(word=="bind"):
						if len(stack)>=2:
							x = stack[len(stack)-2]
							y = stack[len(stack)-1]
							if isName(x) and y != ":error:":
								stack.pop()
								stack.pop()
								nameStack.append(x)
								valueStack.append(y)
								stack.append(y)
							else:
								stack.append(":error:")
						else:
							stack.append(":error:")

					elif(word=="load"): #done
						if len(stack)>=1:
							x = stack[len(stack)-1]
							opened = False
							if(isString(x)):
								fileName = x[1:len(x)-1]
								try:
									inputFile = open(fileName, "r")
									opened = True

								except IOError:
									stack.pop()
									stack.append(":false:")

								if opened:
									mode = 1
									popIndex = len(stack)-1
									stack.pop()
								
							else:
								stack.append(":error:")
						else:
							stack.append(":error:")

					elif(word=="apply"):
						if len(stack)>=1:
							x = stack[len(stack)-1]
							if isClosure(x):
								x = x[1:len(x)-1]
								x = x.lstrip()
								x = x.rstrip()
								mode = 2
								closure = x
								stack.pop()
							else:
								stack.append(":error:")
						else:
							stack.append(":error:")

					elif(word=="if"): #done
						if len(stack)>=3:
							x = stack[len(stack)-3]
							y = stack[len(stack)-2]
							bool = stack[len(stack)-1]
							if(x !=":error:" and y != ":error:" and isBoolean(bool)):
								stack.pop()
								stack.pop()
								stack.pop()
								if bool == ":true:":
									stack.append(x)
								else:
									stack.append(y)
							else:
								stack.append(":error:")
						else:
							stack.append(":error:")

					elif(word==":true:"): #done
						stack.append(":true:")
	
					elif(word==":false:"): #done
						stack.append(":false:")

					elif(isName(word)):
						nameFound = False
						index = 0
						for i in range(len(nameStack)):
							if word == nameStack[i]:
								nameFound = True
								index = i
								break
						if nameFound:
							stack.append(valueStack[index])
						else:
							stack.append(word)

					elif(isString(word)): #done
						stack.append(word)

					elif(isClosure(word)): #done
						stack.append(word)

					else:
						stack.append(":error:")

		if loop != False and mode == 0:
			for item in reversed(stack):
				if isClosure(item):
					print(":closure:")
				else:
					print (item)


def isNumber(word):
	if len(word)==0:
		return False
	elif len(word)==1:
		return word.isdigit()
	else:
		if word[0]=='-' or word[0].isdigit():
			return word[1:len(word)].isdigit()
		else:
			return False

def parseWord(line):
	stringArray = []
	i = 0
	quoteFound = False
	closingFound = False

	if(len(line)<=1):
		if(len(line)==1):
			stringArray.append(line)
		return stringArray

	while(i<len(line)):
		if(line[i] == "\""):
			quoteFound = False
			i += 1
			while(i<len(line)):
				if(line[i] == "\"" and not quoteFound):
					quoteFound = True
					i += 1

				if(quoteFound and i<len(line) and (line[i] == " " or line[i] == "\t" or i==len(line))):
					stringArray.append(line[0:i])
					if(i+1<=len(line)):
						line = line[i:len(line)]
						line = line.lstrip()
						i = 0
					break

				elif(i>=len(line)-1):
					stringArray.append(line)
					i += 1
					break

				else:
					i += 1


		elif(i+1<len(line) and (line[i:i+2] == "{ " or line[i:i+2] == "{\t")):
			closingFound = False
			openings = 1
			closings = 0
			i += 2
			while(i<len(line)):
				if(not closingFound):
					if(line[i]=="{"):
						openings += 1
					elif(line[i]=="}"):
						closings += 1
					if(i==len(line)-1):
						stringArray.append(line)

					if(openings==closings):
						closingFound = True
				else:	
					if(line[i]==" " or line[i]=="\t"):
						stringArray.append(line[0:i])

						if(i+1<=len(line)):
							line = line[i:len(line)]
							line = line.lstrip()
							i = 0
							break
				i += 1

		else:
			i += 1
			if(i==len(line)):
				stringArray.append(line)
			else:
				while(i<len(line)):
					if(line[i] == " " or line[i] == "\t"):
						stringArray.append(line[0:i])
						if(i+1<=len(line)):
							line = line[i:len(line)]
							line = line.lstrip()
							i = 0
						break
					else:
						i += 1

					if(i==len(line)):
						stringArray.append(line)

	return stringArray

def isName(word):
	if(len(word)<=1):
		return (not word.isdigit() and word.isalpha())
	else:
		if(word[0].isdigit()):
			return False
		else:
			for i in range(len(word)):
				if(word[i].isalnum() == False):
					return False

	return True

def isString(word):
	count = 0
	if(len(word)<=1):
		return False
	else:
		if(word[0]=="\"" and word[len(word)-1]=="\""):
			for i in range(len(word)):
				if(word[i]=="\""):
					count += 1
			if(count==2):
				return True
	return False

def isClosure(word):
	openings = 0
	closings = 0
	if(len(word)>=3):
		if(word[0]=="{" and word[len(word)-1]=="}"):
			for i in range(len(word)):
				if(word[i]=="{"):
					openings += 1
				if(word[i]=="}"):
					closings += 1
			if(openings==closings):
				return True
	return False

def isBoolean(word):
	if(word == ":true:" or word == ":false:"):
		return True
	return False

def isError(word):
	if(word==":error:"):
		return True
	return False

repl()
