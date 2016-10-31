#include <stdio.h>
#include <stdint.h>
#include "FLOAT.h"

//#define TEST_LINUX

#ifdef TEST_LINUX
#include <sys/mman.h>
#endif

extern char _vfprintf_internal;
extern char _fpmaxtostr;
extern char _ppfs_setargs;
extern int __stdio_fwrite(char *buf, int len, FILE *stream);

__attribute__((used)) static int format_FLOAT(FILE *stream, FLOAT f) {
	/* TODO: Format a FLOAT argument `f' and write the formating
	 * result to `stream'. Keep the precision of the formating
	 * result with 6 by truncating. For example:
	 *              f          result
	 *         0x00010000    "1.000000"
	 *         0x00013333    "1.199996"
	 */
#ifdef TEST_LINUX
	printf("f = %x\n", f);
	f = 0x10000;
#endif
	char buf[80];
	int i = 0;
	int j = 15;
	uint32_t result = 0;
	uint32_t ex = 1;
	if((f >> 31) & 1 == 1) {
		i += sprintf(buf, "%c", '-');
		f = ~f + 1;
	}
	result = (f & 0x7fff0000) >> 16;
#ifdef TEST_LINUX
	printf("result = %d\n", result);
#endif
	i += sprintf(buf+i, "%d", result);
	i += sprintf(buf+i, "%c", '.');
	result = 0;
	for(ex = 500000000; j >= 0; --j) {
		result += ((f >> j) & 1) * ex;
		ex /= 2;
	}
	result /= 1000;

	i += sprintf(buf+i, "%06d", result);
#ifdef TEST_LINUX
	printf("result = %d\n", result);
#endif
	/*bad 
	uint32_t result = (f & 0x80000000);
	if(result)	f = ~f;
	int8_t e = 0;
	for (; (f & 0x40000000) == 0 && e < 32; e++)
		f = f << 1;
	e = 14 - e;
	result = result | ((f & 0x3fffffff) >> 8);
	result = result | ((e + 127) << 23);
	result = result & 0xfffffeff;*/ 
//	int len = sprintf(buf, "0x%08x", f);
#ifdef TEST_LINUX
	printf("buf = %s, len = %d\n", buf, i);
#endif
	return __stdio_fwrite(buf, i, stream);
}

