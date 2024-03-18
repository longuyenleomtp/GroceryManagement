#include <iostream>
#include <mysql.h>
#include <mysqld_error.h>
//#include <window.h>
#include <sstream>

using namespace std;

const char* HOST = "localhost";
const char* USER = "root";
const char* PW = "PhamLongUyen@1";
const char* DB = "GroceryManagement";

class Customer {
private:
    string id, name, DoB, address;
    int memberPoint;
public:
    Customer() {
        this->id = "";
        this->name = "";
        this->DoB = "";
        this->address = "";
        this->memberPoint = 0;
    };
    Customer(string id, string name, string DoB, string address, int memberPoint) {
        this->id = id;
        this->name = name;
        this->DoB = DoB;
        this->address = address;
        this->memberPoint = memberPoint;
    }
    string getID() {
        return id;
    }
    string getName() {
        return name;
    }
    string getDoB() {
        return DoB;
    }
    string getAddress() {
        return address;
    }
    int getMemberPoint() {
        return memberPoint;
    }
    void setID(string id) {
        this->id = id;
    }
    void setName(string name) {
        this->name = name;
    }
    void setDoB(string DoB) {
        this->DoB = DoB;
    }
    void setAddress(string address) {
        this->address = address;
    }
    void setMemberPoint(int memberPoint) {
        this->memberPoint = memberPoint;
    }
};

class Grocery {
private:
    string id, name, manufac, mfg, exp;
    int quantity;
public:
    Grocery() {
        this->id = "";
        this->name = "";
        this->manufac = "";
        this->mfg = "";
        this->exp = "";
        this->quantity = 0;
    }
    Grocery(string id, string name, string manufac, string mfg, string exp, int quantity) {
        this->id = id;
        this->name = name;
        this->manufac = manufac;
        this->mfg = mfg;
        this->exp = exp;
        this->quantity = quantity;
    }
    string getID() {
        return id;
    }
    string getName() {
        return name;
    }
    string getManufac() {
        return manufac;
    }
    string getMfg() {
        return mfg;
    }
    string getExp() {
        return exp;
    }
    int getQuantity() {
        return quantity;
    }
    void setID(string id) {
        this->id = id;
    }
    void setName(string name) {
        this->name = name;
    }
    void setManufac(string manufac) {
        this->manufac = manufac;
    }
    void setMfg(string mfg) {
        this->mfg = mfg;
    }
    void setExp(string exp) {
        this->exp = exp;
    }
    void setQuantity(int quantity) {
        this->quantity = quantity;
    }
};

void owner(MYSQL* conn, Customer c, Grocery g) {
    bool close = false;
    while(!close) {
        int option;
        cout << "Owner Options: " << endl;
        cout << "1. Add Product into store" << endl;
        cout << "2. Add Customer" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> option;

        if (option == 1) {
            string id, name, manufac, mfg, exp;
            int quantity;

            
            cout << "Product ID: ";
            cin >> id;
            g.setID(id);

            cin.ignore();
            cout << "Product name: ";
            getline(cin, name);
            g.setName(name);

            cin.ignore();
            cout << "Manufacturing Plant: ";
            getline(cin, manufac);
            g.setManufac(manufac);

            cout << "Manufacturing Date: ";
            cin >> mfg;
            g.setMfg(mfg);

            cout << "Expiry Date: ";
            cin >> exp;
            g.setExp(exp);

            cout << "Quantity of Product: ";
            cin >> quantity;
            g.setQuantity(quantity);


            string product = "INSERT INTO grocery (id, name, manufac, mfg, exp, quantity) VALUES('"
            + g.getID() + "', '" + g.getName() + "', '" + g.getManufac() + "', '" + g.getMfg()
            + "', '" + g.getExp() + "', '" + to_string(g.getQuantity()) + "')";

            if (mysql_query(conn, product.c_str())) {
                cout << "Error: " << mysql_error(conn) << endl;
            }
            else {
                cout << "Product addded successfully!" << endl;
            }

        }

        else if (option == 2) {
            string id, name, DoB, address;
            int memberPoint;
            
            
            cout << "Customer ID: ";
            cin >> id;
            c.setID(id);

            cin.ignore();
            cout << "Customer Name: ";
            getline(cin, name);
            c.setName(name);

            cout << "Customer DoB: ";
            cin >> DoB;
            c.setDoB(DoB);

            cin.ignore();
            cout << "Customer Address: ";
            getline(cin, address);
            c.setAddress(address);

            cout << "Member Point (Initial should 0): ";
            cin >> memberPoint;
            c.setMemberPoint(memberPoint);

            string customer = "INSERT INTO customer (id, name, DoB, address, memberPoint) VALUES('" 
                + c.getID() + "','" + c.getName() + "','" + c.getDoB() + "','" + c.getAddress() + "', '" 
                + to_string(c.getMemberPoint()) + "' )";


            if (mysql_query(conn, customer.c_str())) {
                cout << "Error: " << mysql_error(conn) << endl;
            }
            else {
                cout << "Customer added successfully!" << endl;
            }
        }

        else if (option == 0) {
            close = true;
            cout << "System is closing!" << endl;
        }
    }
}

