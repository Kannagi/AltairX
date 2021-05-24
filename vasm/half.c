

int main()
{
	float fval = 0.00006103515625;
	unsigned int *tmp = (void*)&fval;
	unsigned int val = *tmp;
	unsigned int sign,exp,mant,texp;


	//float to half
	sign = (val>>16)&0x8000; //sign
	texp = (val>>23)&0x00FF; //exp
	mant = (val>>13)&0x03FF; //mantisse

	exp = (texp&0x0F)<<10;

	if(texp&0x80)
		exp |= 0x4000;

	//printf("%x\n",sign);
	//printf("%x\n",exp);
	//printf("%x\n",mant);

	unsigned int half = sign+mant+exp;
	printf("half %x\n",half);
	unsigned int xval;

	//half to float
	exp = (half&0x3C00)>>3;

	if(half&0x4000)
	{
		exp |= 0x4000;
	}
	else
	{
		if(exp != 0)
			exp |= 0x3800;
	}
	
	exp = exp<<16;



	xval  = (half&0x8000)<<16;
	xval += (half&0x03FF)<<13;
	xval += exp;


	//printf("%x\n",sign);
	//printf("%x\n",exp);
	//printf("%x\n",mant);

	//xval = sign+mant+exp;
	float *ftmp = (void*)&xval;

	printf("%f %x/%x\n",*ftmp,xval,val);

	return 0;
}
