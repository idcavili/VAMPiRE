#include "Node.h"
class InputNode:public Node
{
public:
	//String id;
	//String name;
	int address[4];
	short port;
	double *outputBuffer;
};