void display(MYSQL* conn) {
    cout << "Available Products: " << endl;
    cout << "--------------------" << endl;
	cout << "Product ID\t" << "||" << "Product Name\t" << "||" << "Manufacturing Plant\t" << "||" << "Manufacturing Date\t\t" << "||" << "Expiry Date\t\t" << "||" << "Quantity\t" << endl;
    string disp = "SELECT * FROM grocery";
    if (mysql_query(conn, disp.c_str())) {
        cout << "Error: " << mysql_error(conn) << endl;
    }
    else {
        MYSQL_RES* res;
        res = mysql_store_result(conn);
        if(res){
        	int num = mysql_num_fields(res);
        	MYSQL_ROW row;
        	while(row = mysql_fetch_row(res)){
        		for(int i = 0; i < num; i++){
        			cout <<  " " << row[i] << "\t\t";
				}
				cout << endl;
			}
			mysql_free_result(res);
		}
    }

}

int grocery(MYSQL* conn, string Gname) {
    string exist = "SELECT id, name, manufac, mfg, exp, quantity FROM grocery WHERE name = '" + Gname + "'";
    if (mysql_query(conn, exist.c_str())) {
        cout << "Error: " << mysql_error(conn) << endl;
    }
    else {
        MYSQL_RES* res;
        res = mysql_store_result(conn);
        if (res) {
            MYSQL_ROW row;
            if ((row = mysql_fetch_row(res)) != NULL) {
                int quantity = atoi(row[5]);
                mysql_free_result(res);
                return quantity;
            }
            else {
                cout << "Grocery not found! " << endl;
            }
        }
    }
    return 0;
}


void customer(MYSQL* conn, Customer c, Grocery g) {
	display(conn);
	string id;
	cout << "Enter your ID: ";
	cin >> id;
	
	string cus = "SELECT * FROM customer WHERE id = '"+ id + "' ";
	if(mysql_query(conn, cus.c_str() )){
		cout << "Error: " << mysql_error(conn) << endl;
	} else {
		MYSQL_RES* res;
		res = mysql_store_result(conn);
		
		if(res){
			int num = mysql_num_rows(res);
			if(num == 1){
				cout << "Customer ID Found!" << endl;
				string Gname;
				cout << "Enter Product Name: ";
				cin.ignore();
				getline(cin, Gname);
				int number;
				cout << "Enter number of Products: ";
				cin >> number;
				if(grocery(conn, Gname)-number > 0){
					int groceryQuantity = grocery(conn,Gname)-number;
					string upd = "UPDATE grocery SET quantity = '"+ to_string(groceryQuantity) +"' WHERE name = '"+ Gname +"' ";
					if(mysql_query(conn, upd.c_str())) {
						cout << "Error: " << mysql_error(conn) << endl;
					} else {
						cout << "Product is Available, Purchasing product....." << endl;
					}
				} else {
					cout << "Not enough " << number << " items. We only have " << grocery(conn,Gname) << " items! Try again!"<< endl;
				}
			} else if(num == 0) {
				cout << "You haven't registered yet!" << endl;
			}
		}
		
		mysql_free_result(res);
	}
	
}


int main()
{
    Customer c;
    Grocery g;

    MYSQL* conn;
    conn = mysql_init(NULL);

    if (!mysql_real_connect(conn, HOST, USER, PW, DB, 3306, NULL, 0)) {
        cout << "Error: " << mysql_error(conn) << endl;
    }
    else {
        cout << "LOGGED IN!" << endl;
    }

    bool exit = false;
    while (!exit) {
        int option;
        cout << endl;
        cout << "==============Welcome to the Grocery Management System==============" << endl;
        cout << "*******************************MENU*********************************" << endl;
        cout << "1. Owner" << endl;
        cout << "2. Customer" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> option;

        if (option == 1) {
            owner(conn, c, g);
        }
        else if (option == 2) {
            customer(conn, c, g);
        }
        else if (option == 0) {
            exit = true;
            cout << "System is closing!" << endl;
        }
    }

    mysql_close(conn);


    return 0;
}

