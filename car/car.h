#include <string>
#include <ostream>

using std::string;
using std::ostream;

class Car {
	private:
		struct SPRMEM;
		SPRMEM* M;
		friend ostream& operator<<(ostream& O, Car& C);
	public:
		Car(string& name, float speed);
		~Car();
		void engineOn();
		void engineOff();
		void speedUp();
		void speedDown();
};
