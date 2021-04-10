#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <iomanip>


pqxx::connection C("host=localhost port=5432 dbname=mylinux user=postgres password=");

class DataBase{			
	public:
		DataBase();
		~DataBase();
		bool validate();
		void insert(std::string name, std::string description);
		void Delete(int id);
		void update(int id, std::string name, std::string description);
		void display();
		void showMenu();
};

DataBase::DataBase(){	
}

DataBase::~DataBase(){
	C.disconnect();
}

bool DataBase::validate(){
	if(C.is_open()){		
		return true;
	}
	else {		
		return false;	
	}
}

void DataBase::insert(std::string name, std::string description){
	if(validate()){					
		pqxx::work W(C); //Transactional Object
		C.prepare("insert","INSERT INTO commands(name, description) VALUES($1,$2);");		
		W.exec_prepared("insert", name, description);
		W.commit();
	}
	else {
		return;
	}
}

void DataBase::Delete(int id){	
	if(validate()){
		pqxx::work W(C);
		C.prepare("delete","DELETE FROM commands WHERE id = $1;");
		pqxx::result r = W.exec_prepared("delete",id);
		W.commit();
	}
	else {
		return;
	}
}

void DataBase::update(int id, std::string name, std::string description){
	if(validate()){
		pqxx::work W(C);
		C.prepare("update","UPDATE commands SET name = $1, description = $2 WHERE id = $3");
		W.exec_prepared("update",name, description, id);
		W.commit();
	}
	else {
		return;
	}
}

void DataBase::display(){
	//Create a NO transactional object
	pqxx::nontransaction NT(C);
	//Receive dataset
	pqxx::result buffer = NT.exec("SELECT * FROM commands ORDER BY id;");
	
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

void DataBase::showMenu(){	
	std::cout << std::endl << std::endl;
	std::cout << std::setw(25) << std::setfill('-') << "Options" << std::setw(15) << std::setfill('-') << "" << std::endl;
	std::cout << std::setw(25) << std::setfill('*') << "NEW COMMAND (1)" <<  std::setw(15) << std::setfill('-') << "" << std::endl;
	std::cout << std::setw(25) << std::setfill('*') << "UPDATE COMMAND (2)" <<  std::setw(15) << std::setfill('-') << "" << std::endl;
	std::cout << std::setw(25) << std::setfill('*') << "DELETE COMMAND (3)" <<  std::setw(15) << std::setfill('-') << "" << std::endl;	
	std::cout << std::setw(25) << std::setfill('*') << "DISPLAY COMMANDS (4)" <<  std::setw(15) << std::setfill('-') << "" << std::endl;
	std::cout << std::setw(25) << std::setfill('*') << "EXIT (ANY OTHER KEY)" <<  std::setw(15) << std::setfill('-') << "" << std::endl;	
}

int main(){
				
	DataBase db {};			
	
	bool repeat = true;
	std::string input = "";
	std::string option = "";
	
	while(repeat){		
		db.showMenu();	//Call the showMenu method		
		
		std::cout << "Please, choose an option: " ;
		std::getline(std::cin, option);											
		
		if(option == "1"){//Insert
			std::string name {};
			std::string desc {};
			std::cout << "Type the command: " << std::endl;
			std::getline(std::cin, name);
			std::cout << "Type the description: " << std::endl;
			std::getline(std::cin, desc);
			db.insert(name, desc);
			db.display();
		} 
		else if(option == "2"){//Update
			std::string name {};
			std::string desc {};
			int id = 0;
			std::cout << "Type the command: " << std::endl;
			std::getline(std::cin, name);
			std::cout << "Type its description: " << std::endl;
			std::getline(std::cin, desc);			
			std::cout << "Type its ID: " << std::endl;
			std::cin >> id;
			db.update(id, name, desc);
			db.display();
		}
		else if(option == "3"){//Delete
			int id = 0;
			std::cout << "Type the command\'s id: " << std::endl;
			std::cin >> id;
			db.Delete(id);
			db.display();
		}
		else if(option == "4"){//Display
			db.display();
		}
		else{
			std::cout << "Bye" << std::endl;
			return 0;
		}
		
		
		std::cout << "Select another option (Y/y/):" ;
		std::getline(std::cin, input);
		
		if(input == "Y" || input == "y" || input == ""){
			repeat = true;
		} 
		else {
			repeat = false;
		}
	}
		

	return 0;
}
