#include "bmarksg.h"

int main(){
	DB_Connection b;
	int g = b.create_group("test");
	std::cout << "Made group:" << g << '\n';
	g = b.create_group("dong",g);
	std::cout << "Made child group:" << g << '\n';
	b.print_groups();
	return 0;
}
