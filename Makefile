FILES = main.cpp functions.h
DEPS = header.h

all: $(FILES)
	mpic++ $(FILES) -o main

clean:
	rm main
