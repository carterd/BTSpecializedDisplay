#include "BaseLvObject.h"

BaseLvObject::~BaseLvObject() {
	// Attempt any last gasp free up
	BaseLvObject::destroyLvObj();
}

lv_obj_t* BaseLvObject::getLvObj() {
	return this->this_obj;
}

void BaseLvObject::destroyLvObj() {
	if (this->this_obj) lv_obj_del(this->this_obj);
	this->this_obj = NULL;
}