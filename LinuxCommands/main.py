import psycopg2

class Database:

	def __init__(self, db):
		self.connector = psycopg2.connect(
			dbname = db,
			user = 'postgres',
			password = '',
			host = 'localhost',
			port = '5432'
		)
		self.cursor = self.connector.cursor()

	def postgresQuery(self, query):	
		result = self.cursor.execute(query)
		row = self.cursor.fetchall()
		self.connector.commit()
		print('Succesfull Query')
		return row

	def postgresModify(self, query):		
		self.cursor.execute(query)
		self.connector.commit()		
		print('Succesfull Query')
		
	def updateDescription(self, id, newDescription):
		query = "UPDATE commands SET description = '{}' WHERE id = {};".format(newDescription, id)
		self.postgresModify(query)		
		
	def updateCommand(self, id, newCommand):
		query = "UPDATE commands SET name = '{}' WHERE id = {};".format(newCommand, id)
		self.postgresModify(query)		
		
	def insert(self, name, description):
		query = "INSERT INTO commands(name, description) VALUES('{}','{}');".format(name, description)
		self.postgresModify(query)	
	
	def delete(self, id):
		query = "DELETE FROM commands WHERE id = {}".format(id)
		self.postgresModify(query)
		
	def display(self):
		query = "SELECT * FROM commands ORDER BY id"
		buffer = self.postgresQuery(query)
		print('id |     name     | Description')
		print('---+---------------------------------------------')
		for i in range(len(buffer)):			
			if(buffer[i][0] < 10):#Condition to arrange the table when displaying numbers < 10				
				print(f"{buffer[i][0] }  | {buffer[i][1]} | {buffer[i][2]}")
				print('---+---------------------------------------------')
			else:								
				print(f"{buffer[i][0]} | {buffer[i][1]} | {buffer[i][2]}")
				print('---+---------------------------------------------')				
	#Destructor
	def __del__(self):		
		self.connector.close()
		print('Connection Closed')


db = Database('mylinux') #Create DataBase class instance
db.display() #Use display method to show all records
"""
Examples:
db.updateDescription(id, 'New Description')
db.insert('Command','Description')
db.delete(id)
"""
