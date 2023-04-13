#include <mad_desc.h>
#include <assert.h>
#include <stdio.h>

int main(void)
{
	const int nv=6, mo=2;

	const desc_t* d1 = mad_desc_newv (nv,     mo    );
	const desc_t* d2 = mad_desc_newv (nv,     mo    );

	/* Get the same descriptor if used */
	assert(d1 == d2);
	fprintf(stderr, "d1 = %p, d2 = %p\n", (void *)d1, (void*)d2);
	mad_desc_del(d2);

}
