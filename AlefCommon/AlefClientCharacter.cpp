#include "AlefClientCharacter.h"

AlefClientCharacter::AlefClientCharacter()
{
	acctID = 0;
}

AlefClientCharacter::~AlefClientCharacter()
{

}

CharacterData AlefClientCharacter::getCharAtSlot(Int32 slotIdx)
{
	for (clientCharDataVec::iterator itr = characters.begin(); itr != characters.end(); itr++)
	{
		UInt32 slot = itr->get()->slot;
		if (slot == slotIdx)
			return *itr->get();
	}
}

UInt32 AlefClientCharacter::getAvailableSlotNum()
{
	UInt32 availSlot = 0;
	if (characters.size() == 0)
		return availSlot;
	else
	{
		for (clientCharDataVec::iterator itr = characters.begin(); itr != characters.end(); itr++)
		{
			UInt32 slot = itr->get()->slot;
			if (slot > availSlot)
				availSlot = slot;
		}
		return availSlot + 1;
	}

	return -1;
}