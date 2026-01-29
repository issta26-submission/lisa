#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 802
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain[256];
    memset(plain, 'A', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);

    z_stream dstr;
    memset(&dstr, 0, sizeof(dstr));
    int dinit = deflateInit_(&dstr, 6, version, (int)sizeof(z_stream));
    dstr.next_in = plain;
    dstr.avail_in = (uInt)plain_len;
    uLong bound = deflateBound(&dstr, plain_len);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    dstr.next_out = comp;
    dstr.avail_out = (uInt)bound;

    // step 2: Configure
    int def_ret = deflate(&dstr, 4);
    uLong comp_len = dstr.total_out;

    // step 3: Operate
    z_stream istr;
    memset(&istr, 0, sizeof(istr));
    int iinit = inflateInit_(&istr, version, (int)sizeof(z_stream));
    istr.next_in = comp;
    istr.avail_in = (uInt)comp_len;
    Bytef *decomp = (Bytef *)malloc((size_t)plain_len);
    memset(decomp, 0, (size_t)plain_len);
    istr.next_out = decomp;
    istr.avail_out = (uInt)plain_len;
    int inf_ret = inflate(&istr, 0);
    unsigned long codes = inflateCodesUsed(&istr);

    // step 4: Validate / Cleanup
    gzFile gzf = gzdopen(1, "wb");
    int gz_put = gzputs(gzf, "zlib gzputs test\n");
    int gz_cl = gzclose(gzf);
    int def_end = deflateEnd(&dstr);
    int inf_end = inflateEnd(&istr);
    free(comp);
    free(decomp);

    (void)version;
    (void)dinit;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)iinit;
    (void)inf_ret;
    (void)codes;
    (void)gz_put;
    (void)gz_cl;
    (void)def_end;
    (void)inf_end;
    // API sequence test completed successfully
    return 66;
}