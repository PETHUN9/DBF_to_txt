#include <iostream>
#include <string>
#include <cstring>
#include "dbheader.h"



int main(){
	
	std::cout << "Honnan szeretnel beolvasni?" << std::endl;
	std::string be; 
	std::getline(std::cin, be);

	std::cout << "Hova szeretned kiirni?" << std::endl;
	std::string ki;
	std::getline(std::cin, ki);
	try {
		dbheader dbase(be);
		dbase.write(ki);
	}
	catch(int errnum){
		std::cerr << "Could not open file" << std::endl;
		std::cout << "Error number: " << errnum << std::endl;
	}
	return 0;
}