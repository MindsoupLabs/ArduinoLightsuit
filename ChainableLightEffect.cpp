#include "ChainableLightEffect.h"

ChainableLightEffect::ChainableLightEffect(ChainableLightEffectListener* listener) {
	this->listener = listener;
}

ChainableLightEffect::~ChainableLightEffect() {
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

void ChainableLightEffect::setNext(ChainableLightEffect* effect) {
	this->next = effect;
}

void ChainableLightEffect::setPrevious(ChainableLightEffect* effect) {
	this->previous = effect;
}

ChainableLightEffect* ChainableLightEffect::getNext() {
	return this->next;
}

ChainableLightEffect* ChainableLightEffect::getPrevious() {
	return this->previous;
}

void ChainableLightEffect::add(ChainableLightEffect* effect) {
	if(this->getNext() == 0) {
		this->setNext(effect);
		effect->setPrevious(this);
	} else {
		this->getNext()->add(effect);
	}
}

void ChainableLightEffect::runLoop(VolumeContext &context) {
	this->loop(context);

	this->getNext()->runLoop(context);
}

ChainableLightEffectListener* ChainableLightEffect::getListener() {
	return this->listener;
}