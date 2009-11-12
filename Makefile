run:
	clear
	g++ -ansi -pedantic -Wall main.c++ -o main.app
	valgrind main.app

rt:
	g++ -ansi -pedantic -lcppunit -ldl -Wall -DTEST main.c++ -o main.app
	valgrind main.app

runsmalltest:
	g++ -ansi -pedantic -Wall main.c++ -o main.app
	main.app < smalltest

gitcommit:
	git commit -a

