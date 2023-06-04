TESTS_OBJS = BonusTest.o BurgerTest.o SliceTest.o PlayerTest.o GameTest.o LevelTest.o
GAMESFILES_OBJS = Game.o Bonus.o Burger.o Egg.o Level.o Enemy.o Movable.o Pepper.o Pickle.o Player.o Sausage.o Score.o Sheet.o Slice.o SpriteSheet.o Trace.o Updatable.o
MAIN_OBJS = Main.o

GAME_OBJS = $(GAMESFILES_OBJS) $(MAIN_OBJS)

GAMESFILES_SRC =  Game.cpp Bonus.cpp Burger.cpp Egg.cpp Level.cpp Enemy.cpp Movable.cpp Pepper.cpp Pickle.cpp Player.cpp Sausage.cpp Score.cpp Sheet.cpp Slice.cpp SpriteSheet.cpp Trace.cpp Updatable.cpp

FLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio 


all: clean compile link

compile:
	g++ -c *.cpp

link:
	g++ $(GAME_OBJS) -o BurgerTech $(FLAGS) 

clean:
	rm *.o -f
	rm BurgerTech BonusTest BurgerTest SliceTest PlayerTest GameTest LevelTest -f

clean_tests :
	rm $(TESTS_OBJS) -f
	rm BonusTest BurgerTest SliceTest PlayerTest GameTest LevelTest -f

BonusTest:
	rm $(TESTS_OBJS) -f
	rm BonusTest -f
	g++ -c BonusTest.cpp 
	g++ BonusTest.o $(GAMESFILES_OBJS) -o BonusTest $(FLAGS) 

BurgerTest:
	rm $(TESTS_OBJS) -f
	rm BurgerTest -f
	g++ -c BurgerTest.cpp 
	g++ BurgerTest.o $(GAMESFILES_OBJS) -o BurgerTest $(FLAGS)

SliceTest:
	rm $(TESTS_OBJS) -f
	rm SliceTest -f
	g++ -c SliceTest.cpp 
	g++ SliceTest.o $(GAMESFILES_OBJS) -o SliceTest $(FLAGS) 

PlayerTest:
	rm $(TESTS_OBJS) -f
	rm PlayerTest -f
	g++ -c PlayerTest.cpp 
	g++ PlayerTest.o $(GAMESFILES_OBJS) -o PlayerTest $(FLAGS)


GameTest:
	rm $(TESTS_OBJS) -f
	rm GameTest -f
	g++ -c GameTest.cpp 
	g++ GameTest.o $(GAMESFILES_OBJS) -o GameTest $(FLAGS)

LevelTest:
	rm $(TESTS_OBJS) -f
	rm LevelTest -f
	g++ -c LevelTest.cpp 
	g++ LevelTest.o $(GAMESFILES_OBJS) -o LevelTest $(FLAGS)
