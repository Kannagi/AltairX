

typedef struct
{
	void *ram,*tsram,*spm2;
	void *adrFbuffer,*adrZbuffer;
	uint8_t *clut;

	uint32_t flagsPPU;
	int exeEnable,pcBegin;
	int ppuEnable;

	uint8_t linepixel[64];
}GPU;

