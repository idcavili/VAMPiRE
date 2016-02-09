#include <string>
class MidiWire
{
public:
	std::string source;
	bool chanMap[16][16];
	void mapEvents();
};
