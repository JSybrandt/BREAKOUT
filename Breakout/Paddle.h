#include "Actor.h"

namespace PaddleNS{
	const float SPEED = 15;
	const Vector3 PADDLE_SCALE(3,.5,1);
	const float GROWTH_AMMT = 1.5;
	const float GROWTH_SPEED = 0.8;
	const float MAX_WIDTH = 6;
	const float MIN_WIDTH = 1.5;
};
using namespace PaddleNS;
class Paddle: public virtual Actor{
public:
	Paddle(){desiredWidth=PaddleNS::PADDLE_SCALE.x;}
	void update(float dt);
	void grow(){desiredWidth+=GROWTH_AMMT;if(desiredWidth>MAX_WIDTH)desiredWidth=MAX_WIDTH;}
	void shrink(){desiredWidth-=GROWTH_AMMT; if(desiredWidth<MIN_WIDTH)desiredWidth=MIN_WIDTH;}
private:
	float desiredWidth;
};