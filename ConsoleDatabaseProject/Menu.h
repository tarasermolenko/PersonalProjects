#include <iostream>      
#include <fstream>     
#include <string>       
#include <algorithm> 
#include <cassert>     

#include "database.h"

using namespace std;


class Menu : public Database
{

public:
	// add a solider
	Menu(string fname): Database(fname)
	{

		int run = 1;
		string command;

		while(run)
		{
			cout << "Welcome to the Army Database!" << endl;
			cout << "-----------------------------" << endl;
			cout << endl << endl;
			cout << "(A)dd a solider" << endl;
			cout << "(F)ind a solider" << endl;
			cout << "(D)elete a solider" << endl;
			cout << "(L)ist soliders" << endl;
			cout << "(Q)uit" << endl << endl;

			cout << "Enter the letter of your choice: " << endl;
			cin >> command;

			// all comands will be set to lower case as to avoid dealing with
			// upper case strings
			transform(command.begin(), command.end(), command.begin(), ::tolower);


			if(command == "l" || command == "list" || command == "list soliders")
			{
				list_soliders();
			}
			else if(command == "f" || command == "find")
			{
				find_solider();
			}
			else if(command == "d" || command == "delete")
			{
				delete_solider();
			}
			else if(command == "q" || command == "quit")
			{
				update_database();
				run = 0;
			}
			else if(command == "a" || command == "add a solider")
			{
				add_solider();
			} 
			else 
			{
				cout << "\n\n------------------------------------------\n";
				cout << "Error, Invalid Command, Please try again.\n";
				cout << "-----------------------------------------\n\n";
			}

		}
	}

