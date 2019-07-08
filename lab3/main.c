#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>

double w_time();
double get_clock_time();
static inline uint64_t rdtsc();
double get_hz_proc();

double E(double sum_time, int n);
double S2(double *all_time, int n);
double S(double S2);
const double eps = 1E-10;

double f(double x){//integrated function
	return 1/x;
}

double integr(double a, double b, int nh) {//trapezoid integration
        double h = (b - a) / nh;
        double x = a;
        double sum = 0;

        for (int i = 1; i < nh; i++) {
            x = a + i * h;
            sum += f(x);
        }

        sum = 2 * sum + (f(a) + f(b));
        return sum * h / 2;
}

struct timeval tv1,tv2,dtv;//implementing the gettimeofday() check of our function's speed
struct timezone tz;
void time_start() { gettimeofday(&tv1, &tz); }
long time_stop(){	
	gettimeofday(&tv2, &tz);
	dtv.tv_sec= tv2.tv_sec -tv1.tv_sec;
	dtv.tv_usec=tv2.tv_usec-tv1.tv_usec;
	if(dtv.tv_usec<0) { dtv.tv_sec--; dtv.tv_usec+=1000000; }
	return dtv.tv_sec*1000000 + dtv.tv_usec;//return a number of microseconds
}

static inline uint64_t read_tsc_before_std()
{//this code apparently pushes all the comands to a single core of CPU. That's the reason, the number
//is bigger, than in other tests
 register uint32_t high, low;
 /*
 * 1. Prevent out-of-order execution (serializing by CPUID(0)):
 * wait for the completion of all previous operations (before measured code)
 * 2. Read TSC value
 */
 __asm__ __volatile__ (
 "xorl %%eax, %%eax\n"
 "cpuid\n" /* Serialize execution */
 "rdtsc\n" /* Read TSC */
 "movl %%edx, %0\n" /* - high 32 bits */
 "movl %%eax, %1\n" /* - low 32 bits */
 : "=r" (high), "=r" (low) /* Output */
 : /* Input */
 : "%rax", "%rbx", "%rcx", "%rdx" /* Clobbered registers */
 );
 return ((uint64_t)high << 32) | low;
}

static inline uint64_t read_tsc_after_std()//http://www.mkurnosov.net/uploads/Main/mkurnosov-rdtsc-2014.pdf
{
 register uint32_t high, low;
 /*
 * 1. Serialize by CPUID(0): wait for the completion of all operations in measured block
 * 2. Read TSC value
 */
 __asm__ __volatile__ (
 "xorl %%eax, %%eax\n"
 "cpuid\n" /* Serialize: wait for all prev. ops */
 "rdtsc\n" /* Read TSC */
 "movl %%edx, %0\n" /* - high 32 bits */
 "movl %%eax, %1\n" /* - low 32 bits */
 : "=r" (high), "=r" (low) /* Output */
 : /* Input */
 : "%rax", "%rbx", "%rcx", "%rdx" /* Clobbered registers */
 );
 return ((uint64_t)high << 32) | low;
}

double w_time()//returns time using gettimeofday()
{
	struct timeval tv; //<sys/time.h>
	gettimeofday(&tv, NULL);//The gettimeofday() function obtains the current time, expressed as seconds and microseconds since 00:00 Coordinated Universal Time (UTC), January 1, 1970, and stores it in the timeval structure. About the second parameter: If tzp is not a null pointer, the behaviour is unspecified.
	return tv.tv_sec + tv.tv_usec * 1E-6;//we need to add the rest in microseconds. And those are also integer, so they have to be divided by 10^6 to become true
}

//returns clock() time
double get_clock_time()//trying to get rid of this function...it just gets clock_t divided by 10^6
{
	double time;
	clock_t cl = clock();//https://www.gnu.org/software/libc/manual/html_node/CPU-Time.html#CPU-Time
	if (cl != (clock_t)-1)
		time = (double)cl / (double)CLOCKS_PER_SEC;
	return time;//WHATS HAPPENING HERE?
}

