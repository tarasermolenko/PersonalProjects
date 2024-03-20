#include "RedBlackTree.h"

using namespace std;




template <class T, class Y>
void printBT(const std::string& prefix, const NodeT<T,Y>* node, bool isLeft)
{
    if( node != nullptr )
    {
        std::cout << prefix;

        std::cout << (isLeft ? "├──" : "└──" );

        // print the value of the node
		if(node->isBlack == true)
		{
		std::cout << node->key << " B" << std::endl;
		}
		else
		{
			std::cout << node->key << " R" << std::endl;
		}
        // enter the next tree level - left and right branch
        printBT( prefix + (isLeft ? "│   " : "    "), node->left, true);
        printBT( prefix + (isLeft ? "│   " : "    "), node->right, false);
    }
}

template <class T, class Y>
void printBT(const NodeT<T,Y>* node)
{
    printBT("", node, false);    
}

void simpleTest()
{
	// Int Tree Tests	
	RedBlackTree<int, char> rb1;
	if (rb1.insert(6, 'f')) cout << "inserted f" << endl;
	rb1.insert(10, 'j');
	rb1.insert(2, 'b');

	printBT(rb1.getRoot());
	RedBlackTree<int, char> rb2(rb1);
	printBT(rb2.getRoot());

	if (rb1.remove(2)) cout << "removed 2" << endl;
	printBT(rb1.getRoot());
	if (rb1.search(6)) cout << "found 6" << endl;
 
 	cout << endl;
	vector<char> v1 = rb1.search(8, 21); //should contain j

	for(int i = 0; i < v1.size(); i++ )
	{
		cout << v1[i] << endl;
	}
	
	cout << endl;

	vector<char> v2 = rb1.values(); //should contain {f, j}
	vector<int> v3 = rb1.keys(); //should contain {6, 10}
 
	cout << "size = " << rb1.size() << endl; //should be 2
	NodeT<int, char>* pRoot = rb1.getRoot(); //BAD design - for our testing only
}

// void simpleTest()
// {
// 	// Int Tree Tests	
// 	RedBlackTree rb1;
// 	rb1.insert(47,4);
// 	rb1.insert(32,3);
// 	rb1.insert(71,7);
// 	rb1.insert(65,6);
// 	rb1.insert(87,9);
// 	rb1.insert(51,5);
// 	rb1.insert(82,8);
// 	rb1.insert(93,10);
// 	rb1.insert(16,2);
// 	rb1.insert(15,1);
// 	printBT(rb1.getRoot());

// 	cout << endl << endl;

// 	//rb1.remove(71);
// 	//printBT(rb1.getRoot());

// 	vector<int> test = rb1.search(47,93);

// 	for(int i = 0; i < test.size(); i++)
// 	{
// 		cout << test[i] << endl;
// 	}

// 	cout << endl << endl;

// 	vector<int> test2 = rb1.values();

// 	for(int i = 0; i < test2.size(); i++)
// 	{
// 		cout << test2[i] << endl;
// 	}

// 	cout << endl << endl;

// 	vector<int> test3 = rb1.keys();

// 	for(int i = 0; i < test3.size(); i++)
// 	{
// 		cout << test3[i] << endl;
// 	}

// }

int main()
{
	simpleTest();
    return 0;
}