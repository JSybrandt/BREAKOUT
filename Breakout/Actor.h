
#ifndef GameObject_H
#define GameObject_H

#include "d3dUtil.h"


#include "Geometry.h"
#include "constants.h"
#include "audio.h"

class BREAKOUT;

static int foo[1];

enum CollisionType{
	SPHERE,
	AABBox
};


class Actor {
private:
	Geometry *geometry; 
	float radius;
	float radiusSquared;
	ID3D10EffectTechnique* mTech;

	Matrix wvp,s,t,r,texMtx;//world view projection
protected:
	AudioData * audioData3D;
	Matrix world;
	Vector3 position;
	Vector3 velocity;
	Vector3 rotation;
	float speed;
	float health;//simple health value. Starts at MAX_HEATH at spawn and object dies if health hits 0
	//consider 100 health to be the "standard"
	float MAX_HEALTH;
	//bool targeted; 
	BREAKOUT* game;
	Vector3 scale;

public:

	Actor();
	~Actor();

	void init(BREAKOUT* game,Geometry *b,TEXTURE* t, float r=1,  Vector3 scale = Vector3(1,1,1));
	virtual void draw(ID3D10EffectMatrixVariable* fx, Matrix& camera, Matrix& projection, ID3D10EffectTechnique* mTech);
	virtual void update(float dt);

	bool rayCollidesWith(Vector3 pos, Vector3 dir, float& dist);
	void create(Vector3 pos, Vector3 rotate = Vector3(0,0,0), Vector3 scale = Vector3(1,1,1));
	void setPosition (Vector3 pos) {position = pos;}
	Vector3 getPosition() {return position;}
	void setVelocity (Vector3 vel) {velocity = vel;}
	Vector3 getVelocity() {return velocity;}
	void setSpeed(float s) {speed = s;}
	float getSpeed() {return speed;}
	void setRadius(float r) {radius = r; radiusSquared = (getSmallestScale()*r)*(getSmallestScale()*r);}
	float getRadiusSquare() {return radiusSquared;}
	float getRadius() {return radius;}
	Matrix getWorldMatrix() {return world;}
	void setScale(Vector3 s) {scale = s; radiusSquared = (getSmallestScale()*radius)*(getSmallestScale()*radius);}
	Vector3 getScale() {return scale;}
	float getLargestScale(){return max(max(scale.x,scale.y),scale.z);}
	float getSmallestScale(){return min(min(scale.x,scale.y),scale.z);}
	void setMTech(ID3D10EffectTechnique* m){ mTech = m;}
	virtual bool collided(Actor *gameObject);
	void virtual onDeath();
	void setRotation(Vector3 rot){rotation = rot;}
	Vector3 getRotation(){return rotation;}
	bool isActive;
	bool isVisible;
	float getHealth(){return health;}
	void setHealth(float H){health = H;}
	void setGeometry(Geometry* g){geometry=g;}
	Geometry* getGeometry(){return geometry;}

	CollisionType collisionType;


	static float SquaredDistPointAABB(Vector3 p, Vector3 min, Vector3 max);

	//all params inout
	void transformAABB(Vector3 & min, Vector3 & max);
	
	TEXTURE* tex;

};



#endif
