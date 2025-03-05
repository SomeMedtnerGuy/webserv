#include <iostream>
#include <string>

void	myFunc(char* str) {
	if (str == NULL)
		return;
}

void myFunc(const std::string str) {
    std::cout << str << std::endl;
}

int	main(void)
{
	myFunc(NULL);
}
