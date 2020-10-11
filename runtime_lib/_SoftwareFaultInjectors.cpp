#include "FaultInjector.h"
#include "FaultInjectorManager.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

//2^20 == 32MB
#define MEM_EXHAUSTION_UNIT 33554432

class BitCorruptionInjector: public SoftwareFaultInjector {
	public:
	virtual void injectFault(long llfi_index, unsigned size, unsigned fi_bit,char *buf){
		unsigned int fi_bytepos = fi_bit/8;
		unsigned int fi_bitpos = fi_bit%8;
		buf[fi_bytepos] ^= 0x1 << fi_bitpos;
		return;
	}
	
	static BitCorruptionInjector* getBitCorruptionInjector(){
		static BitCorruptionInjector* injector_ptr = NULL;
		if(injector_ptr == NULL){
			injector_ptr = new BitCorruptionInjector();
			return injector_ptr;
		}else	return injector_ptr;
	}
};

class MemoryLeakInjector: public SoftwareFaultInjector {
	public:
	virtual void injectFault(long llfi_index, unsigned size, unsigned fi_bit,char *buf){
		void* fake_p = malloc(1024 * sizeof(char));
		void** newbuf = (void**) buf;
		*newbuf = fake_p;
		return;
	}
};

class HangInjector: public SoftwareFaultInjector {
	public:
	virtual void injectFault(long llfi_index, unsigned size, unsigned fi_bit,char *buf){
		while(1);
		return;
	}
};

class SleepInjector: public SoftwareFaultInjector {
	public:
	virtual void injectFault(long llfi_index, unsigned size, unsigned fi_bit,char *buf){
		sleep(3);
		return;
	}
};

static RegisterFaultInjector DA("HighFrequentEvent(Timing)", new SleepInjector());

class wctm: public SoftwareFaultInjector {
        public:
        virtual void injectFault(long llfi_index, unsigned size, unsigned fi_bit,char *buf){
                if(is_replace == false){
                        int* newbuf = (int*) buf;
                        *newbuf = *newbuf + add_val;
                }
                else{
                        int* newbuf = (int*) buf;
                        *newbuf = rep_val;
                }
                return;
        }

        wctm(int val, bool replace):add_val(val), rep_val(val), is_replace(replace){};

        private:
        int add_val;
        int rep_val;
        bool is_replace;
};

#include <sstream>
#include <string>

class floatNormalDistributionByRate: public SoftwareFaultInjector {
	public:
	float GaussRand(float dExpect, float dVariance)
	{
		static float V1, V2, S;
		static int phase = 0;
		float X;

		if (phase == 0)
		{
			do
			{
				float U1 = (float)rand() / RAND_MAX;
				float U2 = (float)rand() / RAND_MAX;

				V1 = 2 * U1 - 1;
				V2 = 2 * U2 - 1;
				S = V1 * V1 + V2 * V2;
			} while (S >= 1 || S == 0);

			X = V1 * sqrt(-2 * log(S) / S);
		}
		else
		{
			X = V2 * sqrt(-2 * log(S) / S);
		}

		phase = 1 - phase;

		return (X * dVariance + dExpect);
	}


	virtual void injectFault(long llfi_index, unsigned size, unsigned fi_bit,char *buf){
		float* newbuf = (float*) buf;
		float tmp=*newbuf;
		float add_val;
		add_val=rate*tmp;
		*newbuf=GaussRand(*newbuf,add_val/1.96);
		if(*newbuf-tmp>add_val)
			*newbuf=tmp+add_val;
		if(*newbuf<tmp-add_val)
			*newbuf=tmp-add_val;
		return;
	}
	
	floatNormalDistributionByRate(float val):rate(val){};
	
	private:
	float rate;
	
};


