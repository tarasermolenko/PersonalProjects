#include <iostream>      
#include <fstream>     
#include <string>       
#include <algorithm> 
#include <cassert>
#include <sstream> 
#include <iomanip>    

#include "Solider.h"

using namespace std;

class Database {
private:
	int arr_capacity; // length of underlying array
    solider* arr;     // pointer to the underlying array
    int arr_size;     // # of elements in this int_vec from user's perspective

public:

    string databasename;

	// Gets rid of magic number 
	int static const initial_capacity = 10;

	Database() 
    : arr_capacity(initial_capacity), arr(new solider[arr_capacity]), arr_size(0)
    { 
    	

    	for(int i = 0; i < arr_capacity; i++) 
        {
            arr[i].id = 0;
            arr[i].first_name = "unassigned";
            arr[i].last_name = "unassigned";
            arr[i].age = -1;
            arr[i].status = "unassigned";
            arr[i].rank = "unassigned";

        }
    }

    // sstream tutorial reference:
    //https://www.youtube.com/watch?v=_IzYGiuX8QM&t=466s&ab_channel=CUBoulderDataStructuresCSCI2270
    Database(string fname): Database()
    {
        databasename = fname;

        ifstream theFile(fname);
        if(!theFile.is_open())
        {
        	cout << "File Failed to open" << endl;
        }

        string first_name, last_name, status, rank, tempString, line;
	    int id, age;

        while(getline(theFile, line))
        {   
            stringstream ss(line);
            getline(ss, tempString, ',');
            id = stoi(tempString);
            getline(ss, first_name, ',');
            getline(ss, last_name, ',');
            getline(ss, tempString, ',');
            age = stoi(tempString);
            getline(ss, status, ',');
            getline(ss, rank, ',');

            append(id, first_name, last_name, age, status, rank);
        }


        theFile.close();
    }

    ~Database()
	{
		delete[] arr;
        // printing just to validate that it's working
		cout << "\nDatabase destructor called\n";
	}

	void append(int id, string first_name, string last_name, int age, string status, string rank) 
    {
       if (arr_size >= arr_capacity) 
       {
            // double the capacity
            arr_capacity = 2 * arr_capacity;  

            // make new underlying array twice size of old one   
            solider* new_arr = new solider[arr_capacity];                        
        
            for(int i = 0; i < arr_size; ++i) 
            {   
                // copy elements into new_arr
                new_arr[i].first_name = arr[i].first_name;
                new_arr[i].last_name = arr[i].last_name;
           		new_arr[i].age = arr[i].age;
           		new_arr[i].status = arr[i].status;
           		new_arr[i].rank = arr[i].rank;
           		new_arr[i].id = arr[i].id;           
            }
        
            // delete old arr
            delete[] arr;                     
        
            // assign new_arr
            arr = new_arr;                    

            for(int i = arr_size; i < arr_capacity; ++i) 
            {   
                // set empty slots to zero into new_arr
                 arr[i].first_name = "unassigned";
                 arr[i].last_name = "unassigned";
		         arr[i].age = -1;
		         arr[i].status = "unassigned";
		         arr[i].rank = "unassigned";
		         arr[i].id = 0;          
            }

       }
       
       arr[arr_size].first_name = first_name;
       arr[arr_size].last_name = last_name;
       arr[arr_size].age = age;
       arr[arr_size].status = status;
       arr[arr_size].rank = rank;
       arr[arr_size].id = id;
       arr_size++;

    
   }

