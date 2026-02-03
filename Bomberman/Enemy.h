#pragma once
class Enemy
{
public:
	Enemy(int);
	~Enemy();
	void move();

private:
	int type;
};