class floatNormalDistribution: public SoftwareFaultInjector {
	public:
	float GaussRand(float dExpect, float dVariance)
	{
		static float V1, V2, S;
		static int phase = 0;
		float X;

		if (phase == 0)
		{
			do
			{
				float U1 = (float)rand() / RAND_MAX;
				float U2 = (float)rand() / RAND_MAX;

				V1 = 2 * U1 - 1;
				V2 = 2 * U2 - 1;
				S = V1 * V1 + V2 * V2;
			} while (S >= 1 || S == 0);

			X = V1 * sqrt(-2 * log(S) / S);
		}
		else
		{
			X = V2 * sqrt(-2 * log(S) / S);
		}

		phase = 1 - phase;

		return (X * dVariance + dExpect);
	}


	virtual void injectFault(long llfi_index, unsigned size, unsigned fi_bit,char *buf){
		float* newbuf = (float*) buf;
		float tmp=*newbuf;
		*newbuf=GaussRand(*newbuf,add_val/1.96);
		if(*newbuf-tmp>add_val)
			*newbuf=tmp+add_val;
		if(*newbuf<tmp-add_val)
			*newbuf=tmp-add_val;
		return;
	}
	
	floatNormalDistribution(float val):add_val(val){};
	
	private:
	float add_val;
	
};



class floatUniformDistributionByRate: public SoftwareFaultInjector {
	public:

	float randdouble(float Lower, float Upper)
	{
		float temp;
		if (Lower > Upper)
		{
			temp = Upper;
			Upper = Lower;
			Lower = temp;
		}

		return rand() / (float)RAND_MAX * (Upper - Lower) + Lower;
	}

	virtual void injectFault(long llfi_index, unsigned size, unsigned fi_bit,char *buf){
		float* newbuf = (float*) buf;
		float tmp=*newbuf;
		float add_val;
		add_val=rate*tmp;
		//add_val=abs(add_val*tmp);
		//srand(GetTickCount());
		*newbuf=randdouble(tmp-add_val,tmp+add_val);
		//*newbuf+=(rand()/1.00)%(2*add_val);
		return;
	}

	
	
	floatUniformDistributionByRate(float val):rate(val){};
	
	private:
	float rate;
	
};


class floatUniformDistribution: public SoftwareFaultInjector {
	public:

	float randdouble(float Lower, float Upper)
	{
		float temp;
		if (Lower > Upper)
		{
			temp = Upper;
			Upper = Lower;
			Lower = temp;
		}

		return rand() / (float)RAND_MAX * (Upper - Lower) + Lower;
	}

	virtual void injectFault(long llfi_index, unsigned size, unsigned fi_bit,char *buf){
		float* newbuf = (float*) buf;
		float tmp=*newbuf;
		*newbuf=randdouble(tmp-add_val,tmp+add_val);
		return;
	}
	
	floatUniformDistribution(float val):add_val(val){};
	
	private:
	float add_val;
	
};

class NormalDistributionByRate: public SoftwareFaultInjector {
	public:
	double GaussRand(double dExpect, double dVariance)
	{
		static double V1, V2, S;
		static int phase = 0;
		double X;

		if (phase == 0)
		{
			do
			{
				double U1 = (double)rand() / RAND_MAX;
				double U2 = (double)rand() / RAND_MAX;

				V1 = 2 * U1 - 1;
				V2 = 2 * U2 - 1;
				S = V1 * V1 + V2 * V2;
			} while (S >= 1 || S == 0);

			X = V1 * sqrt(-2 * log(S) / S);
		}
		else
		{
			X = V2 * sqrt(-2 * log(S) / S);
		}

		phase = 1 - phase;

		return (X * dVariance + dExpect);
	}


	virtual void injectFault(long llfi_index, unsigned size, unsigned fi_bit,char *buf){
		double* newbuf = (double*) buf;
		double tmp=*newbuf;
		double add_val;
		add_val=rate*tmp;
		*newbuf=GaussRand(*newbuf,add_val/1.96);
		if(*newbuf-tmp>add_val)
			*newbuf=tmp+add_val;
		if(*newbuf<tmp-add_val)
			*newbuf=tmp-add_val;
		return;
	}
	