   // need print by ID and albhabetical name
   void print() const 
    {
        if (arr_size == 0) {
            cout << "{}";
        } else 
        {
            cout << "\n";
            cout << "----------------------------------------";
            cout <<"----------------------------------------";
            cout << endl;
            cout << "   ID   |  FIRST NAME  |   LAST NAME  ";
            cout << "|      AGE     |   STATUS     |     RANK    "; 
            cout << endl;
            cout << "-----------------------------------------";
            cout << "---------------------------------------"; 
            cout << endl;
            for (int i = 0; i < arr_size; ++i) 
            {  // i starts at 1 (not 0) 
                cout << setw(10) << to_string(arr[i].id) + " | "  
                << setw(15) << arr[i].first_name + " | "  
                << setw(15) << arr[i].last_name + " | "  
                << setw(15) << to_string(arr[i].age) + " | "  
                << setw(15) << arr[i].status + " | "  
                << setw(10) << arr[i].rank << endl;
            }

            cout << "\n";
        }
    }

    // write to file function
    void update_database()
    {
        ofstream theFile(databasename);
        if(!theFile.is_open())
        {
            cout << "File Failed to open" << endl;
        }

        //theFile.open("data.txt", std::ofstream::out | std::ofstream::trunc);

        for (int i = 0; i < arr_size; i++)
        {
            theFile << to_string(arr[i].id) + "," + arr[i].first_name 
            + "," + arr[i].last_name + "," 
            + to_string(arr[i].age) + "," + arr[i].status + "," 
            + arr[i].rank + ","  + "\n";
        }

        theFile.close();

    }

    // checks if person or id already exist in database
    int check_database(int id, string first_name, string last_name) 
    {
        for (int i = 0; i < arr_size; i++)
        {
            if(arr[i].id == id && arr[i].first_name == first_name && arr[i].last_name == last_name)
            {
                return 1;
            }
            else if(arr[i].id == id)
            {
                return 2;
            }
        }

        return 0;
    }

    int check_database_id(int id) 
    {
        for (int i = 0; i < arr_size; i++)
        {
            if(arr[i].id == id)
            {
                return 1;
            }
        }

        return 0;
    }

    int check_database_name(string first_name, string last_name) 
    {
        for (int i = 0; i < arr_size; i++)
        {
            if(arr[i].first_name == first_name && arr[i].last_name == last_name)
            {
                return 1;
            }
        }

        return 0;
    }

    void find_by_id(int id)
    {
        int notfound = 1;

        for (int i = 0; i < arr_size; i++)
        {
            if(arr[i].id == id)
            {
                cout << "----------------------------------------";
                cout <<"----------------------------------------";
                cout << endl;
                cout << "   ID   |  FIRST NAME  |   LAST NAME  ";
                cout << "|      AGE     |   STATUS     |     RANK    "; 
                cout << endl;
                cout << "-----------------------------------------";
                cout << "---------------------------------------"; 
                cout << endl;
                cout << setw(10) << to_string(arr[i].id) + " | "  
                << setw(15) << arr[i].first_name + " | "  
                << setw(15) << arr[i].last_name + " | "  
                << setw(15) << to_string(arr[i].age) + " | "  
                << setw(15) << arr[i].status + " | "  
                << setw(10) << arr[i].rank << endl;
                notfound = 0;
            }  
        }
        if(notfound)
        {
            cout << "\n\n-----------------------------\n";
            cout << "Agent with that ID does not exist\n";
            cout << "---------------------------------\n\n";
        }
    }

    void find_by_name(string first_name, string last_name)
    {
        int notfound = 1;

        for (int i = 0; i < arr_size; i++)
        {
            if(arr[i].first_name == first_name && arr[i].last_name == last_name)
            {
                cout << "----------------------------------------";
                cout <<"----------------------------------------";
                cout << endl;
                cout << "   ID   |  FIRST NAME  |   LAST NAME  ";
                cout << "|      AGE     |   STATUS     |     RANK    "; 
                cout << endl;
                cout << "-----------------------------------------";
                cout << "---------------------------------------"; 
                cout << endl;
                cout << setw(10) << to_string(arr[i].id) + " | "  
                << setw(15) << arr[i].first_name + " | "  
                << setw(15) << arr[i].last_name + " | "  
                << setw(15) << to_string(arr[i].age) + " | "  
                << setw(15) << arr[i].status + " | "  
                << setw(10) << arr[i].rank << endl;
                notfound = 0;
                cout << endl << endl;
            }  
        }
        if(notfound)
        {
            cout << "\n\n------------------------------------\n";
            cout << "Agent with that name does not exist\n";
            cout << "------------------------------------\n\n";
        }
    }

