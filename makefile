# Vishwa V

all: sumYinRangeX clean

sumYinRangeX: sumYinRangeX.o
	gcc -g -o $@ $^

%.o:%.c
	gcc -g -c $< -o $@

clean:
	rm -rf *.o
