struct reponse{
	float facto;
	int errno;
};

typedef struct reponse reponse;

program FACTO{
	version VERSION_UN{
		void FACTO_NULL(void)=0;
		reponse FACTO_RES(int)=1;
	}=1;
}=0x20000002;	