	void delete_solider()
	{
		cout << "------------------" << endl;
		cout << "\n\nDelete a Solider" << endl;
		cout << "------------------" << endl << endl;
		int run_find = 1;
		string find_command;

		while(run_find)
		{
			cout << "\nYou delete by: " << endl << endl;
			cout << "(I)D nnumber of a solider" << endl;
			cout << "(N)ame of a solider" << endl;
			cout << "(A)ge range" << endl;
			cout << "(S)ubstring of firstname" << endl;

			cout << "\n(R)eturn to main menu\n" << endl;

			cout << "Enter the letter of your choice: " << endl;
			cin >> find_command;

			transform(find_command.begin(), find_command.end(), find_command.begin(), ::tolower);
			if(find_command == "r" || find_command == "return")
			{
				run_find = 0;
			}
			else if(find_command == "i" ||find_command == "id")
			{
				int search_id;
				string delete_confirm;
				cout << "Please enter ID of agent you want to delete: ";
				cin >> search_id;
				while(1)
				{
					if(cin.fail())
					{
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(),'\n');
						cout << "Improper input, ID has to be a number" << endl;
						cout << "Enter ID of agent you are looking for ";
						cin >> search_id;
					}
					if(!cin.fail())
					{

						if(check_database_id(search_id) == 1)
						{
							find_by_id(search_id);
							cout << "Do you wish to procced with";
							cout <<	" deleting agent? (Y)es/(N)o: ";
							cin >> delete_confirm;
							transform(delete_confirm.begin(), delete_confirm.end(), 
								delete_confirm.begin(), ::tolower);
							if(delete_confirm == "y" || delete_confirm == "yes")
							{
								remove_by_id(search_id);
								//validate deletiion
								if(check_database_id(search_id) == 0)
								{
									cout << "\n----------------------------------\n";
									cout << "Sucess, agent with id " + to_string(search_id) + 
									" has been deleted!";
									cout << "\n----------------------------------\n";
								}
								else
								{
									cout << "\n-------------------------------\n";
									cout << "Error has occured, delete failed";
									cout << "\n--------------------------------\n";
								}
								break;
							}
							else if(delete_confirm == "n" || delete_confirm == "no")
							{
								break;
							}
						}

					}
				}
			}
			else if(find_command == "n" || find_command == "name")
			{
				string search_first_name;
				string search_last_name;
				string delete_confirm;
				cout << "Please enter first name of agent you are looking for: ";
				cin >> search_first_name;
				cout << "Please enter last name of agent you are looking for: ";
				cin >> search_last_name;

				transform(search_first_name.begin(), search_first_name.end(),
				search_first_name.begin(), ::tolower);
				transform(search_last_name.begin(), search_last_name.end(),
				search_last_name.begin(), ::tolower);

				if(check_database_name(search_first_name, search_last_name) == 1)
				{
					find_by_name(search_first_name, search_last_name);
					cout << "Do you wish to procced with deleting";
					cout <<	" this agent? (Y)es/(N)o: ";
					cin >> delete_confirm;
					transform(delete_confirm.begin(), delete_confirm.end(), 
						delete_confirm.begin(), ::tolower);
					if(delete_confirm == "y" || delete_confirm == "yes")
					{
						remove_by_name(search_first_name, search_last_name);
								
						//validate deletiion by check that element no longer exist
						if(check_database_name(search_first_name, search_last_name) == 0)
						{
							cout << "\n--------------------------------------------\n";
							cout << "Sucess, agent" + search_first_name + " " + search_last_name + 
							" has been deleted!";
							cout << "\n--------------------------------------------\n";
						}
						else
						{
							cout << "\n--------------------------------\n";
							cout << "Error has occured, delete failed";
							cout << "\n--------------------------------\n";
						}
						break;
					}
					else if(delete_confirm == "n" || delete_confirm == "no")
					{
						break;
					}	
				}
				else
				{
					cout << "Error person not found, delete failed";
				}
			}
			else if(find_command == "s" || find_command == "substring")
			{
				string substr;
				string delete_confirm;
				cout << "Please enter substring you are wanting to delete: ";
				cin >> substr;

				transform(substr.begin(), substr.end(),
				substr.begin(), ::tolower);

				if(find_substring_fname(substr) == 1)
				{
					cout << "Do you wish to procced with deleting";
					cout <<	" this agent? (Y)es/(N)o: ";
					cin >> delete_confirm;

					transform(delete_confirm.begin(), delete_confirm.end(), 
						delete_confirm.begin(), ::tolower);
					
					if(delete_confirm == "y" || delete_confirm == "yes")
					{
						remove_by_fname_substring(substr);
						
						cout << "\nLooking if any agents with \"" + substr + 
							"\" in first name remain in database...";
						//validate deletiion by check that element no longer exist
						if(find_substring_fname(substr) == 0)
						{
							cout << "\n--------------------------------------------\n";
							cout << "Sucess, agents successfully deleted";
							cout << "\n--------------------------------------------\n\n";
						}
						else
						{
							cout << "\n--------------------------------\n";
							cout << "Error has occured, delete failed";
							cout << "\n--------------------------------\n";
						}
						break;
					}
					else if(delete_confirm == "n" || delete_confirm == "no")
					{
						break;
					}	
				}
				else
				{
					cout << "Error person not found, delete failed";
				}
			}
			else if(find_command == "a" || find_command == "age")
			{
				int lower;
				int upper;
				string delete_confirm;
				cout << "Please enter lower age limit: ";
				cin >> lower;
				if(cin.fail() || lower < 18)
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(),'\n');
					cout << "Error, age has a number and atleast 18" << endl;
					cout << "Enter lower limit of age youre looking for: ";
					cin >> lower;
				}
				cout << "Please enter upper age limit: ";
				cin >> upper;
				if(cin.fail() || upper > 99 || upper < lower)
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(),'\n');
					cout << "Error, age has a number and at most 99";
					cout << " and greater than lower limit" << endl;
					cout << "Enter upper limit of age youre looking for: ";
					cin >> upper;
				}
				if(!cin.fail())
				{
					find_by_age_range(lower, upper);
				}
				cout << "Do you wish to procced with deleting all agents within range?: ";
				cout <<	" this agent? (Y)es/(N)o: ";
				cin >> delete_confirm;
				transform(delete_confirm.begin(), delete_confirm.end(), 
					delete_confirm.begin(), ::tolower);
				if(delete_confirm == "y" || delete_confirm == "yes")
				{
					remove_by_age(lower, upper);
							
			
					cout << "\n--------------------------------\n";
					cout << "Agents have been deleted from database";
					cout << "\n--------------------------------\n";
					break;
				}
				else if(delete_confirm == "n" || delete_confirm == "no")
				{
					break;
				}	
			}
			else 
			{
				cout << "\n\n------------------------------------------\n";
				cout << "Error, Invalid Command, Please try again.\n" << endl;
				cout << "-----------------------------------------\n\n";
			}
		}		
		
	}

	void add_solider()
	{
		cout << "\n\n----------------------------" << endl;
		cout << "      Adding new solider    " << endl;
		cout << "-----------------------------\n\n" << endl;

		int id;
	    string first_name;
	    string last_name;
	    int age;
	    string status;
	    string rank;

	    // validate if ID already exist or not validate proper input ID
	    // and age cant be a string must be ints only
	    // https://www.hackerearth.com/practice/notes/validating-user-input-in-c/
	    cout << "Enter ID: ";
	    cin >> id;
	    while(1)
		{
			if(cin.fail())
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(),'\n');
				cout << "Improper input, ID has to be a number" << endl;
				cout << "Enter ID: ";
				cin >> id;
			}
			if(!cin.fail())
			{
				break;
			}
		}

		// STILL NEED TO VALIDATE STRING INPUT YO
	    cout << "Enter First Name: ";
	    cin >> first_name;

	    cout << "Enter Last Name : ";
	    cin >> last_name;

		transform(first_name.begin(), first_name.end(),
			first_name.begin(), ::tolower);
		transform(last_name.begin(), last_name.end(),
			last_name.begin(), ::tolower);

	    cout << "Enter Age: ";
	    cin >> age;
	    while(1)
		{
			if(cin.fail() || age < 18 || age > 90)
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(),'\n');
				cout << "Improper input, age has to be a number between 18 and 90" << endl;
				cout << "Enter Age: ";
				cin >> age;
			}
			if(!cin.fail())
			{
				break;
			}
		}

	    cout << "Enter Status: ";
	    cin >> status;

	    cout << "Enter Rank: ";
	    cin >> rank;

	    transform(status.begin(), status.end(),
			status.begin(), ::tolower);
		transform(rank.begin(), rank.end(),
			rank.begin(), ::tolower);

	    // BEFORE APPENDING CHECK IF ID AND NAME ALREADY EXIST
	    // IF ID & FNAME & LNAME already exist do not add
	    if(check_database(id, first_name, last_name) == 0)
	    {	
			append(id, first_name, last_name, age, status,rank);
			cout << "\n\n-------------------------\n";
			cout << "Agent Successfully added!\n";
			cout << "-------------------------\n\n";
	    } 
	    else if(check_database(id, first_name, last_name) == 1)
	    {
	    	cout << "\n\n-----------------------------------------------------------\n";
	    	cout << "Error, Agent this agent is already in the database!\n";
	    	cout << "-----------------------------------------------------------\n\n";
	    } 
	    else if(check_database(id, first_name, last_name) == 2)
	    {
	    	cout << "\n\n-------------------------------------------------------------\n";
	    	cout << "Error, duplicate ID. Please provide an ID that is not in use!\n";
	    	cout << "-------------------------------------------------------------\n\n";
	    } 
	    else 
	    {
	    	cout << "\n\n-----------------------------------------------------------\n";
	    	cout << "Unkown Error occured, Agent not added!\n";
	    	cout << "-----------------------------------------------------------\n\n";
	    }
	}

	void find_solider()
	{
		cout << "------------------" << endl;
		cout << "\nFind a Solider" << endl;
		cout << "------------------" << endl << endl;

		int run_find = 1;
		string find_command;

		while(run_find)
		{
			cout << "\nYou can search by: " << endl << endl;
			cout << "(I)D number of a solider" << endl;
			cout << "(N)ame of a solider" << endl;
			cout << "(A)ge range" << endl;
			cout << "(F)irst name contains string" << endl;
			// search by status not implemented, do as substring.
			//cout << "(S)tatus of a solider" << endl << endl;
			cout << "\n(R)eturn to main menu\n" << endl;

			cout << "Enter the letter of your choice: " << endl;
			cin >> find_command;

			transform(find_command.begin(), find_command.end(),
				find_command.begin(), ::tolower);
			if(find_command == "r" || find_command == "return")
			{
				run_find = 0;
			}
			else if(find_command == "i" || find_command == "id")
			{
				int search_id;
				cout << "Please enter ID of agent you are looking for: ";
				cin >> search_id;
				while(1)
				{
					if(cin.fail())
					{
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(),'\n');
						cout << "Improper input, ID has to be a number" << endl;
						cout << "Enter ID of agent you are looking for ";
						cin >> search_id;
					}
					if(!cin.fail())
					{
						find_by_id(search_id);
						break;
					}
				}
			}
			else if(find_command == "n" || find_command == "name")
			{
				string search_first_name;
				string search_last_name;
				cout << "Please enter first name of agent you are looking for: ";
				cin >> search_first_name;
				cout << "Please enter last name of agent you are looking for: ";
				cin >> search_last_name;

				// set input to lower case 
				transform(search_first_name.begin(), search_first_name.end(),
				search_first_name.begin(), ::tolower);

				transform(search_last_name.begin(), search_last_name.end(),
				search_last_name.begin(), ::tolower);

				find_by_name(search_first_name, search_last_name);
			}
			else if(find_command == "a" || find_command == "age")
			{
				int upper;
				int lower;
				cout << "Please enter lower age limit: ";
				cin >> lower;
				if(cin.fail() || lower < 18)
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(),'\n');
					cout << "Error, age has a number and atleast 18" << endl;
					cout << "Enter lower limit of age youre looking for: ";
					cin >> lower;
				}
				cout << "Please enter upper age limit: ";
				cin >> upper;
				if(cin.fail() || upper > 99 || upper < lower)
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(),'\n');
					cout << "Error, age has a number and at most 99";
					cout << " and greater than lower limit" << endl;
					cout << "Enter upper limit of age youre looking for: ";
					cin >> upper;
				}
				if(!cin.fail())
				{
					cout << "\n";
					cout << "Agents found within "; 
		            cout << to_string(lower) + " to " + to_string(upper);
		            cout <<" age range: \n";
					find_by_age_range(lower, upper);
				}
			}
			else if(find_command == "f" || find_command == "first")
			{
				string substring;
				cout << "Please enter substring you are looking for: ";
				cin >> substring;
				transform(substring.begin(), substring.end(),
				substring.begin(), ::tolower);
				find_substring_fname(substring);
			}
			else 
			{
				cout << "\n\n------------------------------------------\n";
				cout << "Error, Invalid Command, Please try again.\n" << endl;
				cout << "-----------------------------------------\n\n";
			}
		}
	}

	void list_soliders()
	{
		int run_list = 1;
		
		while(run_list)
		{
			string list_command;
			cout << "\n";
			cout << "--------------------\n";
			cout << "Chose a List Option:\n";
			cout << "--------------------\n\n";
			cout << "(A) by ID in ascending order\n";
			cout << "(B) by ID in descending order\n";
			cout << "(C) by first name in ascending order\n";
			cout << "(D) by first name in descending order\n";
			cout << "\n(R)eturn to main menu\n";
			cin >> list_command;
			
			transform(list_command.begin(), list_command.end(),
				list_command.begin(), ::tolower);
			
			if(list_command == "r")
			{
				run_list = 0;
				cout << "\n";
			}
			else if(list_command == "a")
			{
				sort_by_id_ascend();
				print();
			}
			else if(list_command == "b")
			{
				sort_by_id_descend();
				print();
			}
			else if(list_command == "c")
			{
				sort_by_fname_ascend();
				print();
			}
			else if(list_command == "d")
			{
				sort_by_fname_descend();
				print();
			}
			else
			{
				cout << "\nThat is not a valid command, try again\n";
			}
		}
	}

};

