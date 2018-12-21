#include "bmarksg.h"

int main(){
	DB_Connection b;
	int g = b.create_group("test");
	std::cout << "Made group:" << g << '\n';
	int g2 = b.create_group("dong",g);
	std::cout << "Made child group:" << g2 << '\n';
	b.delete_group(g);
	b.print_groups();
	return 0;
}