    void find_by_age_range(int lower, int upper)
    {
        int notfound = 1;
        cout << "----------------------------------------";
        cout <<"----------------------------------------";
        cout << endl;
        cout << "   ID   |  FIRST NAME  |   LAST NAME  ";
        cout << "|      AGE     |   STATUS     |     RANK    "; 
        cout << endl;
        cout << "-----------------------------------------";
        cout << "---------------------------------------"; 
        cout << endl;

        for (int i = 0; i < arr_size; i++)
        {
            if(arr[i].age >= lower && arr[i].age <= upper)
            {
                cout << setw(10) << to_string(arr[i].id) + " | "  
                << setw(15) << arr[i].first_name + " | "  
                << setw(15) << arr[i].last_name + " | "  
                << setw(15) << to_string(arr[i].age) + " | "  
                << setw(15) << arr[i].status + " | "  
                << setw(10) << arr[i].rank << endl;
                notfound = 0;
            }  
        }
        if(notfound)
        {
            cout << "\n\n------------------------------------\n";
            cout << "Error no agents found within "; 
            cout << to_string(lower) + " to " + to_string(upper);
            cout <<" age range\n";
            cout << "------------------------------------\n\n";
        }
    }

    int find_substring_fname(string substr)
    {
        int notfound = 1;
        cout << "\n";
        cout << "----------------------------------------";
        cout <<"----------------------------------------";
        cout << endl;
        cout << "   ID   |  FIRST NAME  |   LAST NAME  ";
        cout << "|      AGE     |   STATUS     |     RANK    "; 
        cout << endl;
        cout << "-----------------------------------------";
        cout << "---------------------------------------"; 
        cout << endl;

        for (int i = 0; i < arr_size; i++)
        {
            if((arr[i].first_name).find(substr) != -1 || arr[i].first_name == substr)
            {
                cout << setw(10) << to_string(arr[i].id) + " | "  
                << setw(15) << arr[i].first_name + " | "  
                << setw(15) << arr[i].last_name + " | "  
                << setw(15) << to_string(arr[i].age) + " | "  
                << setw(15) << arr[i].status + " | "  
                << setw(10) << arr[i].rank << endl;
                notfound = 0;
            }  
        }
        if(notfound)
        {
            cout << "\n----------------------------------------------------\n";
            cout << "No agents found with \"" + substr + "\" in first name";
            cout << "\n----------------------------------------------------\n\n";
            return 0;
        }
        cout << "\n\n";
        return 1;
    }

    void remove_by_id(int id)
    {
        int notfound = 1;
        int found_index = -1;

        for (int i = 0; i < arr_size; i++)
        {
            // if element is found, old array is over written without the element.
            if(arr[i].id == id)
            {
                found_index = i;
                solider* new_arr = new solider[arr_capacity];
                for (int j = 0; j < found_index; j++)
                {
                    new_arr[j].first_name = arr[j].first_name;
                    new_arr[j].last_name = arr[j].last_name;
                    new_arr[j].age = arr[j].age;
                    new_arr[j].status = arr[j].status;
                    new_arr[j].rank = arr[j].rank;
                    new_arr[j].id = arr[j].id; 
                }
                for (int j = found_index; j < arr_size-1; j++)
                {
                    new_arr[j].first_name = arr[j+1].first_name;
                    new_arr[j].last_name = arr[j+1].last_name;
                    new_arr[j].age = arr[j+1].age;
                    new_arr[j].status = arr[j+1].status;
                    new_arr[j].rank = arr[j+1].rank;
                    new_arr[j].id = arr[j+1].id; 
                }

                delete[] arr;
                arr = new_arr;
                arr_size--;
                arr_capacity++;

                notfound = 0;
            }  
        }
        if(notfound)
        {
            cout << "\n\n-----------------------------\n";
            cout << "Error agent not Deleted, invalid ID\n";
            cout << "---------------------------------\n\n";
        }
    }