	NormalDistributionByRate(double val):rate(val){};
	
	private:
	double rate;
	
};


class NormalDistribution: public SoftwareFaultInjector {
	public:
	double GaussRand(double dExpect, double dVariance)
	{
		static double V1, V2, S;
		static int phase = 0;
		double X;

		if (phase == 0)
		{
			do
			{
				double U1 = (double)rand() / RAND_MAX;
				double U2 = (double)rand() / RAND_MAX;

				V1 = 2 * U1 - 1;
				V2 = 2 * U2 - 1;
				S = V1 * V1 + V2 * V2;
			} while (S >= 1 || S == 0);

			X = V1 * sqrt(-2 * log(S) / S);
		}
		else
		{
			X = V2 * sqrt(-2 * log(S) / S);
		}

		phase = 1 - phase;

		return (X * dVariance + dExpect);
	}


	virtual void injectFault(long llfi_index, unsigned size, unsigned fi_bit,char *buf){
		double* newbuf = (double*) buf;
		double tmp=*newbuf;
		*newbuf=GaussRand(*newbuf,add_val/1.96);
		if(*newbuf-tmp>add_val)
			*newbuf=tmp+add_val;
		if(*newbuf<tmp-add_val)
			*newbuf=tmp-add_val;
		return;
	}
	
	NormalDistribution(double val):add_val(val){};
	
	private:
	double add_val;
	
};



class UniformDistributionByRate: public SoftwareFaultInjector {
	public:

	double randdouble(double Lower, double Upper)
	{
		double temp;
		if (Lower > Upper)
		{
			temp = Upper;
			Upper = Lower;
			Lower = temp;
		}

		return rand() / (double)RAND_MAX * (Upper - Lower) + Lower;
	}

	virtual void injectFault(long llfi_index, unsigned size, unsigned fi_bit,char *buf){
		double* newbuf = (double*) buf;
		double tmp=*newbuf;
		double add_val;
		add_val=rate*tmp;
		//add_val=abs(add_val*tmp);
		//srand(GetTickCount());
		*newbuf=randdouble(tmp-add_val,tmp+add_val);
		//*newbuf+=(rand()/1.00)%(2*add_val);
		return;
	}

	
	
	UniformDistributionByRate(double val):rate(val){};
	
	private:
	double rate;
	
};


class UniformDistribution: public SoftwareFaultInjector {
	public:

	double randdouble(double Lower, double Upper)
	{
		double temp;
		if (Lower > Upper)
		{
			temp = Upper;
			Upper = Lower;
			Lower = temp;
		}

		return rand() / (double)RAND_MAX * (Upper - Lower) + Lower;
	}

	virtual void injectFault(long llfi_index, unsigned size, unsigned fi_bit,char *buf){
		double* newbuf = (double*) buf;
		double tmp=*newbuf;
		*newbuf=randdouble(tmp-add_val,tmp+add_val);
		return;
	}
	
	UniformDistribution(double val):add_val(val){};
	
	private:
	double add_val;
	
};



class ChangeValueInjector: public SoftwareFaultInjector {
	public:
	virtual void injectFault(long llfi_index, unsigned size, unsigned fi_bit,char *buf){
		if(is_replace == false){
			int* newbuf = (int*) buf;
			*newbuf = *newbuf+ add_val*(*newbuf);
		}
		else{
			int* newbuf = (int*) buf;
			*newbuf = rep_val;
		}
		return;
	}
	
	ChangeValueInjector(int val, bool replace):add_val(val), rep_val(val), is_replace(replace){};
	private:
	int add_val;
	int rep_val;
	bool is_replace;
};


class DoubleChange: public SoftwareFaultInjector {
	public:
	virtual void injectFault(long llfi_index, unsigned size, unsigned fi_bit,char *buf){
		float* newbuf = (float*) buf;
		float tmp = *newbuf;
		*newbuf=2*tmp;
		return;
	}
	
