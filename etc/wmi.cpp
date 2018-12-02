
#include <vector>
#include <array>
#include <unordered_map>
#include <tuple>
#include <queue>
#include <utility>

const size_t WORLD_SIZE_H = 24;
const size_t WORLD_SIZE_W = 24;

enum Direction : char {NORTH, EAST, SOUTH, WEST};
enum Status : char {BLEED,ASLEEP,BLIND};

class Item {
};

typedef std::pair<size_t,size_t> Position;

struct Stats {
	int cur_health;
	int max_health;
	int defense;
	int attack;
	int speed;
	int sight;
	Stats(int hp, int d, int a, int sp, int si): cur_health(hp), max_health(hp), defense(d), attack(a), speed(sp), sight(si){}
	Stats();
};
class World;

class Mob {
	public:
		std::vector<Status> ailments;
		std::vector<Item*> holding;
		Stats v;
		Direction facing;
		//Mob(Stats stats, Direction toface): v(stats), facing(toface){}
		//Mob(Stats stats): Mob(stats,Direction::NORTH){}
		Mob(Stats stats): v(stats){};
		Mob(){}
		void act(World& w);
};

class MobBat : public Mob {
	//MobBat(): v(Stats(3,1,1,3,2)){}
	//void act(World& w);
	MobBat(): Mob(Stats(1,2,3,4,5)){
		//v = Stats(1,2,3,4,5);
	}
};//*/

class World {
	public:
		std::array<std::array<Mob*,WORLD_SIZE_H>,WORLD_SIZE_W> field;
		//std::unordered_map<Mob*,std::pair<size_t,size_t>> mob_pos;
		std::unordered_map<Mob*,Position> mob_pos;
		std::array<std::array<Item*,WORLD_SIZE_H>,WORLD_SIZE_W> ground;
		std::unordered_map<Item*,Position> item_pos;
		std::priority_queue<std::pair<int,Mob*>> turns;
		void move_mob(Position p, Direction d);
		void loop(){
			while (mob_pos.size()>0){
				for (auto m : mob_pos){
					for (int i=m.first->v.speed;i>0;--i){
						turns.emplace(m.first->v.speed,m.first);
					}
				}
				while (!turns.empty()){
					turns.top().second->act(*this);

					turns.pop();
				}
			}
		}
};

int main(){
	auto w = new World;


	delete w;

	return 0;
}