    void remove_by_fname_substring(string substr)
    {
        int notfound = 1;
        int found_index = -1;

        for (int i = 0; i < arr_size; i++)
        {
            // if substring is found, old array is over written without the element.
            if((arr[i].first_name).find(substr) != -1 || arr[i].first_name == substr)
            {
                found_index = i;
                solider* new_arr = new solider[arr_capacity];
                for (int j = 0; j < found_index; j++)
                {
                    new_arr[j].first_name = arr[j].first_name;
                    new_arr[j].last_name = arr[j].last_name;
                    new_arr[j].age = arr[j].age;
                    new_arr[j].status = arr[j].status;
                    new_arr[j].rank = arr[j].rank;
                    new_arr[j].id = arr[j].id; 
                }
                for (int j = found_index; j < arr_size-1; j++)
                {
                    new_arr[j].first_name = arr[j+1].first_name;
                    new_arr[j].last_name = arr[j+1].last_name;
                    new_arr[j].age = arr[j+1].age;
                    new_arr[j].status = arr[j+1].status;
                    new_arr[j].rank = arr[j+1].rank;
                    new_arr[j].id = arr[j+1].id; 
                }

                delete[] arr;
                arr = new_arr;
                arr_size--;
                arr_capacity++;

                notfound = 0;
            }  
        }
        if(notfound)
        {
            cout << "\n\n-----------------------------\n";
            cout << "Error agent not Deleted, invalid age range\n";
            cout << "---------------------------------\n\n";
        }
    }

    void remove_by_age(int lower, int upper)
    {
        int notfound = 1;
        int found_index = -1;

        for (int i = 0; i < arr_size; i++)
        {
            // if in age range limit, old array is over written without the element.
            if(arr[i].age >= lower && arr[i].age <= upper)
            {
                found_index = i;
                solider* new_arr = new solider[arr_capacity];
                for (int j = 0; j < found_index; j++)
                {
                    new_arr[j].first_name = arr[j].first_name;
                    new_arr[j].last_name = arr[j].last_name;
                    new_arr[j].age = arr[j].age;
                    new_arr[j].status = arr[j].status;
                    new_arr[j].rank = arr[j].rank;
                    new_arr[j].id = arr[j].id; 
                }
                for (int j = found_index; j < arr_size-1; j++)
                {
                    new_arr[j].first_name = arr[j+1].first_name;
                    new_arr[j].last_name = arr[j+1].last_name;
                    new_arr[j].age = arr[j+1].age;
                    new_arr[j].status = arr[j+1].status;
                    new_arr[j].rank = arr[j+1].rank;
                    new_arr[j].id = arr[j+1].id; 
                }

                delete[] arr;
                arr = new_arr;
                arr_size--;
                arr_capacity++;

                notfound = 0;
            }  
        }
        if(notfound)
        {
            cout << "\n\n-----------------------------\n";
            cout << "Error agent not Deleted, invalid age range\n";
            cout << "---------------------------------\n\n";
        }
    }

