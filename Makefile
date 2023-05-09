all: clean compile link

compile:
	g++ -c *.cpp

link:
	g++ *.o -o BurgerTech -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio 

clean:
	rm *.o -f
	rm BurgerTech -f
