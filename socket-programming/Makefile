all: rpsserver rpsclient

rpsserver: rps.c
	gcc -g $< -o $@

rpsclient: rps.c
	gcc -g $< -o $@

clean:
	rm -f rpsserver rpsclient