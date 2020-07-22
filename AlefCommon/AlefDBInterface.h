#pragma once

#include "AlefTypes.h"

#include "AlefDatabase.h"

constexpr auto MAX_DB = 3;

class AlefDBInterface
{
public:
	AlefDBInterface();
	~AlefDBInterface();

    enum dbType : Int32
    {
        loginDB = 0,
        worldDB = 1,
        dataDB = 2
    };

    bool createDatabaseConnection(dbType type, std::string connectionString);
    Session getLoginDB()    {   return acquireDatabaseSession(dbType::loginDB); }
    Session getWorldDB()    {   return acquireDatabaseSession(dbType::worldDB); }
    Session getDataDB()     {   return acquireDatabaseSession(dbType::dataDB);  }


    void test();

private:
	//0 - LoginDB
	//1 - WorldDB
	//2 - DataDB
    Session acquireDatabaseSession(dbType type);
    AlefDatabase* database[MAX_DB]; //this should probably just be a vector
	std::string sessionNames[MAX_DB]; //MySQL:///<connectionstring>
	
};



/*

void exampleAccountData()
{
    Session loginSess = dbInterface->acquireDatabaseSession(LOGINDB);

    Statement accountCheck(loginSess);

    accountCheck << "SELECT * FROM account WHERE user = ? AND pw = ?", use(loginAcct), use(password);

    accountCheck.execute();

    RecordSet rs(accountCheck);

    //do stuff after this
	
}


void connect()
{
	"host=localhost;port=3306;db=mydb;user=alice;password=s3cr3t;compress=true;auto-reconnect=true"
}
*/

/*NOTES

	// register SQLite connector
    Poco::Data::SQLite::Connector::registerConnector();

    // create a session
    Session session("SQLite", "sample.db");

    // drop sample table, if it exists
    session << "DROP TABLE IF EXISTS Person", now;

    // (re)create table
    session << "CREATE TABLE Person (Name VARCHAR(30), Address VARCHAR, Age INTEGER(3))", now;

    // insert some rows
    Person person = 
    {
        "Bart Simpson",
        "Springfield",
        12
    };

    Statement insert(session);
    insert << "INSERT INTO Person VALUES(?, ?, ?)",
        use(person.name),
        use(person.address),
        use(person.age);

    insert.execute();

    person.name    = "Lisa Simpson";
    person.address = "Springfield";
    person.age     = 10;

    insert.execute();

    // a simple query
    Statement select(session);
    select << "SELECT Name, Address, Age FROM Person",
        into(person.name),
        into(person.address),
        into(person.age),
        range(0, 1); //  iterate over result set one row at a time

    while (!select.done())
    {
        select.execute();
        std::cout << person.name << " " << person.address << " " << person.age << std::endl;
    }


    std::string aName;
std::string match("Peter")
ses << "SELECT NAME FROM FORENAME WHERE NAME=?", into(aName), use(match), now;
poco_assert (aName == match);

std::string firstName("Peter";
std::string lastName("Junior");
int age = 0;
ses << INSERT INTO PERSON VALUES (?, ?, ?)", use(firstName), use(lastName), use(age), now;
ses << "SELECT (firstname, lastname, age) FROM Person", into(firstName), into(lastName), into(age), now;

std::string firstName("Peter";
Poco::Nullable<std::string> lastName("Junior");
Poco::Nullable<int> age = 0;
ses << INSERT INTO PERSON VALUES (?, ?, ?)", use(firstName), use(lastName), use(age), now;
ses << "SELECT (firstname, lastname, age) FROM Person", into(firstName), into(lastName), into(age), now;
// now you can check if age was null:
if (!lastName.isNull()) { ... }

session << "SELECT * FROM Person", now; // note the absence of target storage

Statement select(session); // we need a Statement for later RecordSet creation
select << "SELECT * FROM Person", now;

// create a RecordSet
RecordSet rs(select);
std::size_t cols = rs.columnCount();

// print all column names
for (std::size_t col = 0; col < cols; ++col)
    std::cout << rs.columnName(col) << std::endl;

// iterate over all rows and columns
for (RecordSet::Iterator it = rs.begin(); it != rs.end(); ++it)
    std::cout << *it << " ";
		

std::copy(rs.begin(), rs.end(), std::ostream_iterator<Row>(std::cout));

std::cout << rs;

Row row;
row.append("Field0", 0);
row.append("Field1", 1);
row.append("Field2", 2);

row.addSortField("Field1"); // now Field0 and Field1 are used for sorting
row.replaceSortField("Field0", "Field2");// now Field1 and Field2 are used for sorting


		*/

/*struct acct
	{
		int id;
		std::string name;
		std::string pw;
	};

	// insert some rows
	acct testacct =
	{
		3,
		"testinsert",
		"testinsert"
	};

	insert << "INSERT INTO account VALUES(?, ?, ?)",
		use(testacct.id),
		use(testacct.name),
		use(testacct.pw);

	std::stringstream testStr;
		testStr << "INSERT INTO account VALUES(?, ?, ?)",
		use(testacct.id),
		use(testacct.name),
		use(testacct.pw);

	AlefQuery query;

	query << "INSERT INTO account VALUES(?, ?, ?)",
		use(testacct.id),
		use(testacct.name),
		use(testacct.pw);

        //dbInterface->runLoginQuery(query);

Session sess = dbInterface->acquireDatabaseSession(AlefDBInterface::dbType::loginDB);

sess << "INSERT INTO account VALUES(?, ?, ?)",
use(testacct.id),
use(testacct.name),
use(testacct.pw), now;

//dbInterface->test();*/