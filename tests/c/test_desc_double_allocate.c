#include <mad_desc.h>
#include <assert.h>

int main(void)
{
	const int nv=6, mo=2;

	const desc_t* d1 = mad_desc_newv (nv,     mo    );
	const desc_t* d2 = mad_desc_newv (nv,     mo    );

	assert(d1 != d2);

	mad_desc_del(d1);
	mad_desc_del(d2);

}
