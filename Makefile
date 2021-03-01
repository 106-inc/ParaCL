FB_BLD = FB_BLD
BLD = PCL_BLD
FNB = grammar
DVR = driver
PSR = parser

CC = g++ --std=c++2a -Wall -Wextra

all: start

compile:
	$(CC) $(BLD)/scanner.o $(BLD)/compiler.o $(BLD)/driver.o $(BLD)/parser.o $(BLD)/inode.o $(BLD)/opnode.o $(BLD)/node.o

start: bld_dir fb_bld bison.o flex.o parser.o ast.o driver.o
	$(CC) -o $(BLD)/paracl main.cc $(BLD)/scanner.o $(BLD)/compiler.o $(BLD)/parser.o $(BLD)/inode.o $(BLD)/node.o $(BLD)/opnode.o $(BLD)/driver.o

clean:
	rm -rf BLD FD_BLD

bld_dir:
	mkdir -p $(BLD)

fb_bld:
	mkdir -p $(FB_BLD)

flex.o: $(FNB)/scanner.l
	flex --c++ -o $(FB_BLD)/lex.yy.cc $(FNB)/scanner.l
	$(CC) -c -o $(BLD)/scanner.o $(FB_BLD)/lex.yy.cc

bison.o: $(FNB)/compiler.y
	bison -d -o $(FB_BLD)/compiler.tab.cc $(FNB)/compiler.y
	$(CC) -c -o $(BLD)/compiler.o $(FB_BLD)/compiler.tab.cc

driver.o: $(DVR)/driver.cc
	$(CC) -c -o $(BLD)/driver.o $(DVR)/driver.cc

parser.o: $(PSR)/parser.cc
	$(CC) -c -o $(BLD)/parser.o $(PSR)/parser.cc

ast.o: AST/INode.cc AST/Node.cc
	$(CC) -c -o $(BLD)/inode.o AST/INode.cc
	$(CC) -c -o $(BLD)/node.o AST/Node.cc

main.o: main.cc
	$(CC) -c -o $(BLD)/main.o main.cc

