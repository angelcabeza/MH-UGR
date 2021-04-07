#include <iostream>
#include <set>
using namespace std;

int main (){
	set<int> set1;
	set<int> set2;
	bool igual;

	for (int i = 0; i < 3; i++){
		set1.insert(i);
		set2.insert(i);
	}

	igual = set1 != set2;

	cout << igual << endl;
}