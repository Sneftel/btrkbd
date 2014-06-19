#include "trainer.h"
#include "keymatrix.h"
#include "kbd.h"

void initTrainer()
{
}

static const char TRAINER_KEYS[] = {
	calcKeyIdx(1,0), calcKeyIdx(1,9), calcKeyIdx(3,0), calcKeyIdx(3,9)
};

static const char INHIBIT_KEYS[] = {
	calcKeyIdx(2,0), calcKeyIdx(2,9)
};

static char shouldStartTrainer()
{
	char NUM_TRAINER_KEYS = sizeof(TRAINER_KEYS)/sizeof(*TRAINER_KEYS);
	char NUM_INHIBIT_KEYS = sizeof(INHIBIT_KEYS)/sizeof(*INHIBIT_KEYS);
	int i;
	for(i=0; i<NUM_TRAINER_KEYS; i++)
	{
		if(!isKeyDown(TRAINER_KEYS[i]))
		{
			return 0;
		}
	}
	for(i=0; i<NUM_INHIBIT_KEYS; i++)
	{
		if(isKeyDown(INHIBIT_KEYS[i]))
		{
			return 0;
		}
	}
	
	return 1;
}

void tickTrainer()
{
	if(shouldStartTrainer())
	{
		setTrainerMode();
	}
}
