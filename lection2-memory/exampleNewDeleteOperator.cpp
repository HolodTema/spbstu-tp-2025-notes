#include <iostream>

//this class allows to create <= MAX_COUNT instances of self. 

class Foo {
public:
	void* operator new(std::size_t size) {
		if (index >= MAX_COUNT) {
				throw std::bad_alloc();
		}

		void* ptr = std::malloc(size);
		if (!ptr) {
				throw std::bad_alloc();
		}
		ptrs[index++] = ptr;
		return ptr;
	}
		
	void operator delete(void* ptr) {
		int index = std::distance(ptrs.begin(), std::find(ptrs.begin(), ptrs.end(), ptr));
		ptrs[index] = nullptr;
		free(ptr);
	}

	static void printMemoryInfo(std::ostream &os) {
		for (auto p: ptrs) {
				if (p) {
						os << "Address " << p << " is not deallocated\n";
				}
		}
	}

private:
	static int index = 0;
	static const int MAX_COUNT = 10;
	static std::array<void*, MAX_COUNT> ptrs{ nullptr };
};

int main() {
	Foo* foo1 = new Foo;
	Foo::printMemoryInfo(std::cout);
	return 0;
}