static inline uint64_t rdtsc()
{//http://www.mkurnosov.net/uploads/Main/mkurnosov-rdtsc-2014.pdf
	uint32_t high, low;
	__asm__ __volatile__ (
		"xorl %%eax, %%eax\n"
		"cpuid\n"
		"rdtsc\n"
		"movl %%edx, %0\n"
		"movl %%eax, %1\n"
		: "=r" (high), "=r" (low) 
		:: "%rax", "rbx", "%rcx", "%rdx"
	);
	return ((uint64_t)high << 32) | low;
}



double get_hz_proc()//returns a number of GygaHerz of my CPU
{
	system("./proc_hz.sh");//just launching ./proc_hz.sh for writing the cpu's frequency to hz.txt...
	FILE *in = fopen("hz.txt", "r");
	if (!in)
		return -1;
	
	double hz;
	char *str = NULL;
	size_t len = 0;

	getline(&str, &len, in);

	hz = atof(str);//String to double(letters are ignored except for e)

	fclose(in);
	return hz;
}

double E(double sum_time, int n) { return sum_time / n; }//primitve action

double S2(double *all_time, int n)
{
	double s1 = 0, s2 = 0;
	for (int i = 0; i < n; i++) {
		s1 += powf(all_time[i], 2);
		s2 += all_time[i];
	}
	s1 *= n;
	s2 = powf(s2, 2);
	return (s1 - s2) / (n * (n - 1));
}

double S(double S2) { return sqrt(S2); }

double absolute_error(double *all_time, double E_time, int n)
{
	double sum = 0;
	for (int i = 0; i < n; i++)
		sum += fabsf(all_time[i] - E_time);//float fabsf(float x); double fabs(double x);
	return sum / n;
}

double relative_error(double abs_err_time, double E_time)
{
	return (abs_err_time / E_time) * 100;
}

