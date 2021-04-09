#include <iostream>
#include <pqxx/pqxx>
#include <string>

pqxx::connection C("host=localhost port=5432 dbname=mylinux user=postgres password=");	

bool validate(){
	if(C.is_open()){
		std::cout << "Connected..." << std::endl;
		return true;
	}
	else {
		std::cout << "Not connected" << std::endl;	
		return false;	
	}
}

void insert(std::string name, std::string description){
	if(validate()){		
		pqxx::work W(C); //Transactional Object
		C.prepare("insert","INSERT INTO commands(name, description) VALUES($1,$2);");		
		W.exec_prepared("insert",name, description);
		W.commit();
	}
	else {
		return;
	}
}

void Delete(int id){	
	if(validate()){
		pqxx::work W(C);
		C.prepare("delete","DELETE FROM commands WHERE id = $1;");
		pqxx::result r = W.exec_prepared("delete",id);
		W.commit();
	}
}

void display(){
	//Create a NO transactional object
	pqxx::nontransaction NT(C);
	//Receive dataset
	pqxx::result buffer = NT.exec("SELECT * FROM commands;");
	
	if(buffer.size() < 1){
		std::cout << "There was an issue" << std::endl;
		return;
	}
	std::cout << "id  |    name    | description" << std::endl;
	std::cout << "------------------------------" << std::endl;
	for(size_t i=0;i<buffer.size();i++){
		std::cout << buffer[i][0] << " | " << buffer[i][1] << " | " << buffer[i][2] << std::endl;
		std::cout << "------------------------------" << std::endl;
	}
}

int main(){
			
	insert("lastOne","Description");
	display();
	
	int id = 0;
	
	std::cout << "Insert ID to delete" << std::endl;
	std::cin >> id; 
	
	Delete(id);
	
	display();

	C.disconnect();
	return 0;
}
