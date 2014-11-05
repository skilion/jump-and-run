#include <string>
#include <iostream>

#include "thread.h"
#include "win_thread.h"

using namespace std;
using namespace myengine;

class MyThread: public Runnable
{
public:
	string name;

	MyThread(const char *name) {
		this->name = name;
	}

	void run() {
		for(int i = 0; i < 10; i++) {
			for(int j = 0; j < 10000; j++) {
				for(int k = 0; k < 10000; k++) {
				}
			}
			yield();
			cout << name << ": " << i << "\n";
		}
		cout << "Fine thread " << name << "\n";
	}
};

int main()
{
	{
		Thread *a = createThread(new MyThread("a"));
		Thread *b = createThread(new MyThread("b"));
		Thread *c = createThread(new MyThread("c"));

		a->start();
		b->start();
		c->start();
		
		//c.kill();

		a->join();
		b->join();
		c->join();

		cin.ignore();
	}
	cin.ignore();

	return 0;
}