	DoubleChange(){};
	

};


class dDoubleChange: public SoftwareFaultInjector {
	public:
	virtual void injectFault(long llfi_index, unsigned size, unsigned fi_bit,char *buf){
		double* newbuf = (double*) buf;
		double tmp = *newbuf;
		*newbuf=2*tmp;
		return;
	}
	
	dDoubleChange(){};
	

};


class InappropriateCloseInjector: public SoftwareFaultInjector {
	public:
	virtual void injectFault(long llfi_index, unsigned size, unsigned fi_bit,char *buf){
		if(add_close){
			FILE** newbuf = (FILE**) buf;
			fclose(*newbuf);
		}else{
			FILE* fp = fopen("fake_file.txt", "w");
			FILE** newbuf = (FILE**) buf;
			*newbuf = fp;
		}
		return;
	}
	InappropriateCloseInjector(bool addclose):add_close(addclose){};
	
	private:
	bool add_close;
};

class StalePointerInjector: public SoftwareFaultInjector {
	public:
	virtual void injectFault(long llfi_index, unsigned size, unsigned fi_bit,char *buf){
		void** newbuf = (void**) buf;
		free(*newbuf);
	}
};

class MemoryExhaustionInjector: public SoftwareFaultInjector {
	public:
	virtual void injectFault(long llfi_index, unsigned size, unsigned fi_bit,char *buf){
		void* p = NULL;
		void* left_space = NULL;
		do{
			p = malloc(MEM_EXHAUSTION_UNIT);
			if(p == NULL)	p = malloc(MEM_EXHAUSTION_UNIT>>4);
			if(p == NULL)	p = malloc(MEM_EXHAUSTION_UNIT>>8);
			if(p == NULL)	p = malloc(MEM_EXHAUSTION_UNIT>>12);
			if(p != NULL)	left_space = p;
		}while(p != NULL);
		if(non_left_space){
			void** newbuf = (void**) buf;
			*newbuf = p;
		}else{
			void** newbuf = (void**) buf;
			*newbuf = left_space;
		}
		return;
	}
	
	MemoryExhaustionInjector(bool nonleftspace):non_left_space(nonleftspace) {};
	private:
	bool non_left_space;
};

class WrongFormatInjector: public SoftwareFaultInjector {
	public:
	virtual void injectFault(long llfi_index, unsigned size, unsigned fi_bit,char *buf){
		switch(*buf){
			case 1:
				*buf = 2; break;
			case 2:
				*buf = 4; break;
			case 4:
				*buf = 8; break;
			case 8:
				*buf = 4; break;
			case 10:
				*buf = 4; break;
			default:
				break;
		}
		return;
	}
};

class PthreadDeadLockInjector: public SoftwareFaultInjector {
	public:
	virtual void injectFault(long llfi_index, unsigned size, unsigned fi_bit,char *buf){
		pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
		pthread_mutex_lock(&mutex1);
		pthread_t thread1 = pthread_t(*buf);
		pthread_join(thread1, NULL);
		pthread_mutex_lock(&mutex1);
		return;
	}
};

class PthreadThreadKillerInjector: public SoftwareFaultInjector {
	public:
	virtual void injectFault(long llfi_index, unsigned size, unsigned fi_bit,char *buf){
		pthread_t t = pthread_t(*buf);
		sleep(0.02);
		pthread_cancel(t);
		return;
	}
};

class PthreadRaceConditionInjector: public SoftwareFaultInjector {
  public:
  virtual void injectFault(long llfi_index, unsigned size, unsigned fi_bit,char *buf) {
    pthread_mutex_t *fake_mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(fake_mutex, NULL);
    pthread_mutex_t **newbuf = (pthread_mutex_t**) buf;
    *newbuf = fake_mutex;
    return;
  }
};


