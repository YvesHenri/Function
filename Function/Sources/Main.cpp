#include <stdio.h>

#include "..\Includes\Function.h"

class DummyClass
{
public:
	int meep(int i) { return i; }
	int moop() { }
};

struct DummyStruct
{
	int meep(int i) { return i; }
};

int meep(int i)
{
	return i;
}

int bleh(int i)
{
	return i;
}

int main()
{
	fn::Function<int(int)> meepFn(&meep);
	fn::Function<int(int)> blehFn(&bleh);
	fn::Function<int(int)> lambFn([](int a) {
		return a; 
	});

	printf("%d \n", meepFn(1));
	printf("%d \n", blehFn(2));
	printf("%d \n", lambFn(3));

	DummyClass dummyClass;
	DummyStruct dummyStruct;

	fn::Function<int(int)> classFn(&DummyClass::meep, &dummyClass);
	fn::Function<int(int)> structFn(&DummyStruct::meep, &dummyStruct);

	printf("%d \n", classFn(4));
	printf("%d \n", structFn(5));

	auto x = &DummyClass::meep;
	auto y = &meep;
	auto z = fn::bind(&meep);
	auto w = fn::bind(&DummyClass::meep, &dummyClass);

	printf("%d \n", z(6));

	return getchar();
}