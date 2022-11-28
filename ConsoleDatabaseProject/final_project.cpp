/////////////////////////////////////////////////////////////////////////
//
// Student Info
// ------------
//
// Name : Taras Ermolenko
// St.# : <301448503>
// Email: tea6@sfu.ca
//
//
// Statement of Originality
// ------------------------
//
// All the code and comments below are my own original work. For any non-
// original work, I have provided citations in the comments with enough
// detail so that someone can see the exact source and extent of the
// borrowed work.
//
// In addition, I have not shared this work with anyone else, and I have
// not seen solutions from other students, tutors, websites, books,
// etc.
//
/////////////////////////////////////////////////////////////////////////

#include "Menu.h"

using namespace std;

void run_Database()
{
	cout << "\nLaunching Database ... \n\n\n";
    Menu Regimen("database.txt");
};

int main() 
{	
	// Bootup Art
	cout << "'--------------------------'()'--------------------------'" << endl;
    cout << "                            ||                            " << endl;
    cout << "                            ||                                       ___  "<< endl;
    cout << "                        ---------...._                             .'  /  "<< endl;
    cout << "                   ,-^^^==============`--.                       .'/) /   "<< endl;
    cout << "                ,' ) ,--. .-----.         `.___________________.' // /  "<< endl;
    cout << "              .'  / /___| |_____|  sfu      CMPT-125    _______  () |  "<< endl;
    cout << "             /   / /____| |__|__|             ,----====^       `//  \\   "<< endl;
    cout << "           .<`=='===========================.'                 (/`.  \\  "<< endl;
    cout << "          (  `.----------------------------/                       `._\\ "<< endl;
    cout << "          `-.___              ______...--'                               "<< endl;
    cout << "                \\\\--77-----77^                                         "<< endl;
    cout << "              .____//______//___,                                       "<< endl;
    cout << "              `-----------------'                                       "<< endl;

    // Launches Database
	run_Database();

};