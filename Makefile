# R-Type Makefile wrapper for CMake

.PHONY: all server client clean rebuild rebuild-server rebuild-client install help

# Default target
all:
	@./build.sh all
	cp ./build/Server/rtype_server .
	cp ./build/Client/rtype_client .

server:
	@./build.sh server
	cp ./build/Server/rtype_server .

client:
	@./build.sh client
	cp ./build/Client/rtype_client .

clean:
	@./build.sh clean
	@rm -f rtype_server rtype_client

rebuild:
	@./build.sh rebuild
	cp ./build/Server/rtype_server .
	cp ./build/Client/rtype_client .


rebuild-server:
	@./build.sh rebuild-server

rebuild-client:
	@./build.sh rebuild-client

install:
	@./build.sh install

help:
	@echo "R-Type Project Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  all     - Build both server and client (default)"
	@echo "  server  - Build only the server"
	@echo "  client  - Build only the client"
	@echo "  clean          - Clean build directory"
	@echo "  rebuild        - Clean and rebuild everything"
	@echo "  rebuild-server - Clean and rebuild server only"
	@echo "  rebuild-client - Clean and rebuild client only"
	@echo "  install        - Install the project"
	@echo "  help    - Show this help message"
	@echo ""
	@echo "Built executables will be in:"
	@echo "  Server: ./build/Server/rtype_server"
	@echo "  Client: ./build/Client/rtype_client"