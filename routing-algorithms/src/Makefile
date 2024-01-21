all: routing_ls routing_dv

routing_ls: routing_ls.c
	gcc -g $< -o $@

routing_dv: routing_dv.c
	gcc -g $< -o $@

clean:
	rm -f routing_ls routing_dv
