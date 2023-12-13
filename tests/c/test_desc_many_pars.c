#include <mad_desc.h>
#include <assert.h>
#include <stdio.h>

int main(void)
{
    const int nv=6, np=144;
    const ord_t mo=7, po=1;

    const desc_t* d = mad_desc_newvp (nv, mo, np, po);
    assert(d);
    mad_desc_info(d, NULL);

}
