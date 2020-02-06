#include "parser.h"
#include "prints.h"
#include "grammar.h"

string parser::compile(string fn){
	if(startup(fn) == false){
		cout << "Unable to open file: " << fn << endl;
		return NULL;
	}
	else{
		//printProduction(0);
		nextToken = scan.getNextToken(); //get first token
		if(scan.isError()){
			return scan.getError();
		}
		token t;
		t.setId(TOK_N_PROGRAM); //set token ID as program 
		tokStack.push(t); // push token to stack
		while((!tokStack.empty()) && (!scan.isError()) && (nextToken.getId() != TOK_EOS)){ //while no error, stack is not empty and string is not empty 
			top = tokStack.top(); // first character on stack
			tokStack.pop();
			if(isNonTerminal(top) == true){ //popped element is NonTerminal
				int index = selectProd(); //find production

				if(index != -1){ //production found
				//	cout <<  "pushed RHS" << endl;
					pushProdRHS(index);
					cout << "TEST: PROD SELECTED = ";
					printProduction(index);			
				//	pushProdRHS(index);
				}
			/*	else if(index == -1){
					string error = "No production found for top=" + TOKENID_STR[top.getId()] + " next=" + TOKENID_STR[nextToken.getId()];
					return error;
				}	*/ //this doesn't seem to be working properly. Causes error in p1.pas and p2.pas

			}
			else{// if(isNonTerminal(top) == false){
				//nextToken = scan.getNexToken();
				if(nextToken.getId() == top.getId()){ //popped token is non-terminal and matches source, then consume
					cout <<"TEST: " << TOKENID_STR[nextToken.getId()] <<  " consumed" << endl;
					nextToken = scan.getNextToken(); //get next token from source
					if(scan.isError()){ //scanner error
						return scan.getError();
					}
				}
				else{
					string error = TOKENID_STR[top.getId()] + " expected!"; //terminal does not match with terminal from source
					return error;
				}
			}
		}
		nextToken = scan.getNextToken();

		if(tokStack.empty() == true && (nextToken.getId() != TOK_EOS)){ //stack is empty but source is not
			return "Extra tokens at end of source";
		}
		
		else if((tokStack.empty() == false) && (nextToken.getId() == TOK_EOS)){ //stack is not empty but source is
			return "Unexpected End of Source";
		}

		else if((nextToken.getId() == TOK_EOS) && (scan.isError() == false) && (tokStack.empty() == true)){ //stack is empty, no error, source is empty. String accepted
			return "";
		}
		else if(scan.isError() == true){//scanner error
			return scan.getError();	
		}
	}
}//compile() 


int parser::selectProd(){
	for(int i = GR_LHS; i < GR_NUMPRODS; i++){ //iterate through rows of productions array
		if(top.getId() == PROD[i][GR_LHS]){ //LHS must equal current top token ID
			if(PROD[i][GR_FIRST_SELSET] == TOK_DEFAULT){ //if first token in the select set is TOK_DEFAULT, select that production
				return i;
			}
			else{
				for(int j = GR_FIRST_SELSET; j <= GR_LAST_SELSET; j++){ //check other select set elements
					if(nextToken.getId() == PROD[i][j]){ //if the nextToken matches one of the select set elements, choose that production
						return i;
					}
				}
			}
		
		}
	}
	return -1;//if not found, return -1. This should not occur since all non-terminals have a default in grammar.h
}//selectProd()


/*int parser::selectProd(){
	for(int i = GR_LHS; i < GR_NUMPRODS; i++){
		for(int j= GR_FIRST_SELSET; j <= GR_LAST_SELSET; j++){
			if(top.getId() == PROD[i][GR_LHS]){
				
				if(PROD[i][GR_FIRST_SELSET] == TOK_DEFAULT){
					return i;
			}
		}	
			else{
				if(nextToken.getId() == PROD[i][j]){
				return i;
			}
		}
	}
	}
	return -1;
}//selectProd();*/


bool parser::isNonTerminal(token t){
	if((t.getId() <= LAST_NON_TERMINAL) && (t.getId() >= FIRST_NON_TERMINAL)){ //id must be in between non-terminal indexes
		return true;
	}
	else{
		return false;
	}
}//isNonTerminal()


void parser::pushProdRHS(int prodNum){
	for(int i = GR_LAST_RHS; i >= GR_FIRST_RHS; i--){ //start at last selset index to push in right to left order
		token t;
		t.setId(PROD[prodNum][i]); //set ID then push for all elements in that row of the production array
		tokStack.push(t);
	}
}


bool parser::startup(string filename){
	if(scan.open(filename) == 0){
		return false;
	}
	else{
		return true;
	}

}//startup()

