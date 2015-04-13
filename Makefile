CFLAGS += -pipe -std=c99 -O3 -march=native -DNDEBUG -fforce-addr -ffast-math -fomit-frame-pointer -flto -Wl,-O3 -Wl,-flto

all:
	mkdir -p buildviamake
	cd buildviamake && $(CC) $(CFLAGS) ../*.c -o huffar

clean:
	rm -rf buildviamake