    void remove_by_name(string first_name, string last_name)
    {
        int notfound = 1;
        int found_index = -1;

        for (int i = 0; i < arr_size; i++)
        {
            // if name is found, old array is over written without the element.
            if(arr[i].first_name == first_name && arr[i].last_name == last_name )
            {
                found_index = i;
                solider* new_arr = new solider[arr_capacity];
                for (int j = 0; j < found_index; j++)
                {
                    new_arr[j].first_name = arr[j].first_name;
                    new_arr[j].last_name = arr[j].last_name;
                    new_arr[j].age = arr[j].age;
                    new_arr[j].status = arr[j].status;
                    new_arr[j].rank = arr[j].rank;
                    new_arr[j].id = arr[j].id; 
                }
                for (int j = found_index; j < arr_size-1; j++)
                {
                    new_arr[j].first_name = arr[j+1].first_name;
                    new_arr[j].last_name = arr[j+1].last_name;
                    new_arr[j].age = arr[j+1].age;
                    new_arr[j].status = arr[j+1].status;
                    new_arr[j].rank = arr[j+1].rank;
                    new_arr[j].id = arr[j+1].id; 
                }

                delete[] arr;
                arr = new_arr;
                arr_size--;
                arr_capacity++;

                notfound = 0;
            }  
        }
        if(notfound)
        {
            cout << "\n\n-----------------------------\n";
            cout << "Error agent not Deleted, invalid ID\n";
            cout << "---------------------------------\n\n";
        }
    }

    // basic swap method learned in 120.
    void sort_by_id_ascend()
    {
        int i, j, min_idx;
        for (i = 0; i < arr_size-1; i++)
        {
            // Find the minimum element in unsorted array
            min_idx = i;
            for (j = i+1; j < arr_size; j++)
            {
                if (arr[j].id < arr[min_idx].id)
                {
                    min_idx = j;
                }
            } 
            // Swap the found minimum element with the first element
            swap(arr[min_idx].id, arr[i].id);
            swap(arr[min_idx].first_name, arr[i].first_name);
            swap(arr[min_idx].last_name, arr[i].last_name);
            swap(arr[min_idx].age, arr[i].age);
            swap(arr[min_idx].status, arr[i].status);
            swap(arr[min_idx].rank, arr[i].rank);
        }
    }

    void sort_by_id_descend()
    {
        int i, j, min_idx;
        for (i = 0; i < arr_size-1; i++)
        {
            // Find the minimum element in unsorted array
            min_idx = i;
            for (j = i+1; j < arr_size; j++)
            {
                if (arr[j].id > arr[min_idx].id)
                {
                    min_idx = j;
                }
            } 
            // Swap the found minimum element with the first element
            swap(arr[min_idx].id, arr[i].id);
            swap(arr[min_idx].first_name, arr[i].first_name);
            swap(arr[min_idx].last_name, arr[i].last_name);
            swap(arr[min_idx].age, arr[i].age);
            swap(arr[min_idx].status, arr[i].status);
            swap(arr[min_idx].rank, arr[i].rank);
        }
    }
    void sort_by_fname_ascend()
    {

        int i, j, min_idx;
        for (i = 0; i < arr_size-1; i++)
        {
            // Find the minimum element in unsorted array
            min_idx = i;
            for (j = i+1; j < arr_size; j++)
            {
                if (arr[j].first_name < arr[min_idx].first_name)
                {
                    min_idx = j;
                }
            } 
            // Swap the found minimum element with the first element
            swap(arr[min_idx].id, arr[i].id);
            swap(arr[min_idx].first_name, arr[i].first_name);
            swap(arr[min_idx].last_name, arr[i].last_name);
            swap(arr[min_idx].age, arr[i].age);
            swap(arr[min_idx].status, arr[i].status);
            swap(arr[min_idx].rank, arr[i].rank);
        }
    }

    void sort_by_fname_descend()
    {

        int i, j, min_idx;
        for (i = 0; i < arr_size-1; i++)
        {
            // Find the minimum element in unsorted array
            min_idx = i;
            for (j = i+1; j < arr_size; j++)
            {
                if (arr[j].first_name > arr[min_idx].first_name)
                {
                    min_idx = j;
                }
            } 
            // Swap the found minimum element with the first element
            swap(arr[min_idx].id, arr[i].id);
            swap(arr[min_idx].first_name, arr[i].first_name);
            swap(arr[min_idx].last_name, arr[i].last_name);
            swap(arr[min_idx].age, arr[i].age);
            swap(arr[min_idx].status, arr[i].status);
            swap(arr[min_idx].rank, arr[i].rank);
        }
    }

}; 