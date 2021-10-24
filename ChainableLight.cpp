#include "ChainableLight.h"

ChainableLight::ChainableLight(ChainableLightListener* listener) {
	this->listener = listener;
}

ChainableLight::~ChainableLight() {
	// this effect is being deleted, so reconnect the chain
	if(this->getPrevious() != 0) {
		this->getPrevious()->setNext(this->getNext());

		if(this->getNext() != 0) {
			this->getNext()->setPrevious(this->getPrevious());
		}
	} else {
		if(this->getNext() != 0) {
			this->getNext()->setPrevious(0);
		}
		this->getListener()->onFirstElementChanged(this->getNext());
	}
}

void ChainableLight::setNext(ChainableLight* effect) {
	this->next = effect;
}

void ChainableLight::setPrevious(ChainableLight* effect) {
	this->previous = effect;
}

ChainableLight* ChainableLight::getNext() {
	return this->next;
}

ChainableLight* ChainableLight::getPrevious() {
	return this->previous;
}

void ChainableLight::add(ChainableLight* effect) {
	if(this->getNext() == 0) {
		this->setNext(effect);
		effect->setPrevious(this);
	} else {
		this->getNext()->add(effect);
	}
}

void ChainableLight::runLoop(VolumeContext &context) {
	this->loop(context);

	this->getNext()->runLoop(context);
}

ChainableLightListener* ChainableLight::getListener() {
	return this->listener;
}