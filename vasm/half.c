

int main()
{
	float fval = 1.5;
	unsigned int val;
	memcpy(&val, &fval, sizeof(val));

	unsigned int sign,exp,mant,texp;

	//float to half
	sign = (val>>16)&0x8000; //sign
	texp = (val>>23)&0x00FF; //exp
	mant = (val>>13)&0x03FF; //mantisse

	exp = (texp&0x0F)<<10;

	if(texp&0x80)
		exp |= 0x4000;

	unsigned int half = sign+mant+exp;
	printf("half %x\n",half);




	//half to float
	unsigned int xval;
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

	memcpy(&fval, &xval, sizeof(xval));

	printf("%f %x/%x\n",fval,xval,val);

	return 0;
}
