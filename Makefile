FB_BLD = FB_BLD
BLD = PCL_BLD
FNB = flex+bison
DVR = Driver
PSR = parser

CC = g++ --std=c++2a

all: start_tmp2

compile: 
	$(CC) $(BLD)/scanner.o $(BLD)/compiler.o $(BLD)/driver.o $(BLD)/parser.o $(BLD)/inode.o $(BLD)/opnode.o $(BLD)/node.o

start: bld_dir fb_bld bison flex driver ast
	$(CC) -o $(BLD)/paracl main.cc $(BLD)/scanner.o $(BLD)/compiler.o $(BLD)/driver.o $(BLD)/*.o

start_tmp1: bld_dir fb_bld bison flex driver ast
	$(CC) -o $(BLD)/paracl main.cc $(BLD)/*.o $(FB_BLD)/compiler.tab.cc $(FB_BLD)/lex.yy.cc

start_tmp2: bld_dir fb_bld driver parser ast bison flex
	$(CC) -o $(BLD)/paracl main.cc $(BLD)/*.o

bld_dir:
	mkdir -p $(BLD)

fb_bld: 
	mkdir -p $(FB_BLD)

flex: $(FNB)/scanner.l
	flex --c++ -o $(FB_BLD)/lex.yy.cc $(FNB)/scanner.l
	$(CC) -c -o $(BLD)/scanner.o $(FB_BLD)/lex.yy.cc

bison: $(FNB)/compiler.y
	bison -d -o $(FB_BLD)/compiler.tab.cc $(FNB)/compiler.y
	$(CC) -c -o $(BLD)/compiler.o $(FB_BLD)/compiler.tab.cc

driver: $(DVR)/driver.cc
	$(CC) -c -o $(BLD)/driver.o $(DVR)/driver.cc

parser: $(PSR)/parser.cc
	$(CC) -c -o $(BLD)/parser.o $(PSR)/parser.cc

ast: AST/INode.cc AST/Node.cc AST/OPNode.cc
	$(CC) -c -o $(BLD)/inode.o AST/INode.cc
	$(CC) -c -o $(BLD)/node.o AST/Node.cc
	$(CC) -c -o $(BLD)/opnode.o AST/OPNode.cc

main: main.cc
	$(CC) -c -o $(BLD)/main.o main.cc
