FB_BLD = FB_BLD
BLD = PCL_BLD
FNB = flex+bison
DVR = Driver

all: start

start: bld_dir fb_bld bison flex driver AST
	g++ -o $(BLD)/paracl main.cc $(BLD)/scanner.o $(BLD)/compiler.o $(BLD)/driver.o $(BLD)/node.c

start_tmp: bld_dir fb_bld flex bison
	g++ -o $(BLD)/paracl main.cc $(FB_BLD)/lex.yy.cc $(FB_BLD)/compiler.tab.cc $(DVR)/driver.cc AST/INode.cc AST/Node.cc AST/OPNode.cc

bld_dir:
	mkdir -p $(BLD)

fb_bld: 
	mkdir -p $(FB_BLD)

flex: $(FNB)/scanner.l
	flex -o $(FB_BLD)/lex.yy.cc $(FNB)/scanner.l
	g++ -c -o $(BLD)/scanner.o $(FB_BLD)/lex.yy.cc

bison: $(FNB)/compiler.y
	bison -d -o $(FB_BLD)/compiler.tab.cc $(FNB)/compiler.y
	g++ -c -o $(BLD)/compiler.o $(FB_BLD)/compiler.tab.cc

driver: $(DVR)/driver.cc
	g++ -c -o $(BLD)/driver.o $(DVR)/driver.cc

AST: AST/INode.cc AST/Node.cc AST/OPNode.cc
	g++ -c -o $(BLD)/node.o AST/INode.cc AST/Node.cc AST/OPNode.cc