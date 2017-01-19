all: server client

server:
	gcc -m32 -O0 -g EasyServer.c -o easy32 -fno-stack-protector

client:
	gcc -m32 -O0 -g hack-easy32.c -o hack-easy32

clean:
	rm -rf hack-easy32 easy32