int main(int argc, char const *argv[])
{
	#if 1 //another way of writing if statement
	FILE *out_wtime = fopen("wtime.txt", "w");
	FILE *out_click_time = fopen("clock_time.txt", "w");
	FILE *out_tsc_time = fopen("tsc_time.txt", "w");
	#endif

	const double a = 0.5;//integration point
	const double b = 2;//integration point
	const double nh = 100;//a number of hops for trapezoid integration
	
	//printf("\nanswer: %f\n", integr(a,b,nh));

	clock_t start, end;
	double cpu_time_used;
	start = clock();
	integr(a, b, nh);
	end = clock();
	cpu_time_used = ((double) (end - start))/ CLOCKS_PER_SEC;// CLOCKS_PER_SEC is always one million
	//so it's just for an estimate(number) to comppare to each other here(it doesn't compare my CPU
	//with some other, it allows me to compare two numbers in a single program, that show which
	//code is faster)https://www.gnu.org/software/libc/manual/html_node/CPU-Time.html#CPU-Time
	//printf("start: %li, end: %li\n", start, end);
	printf("clock() function:        %f seconds\n", cpu_time_used);
	
	time_start();
	integr(a, b, nh);
	printf("\ngettimeofday() function: %f seconds\n",(double) time_stop() * 1E-6);	
	
	double Hz = get_hz_proc() * pow(10, 9);// getting a number of Herz of my CPU
	uint64_t tsc01, tsc02;//unsigned integer 64 type of a number with constant width
	tsc01 = read_tsc_before_std();
	integr(a, b, nh);
	tsc02 = read_tsc_after_std();
	tsc02 -=tsc01;
	printf("\ntsc function:            %f seconds\n",(double) tsc02 /(Hz));

	int count_start = 50;
	
	double wtime;
	double clock_time;
	double tsc_time;
	uint64_t tsc;//unsigned integer 64 type of a number with constant width

	double *all_wtime = malloc(sizeof(all_wtime) * count_start);
	double *all_clock_time = malloc(sizeof(all_clock_time) * count_start);
	double *all_tsc_time = malloc(sizeof(all_tsc_time) * count_start);

	double sum_wtime = 0;
	double sum_clock_time = 0;
	double sum_tsc_time = 0;


	for (int i = 0; i < count_start; i++) {//repeat 50 times

		wtime = w_time();
		integr(a, b, nh);
		wtime = w_time() - wtime;

		clock_time = get_clock_time();
		integr(a, b, nh);
		clock_time = get_clock_time() - clock_time;

		tsc = rdtsc();
		integr(a, b, nh);
		tsc = rdtsc() - tsc;
		tsc_time = tsc / Hz;// first time I see the Hz variable used

		all_wtime[i] = wtime;
		all_clock_time[i] = clock_time;
		all_tsc_time[i] = tsc_time;

		sum_wtime += wtime;
		sum_clock_time += clock_time;
		sum_tsc_time += tsc_time;

		#if 0
		printf("//////%d//////\n", i);
		printf("t(gettimeofday) = %.6f\n", wtime);
		printf("t(clock) = %.6f\n", clock_time);
		printf("t(tsc) = %.6f\n", tsc_time);
		#endif

		#if 1
		fprintf(out_wtime, "%d %.10f\n", i, wtime);
		fprintf(out_click_time, "%d %.10f\n", i, clock_time);
		fprintf(out_tsc_time, "%d %.10f\n", i, tsc_time);
		#endif

	}

	#if 1
	double E_wtime = E(sum_wtime, count_start);//just dividing sum_wtime by count_start
	//and assigning it to E_wtime
	double E_clock_time = E(sum_clock_time, count_start);//the same as the second line
	double E_tsc_time = E(sum_tsc_time, count_start);

	printf("////////\n");
	printf("E(wtime) = %.6f sec.\n", E_wtime);
	printf("E(clock) = %.6f sec.\n", E_clock_time);
	printf("E(tsc) = %.6f sec.\n", E_tsc_time);
	printf("\n");
	#endif

	#if 0
	double S2_wtime = S2(all_wtime, count_start);
	double S2_clock_time = S2(all_clock_time, count_start);
	double S2_tsc_time = S2(all_tsc_time, count_start);

	printf("////////\n");
	printf("S^2(wtime) = %.6f\n", S2_wtime);
	printf("S^2(clock) = %.6f\n", S2_clock_time);
	printf("S^2(tsc) = %.6f\n", S2_tsc_time);
	printf("\n");
	#endif

	#if 0
	double s_wtime = S(S2_wtime);
	double s_clock_time = S(S2_clock_time);
	double s_tsc_time = S(S2_tsc_time);

	printf("////////\n");
	printf("S(wtime) = %.6f sec.\n", s_wtime);
	printf("S(clock) = %.6f sec.\n", s_clock_time);
	printf("S(tsc) = %.6f sec.\n", s_tsc_time);
	printf("\n");
	#endif

	#if 1
	double abs_err_wtime = absolute_error(all_wtime, E_wtime, count_start);
	double abs_err_clock = absolute_error(all_clock_time, E_clock_time, count_start);
	double abs_err_tsc = absolute_error(all_tsc_time, E_tsc_time, count_start);

	printf("////////\n");
	printf("Absolute error(wrime) = %.10f sec.\n", abs_err_wtime);
	printf("Absolute error(clock) = %.10f sec.\n", abs_err_clock);
	printf("Absolute error(tsc)   = %.10f sec.\n", abs_err_tsc);
	printf("\n");
	#endif

	#if 1
	double rel_err_wtime = relative_error(abs_err_wtime, E_wtime);
	double rel_err_clock = relative_error(abs_err_clock, E_clock_time);
	double rel_err_tsc = relative_error(abs_err_tsc, E_tsc_time);

	printf("////////\n");
	printf("Relative error(wtime) = %.3f%c\n", rel_err_wtime, (char) 37);
	printf("Relative error(clock) = %.3f%c\n", rel_err_clock, (char) 37);
	printf("Relative error(tsc)   = %.3f%c\n", rel_err_tsc, (char) 37);
	printf("\n");
	#endif
	
	#if 0
	printf("Res integr: %.6f\n", sq[k]);
	printf("t(gettimeofday) = %.6f\n", wt);
	printf("t(clock) = %.6f\n", clt);
	printf("t(tsc) = %.6f\n", tsc_time);
	#endif

	#if 1
	fclose(out_tsc_time);
	fclose(out_click_time);
	fclose(out_wtime);
	#endif

	system("gnuplot scen.plt");

	free(all_wtime);
	free(all_clock_time);
	free(all_tsc_time);

	return 0;
}
