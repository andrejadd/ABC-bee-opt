#include <vector>

#include <iostream>

using namespace std;

int sum(vector<int> in) {
	int ret=0;
	for (int i = 0; i < in.size(); i++) {
		ret += in[i];
	}	
	return ret;
}

int main() {
	vector<int> v;
	v.push_back(1);
	v.push_back(3);
	v.push_back(5);
	cout << sum(v) << endl;
}