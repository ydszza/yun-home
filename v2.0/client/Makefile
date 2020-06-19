objects = main.o welcome.o lcd.o show_smoke.o show_gy39.o show_contrlapp.o socket.o

main: $(objects)
	gcc -o main $(objects) -lpthread

%.o: %.c
	gcc -c $<




.PYTHON:clean

clean :
	rm *.o
	rm main