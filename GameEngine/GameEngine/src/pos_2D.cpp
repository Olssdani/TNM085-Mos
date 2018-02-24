
#include "pos_2D.h"




pos_2D::pos_2D() {
	x = 0;
	y = 0;
}

pos_2D::pos_2D(const double &_x, const double &_y){
	x = _x;
	y = _y;
}

pos_2D::pos_2D(const pos_2D &_pos) {
	x = _pos.x;
	y = _pos.y;
}

void pos_2D::set(const double &_x, const double &_y) {
	x = _x;
	y = _y;
}
//Returns the manhattan distance multiplied by 10 
int const pos_2D::manhatan(const pos_2D &_pos) {
	return (abs(this->x -_pos.x) + abs(this->y - _pos.y))*10;
}

bool const pos_2D::operator==(const pos_2D &_pos) {
	return (this->x == _pos.x && this->y == _pos.y);
}