static void modify_vfprintf() {
 	/* TODO: Implement this function to hijack the formating of "%f"
	 * argument during the execution of `_vfprintf_internal'. Below
	 * is the code section in _vfprintf_internal() relative to the
	 * hijack.
	 */

	uint32_t addr_format = (uint32_t)(format_FLOAT);
	uint32_t addr_vf = (uint32_t)(void *)&_vfprintf_internal;
	uint32_t addr_fp = (uint32_t)(void *)&_fpmaxtostr;
//	printf("%x, %x, %x\n", addr_format, addr_vf, addr_fp);
	uint32_t addr_call = addr_vf + 0x306;
	uint32_t addr_delta = (*(uint32_t *)(void *)(addr_call+1)) + (addr_format - addr_fp);

#ifdef TEST_LINUX
	mprotect((void *)((addr_call - 100 + 5) & 0xfffff000), 4096*2, PROT_READ | PROT_WRITE | PROT_EXEC);
#endif

	*(uint32_t *)(void *)(addr_call+1) = addr_delta;
//	printf("*addr_call = %x\n", *(uint16_t *)(void *)(addr_call+1));
//	printf("%x, %x\n", addr_delta);
	// modify instrucrions 
	*(uint32_t *)(void *)(addr_vf + 0x2fc) = 0xff9030ff;	//ff 30 90 ff
//	printf("modyfy 3\n");
	*(uint32_t *)(void *)(addr_vf + 0x2f9) = 0xff08ec83;	// 83 ec 08 ff
	*(uint16_t *)(void *)(addr_vf + 0x2e8) = 0x9090;
	*(uint16_t *)(void *)(addr_vf + 0x2e4) = 0x9090;
//	printf("modyfy 4\n");


# if 0
	else if (ppfs->conv_num <= CONV_A) {  /* floating point */
		ssize_t nf;
		nf = _fpmaxtostr(stream,
				(__fpmax_t)
				(PRINT_INFO_FLAG_VAL(&(ppfs->info),is_long_double)
				 ? *(long double *) *argptr
				 : (long double) (* (double *) *argptr)),
				&ppfs->info, FP_OUT );
		if (nf < 0) {
			return -1;
		}
		*count += nf;

		return 0;
		p
	} else if (ppfs->conv_num <= CONV_S) {  /* wide char or string */
#endif

	/* You should modify the run-time binary to let the code above
	 * call `format_FLOAT' defined in this source file, instead of
	 * `_fpmaxtostr'. When this function returns, the action of the
	 * code above should do the following:
	 */

#if 0
	else if (ppfs->conv_num <= CONV_A) {  /* floating point */
		ssize_t nf;
		nf = format_FLOAT(stream, *(FLOAT *) *argptr);
		if (nf < 0) {
			return -1;
		}
		*count += nf;

		return 0;
	} else if (ppfs->conv_num <= CONV_S) {  /* wide char or string */
#endif

}

static void modify_ppfs_setargs() {
	/* TODO: Implement this function to modify the action of preparing
	 * "%f" arguments for _vfprintf_internal() in _ppfs_setargs().
	 * Below is the code section in _vfprintf_internal() relative to
	 * the modification.
	 */
	uint32_t addr_ppfs = (uint32_t)(void *)&_ppfs_setargs;
	// e9 a8 ff ff ff
	//eb 35
#ifdef TEST_LINUX
	mprotect((void *)((addr_ppfs - 100 + 5) & 0xfffff000), 4096*2, PROT_READ | PROT_WRITE | PROT_EXEC);
#endif
	*(uint16_t *)(void *)(addr_ppfs + 0x71) = 0x35eb;
//	*(uint32_t *)(void *)(addr_ppfs + 0x72) = 0xffffffa8;
	 
	



#if 0
	enum {                          /* C type: */
		PA_INT,                       /* int */
		PA_CHAR,                      /* int, cast to char */
		PA_WCHAR,                     /* wide char */
		PA_STRING,                    /* const char *, a '\0'-terminated string */
		PA_WSTRING,                   /* const wchar_t *, wide character string */
		PA_POINTER,                   /* void * */
		PA_FLOAT,                     /* float */
		PA_DOUBLE,                    /* double */
		__PA_NOARG,                   /* non-glibc -- signals non-arg width or prec */
		PA_LAST
	};

	/* Flag bits that can be set in a type returned by `parse_printf_format'.  */
	/* WARNING -- These differ in value from what glibc uses. */
#define PA_FLAG_MASK		(0xff00)
#define __PA_FLAG_CHAR		(0x0100) /* non-gnu -- to deal with hh */
#define PA_FLAG_SHORT		(0x0200)
#define PA_FLAG_LONG		(0x0400)
#define PA_FLAG_LONG_LONG	(0x0800)
#define PA_FLAG_LONG_DOUBLE	PA_FLAG_LONG_LONG
#define PA_FLAG_PTR		(0x1000) /* TODO -- make dynamic??? */

	while (i < ppfs->num_data_args) {
		switch(ppfs->argtype[i++]) {
			case (PA_INT|PA_FLAG_LONG_LONG):
				GET_VA_ARG(p,ull,unsigned long long,ppfs->arg);
				break;
			case (PA_INT|PA_FLAG_LONG):
				GET_VA_ARG(p,ul,unsigned long,ppfs->arg);
				break;
			case PA_CHAR:	/* TODO - be careful */
				/* ... users could use above and really want below!! */
			case (PA_INT|__PA_FLAG_CHAR):/* TODO -- translate this!!! */
			case (PA_INT|PA_FLAG_SHORT):
			case PA_INT:
				GET_VA_ARG(p,u,unsigned int,ppfs->arg);
				break;
			case PA_WCHAR:	/* TODO -- assume int? */
				/* we're assuming wchar_t is at least an int */
				GET_VA_ARG(p,wc,wchar_t,ppfs->arg);
				break;
				/* PA_FLOAT */
			case PA_DOUBLE:
				GET_VA_ARG(p,d,double,ppfs->arg);
				break;
			case (PA_DOUBLE|PA_FLAG_LONG_DOUBLE):
				GET_VA_ARG(p,ld,long double,ppfs->arg);
				break;
			default:
				/* TODO -- really need to ensure this can't happen */
				assert(ppfs->argtype[i-1] & PA_FLAG_PTR);
			case PA_POINTER:
			case PA_STRING:
			case PA_WSTRING:
				GET_VA_ARG(p,p,void *,ppfs->arg);
				break;
			case __PA_NOARG:
				continue;
		}
		++p;
	}
#endif

	/* You should modify the run-time binary to let the `PA_DOUBLE'
	 * branch execute the code in the `(PA_INT|PA_FLAG_LONG_LONG)'
	 * branch. Comparing to the original `PA_DOUBLE' branch, the
	 * target branch will also prepare a 64-bit argument, without
	 * introducing floating point instructions. When this function
	 * returns, the action of the code above should do the following:
	 */

#if 0
	while (i < ppfs->num_data_args) {
		switch(ppfs->argtype[i++]) {
			case (PA_INT|PA_FLAG_LONG_LONG):
			here:
				GET_VA_ARG(p,ull,unsigned long long,ppfs->arg);
				break;
			// ......
				/* PA_FLOAT */
			case PA_DOUBLE:
				goto here;
				GET_VA_ARG(p,d,double,ppfs->arg);
				break;
			// ......
		}
		++p;
	}
#endif

}

void init_FLOAT_vfprintf() {
	modify_vfprintf();
	modify_ppfs_setargs();
}
