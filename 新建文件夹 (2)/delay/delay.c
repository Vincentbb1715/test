#include <delay.h>
void mDelay(unsigned int mdelay){
	
	unsigned int i;
	for(;mdelay>0;mdelay--)
	{
		for(i=0;i<54;i++);
	}
}