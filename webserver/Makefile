*WWW = -Wall -W -Werror

#Build apache3
apache3: apache3.o
	gcc -o $@ $^

#Build .o
apache3.o: apache3.c
	gcc -o $@ -c $^ $(WWW)
