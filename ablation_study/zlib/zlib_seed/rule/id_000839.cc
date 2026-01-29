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
//<ID> 839
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 512UL;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'X', (size_t)INPUT_LEN);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)INPUT_LEN;

    // step 2: Initialize deflate, compute bound and prepare output buffer
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, INPUT_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;

    // step 3: Operate: run deflate, write a byte via gzputc, and use inflateCopy for validation
    int def_status = deflate(&dstrm, 4); /* Z_FINISH */
    gzFile gf = gzopen("test_zlib_api_seq.gz", "wb");
    gzputc(gf, (int)(dstrm.total_out ? comp[0] : '\0'));
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    z_stream istrm_copy;
    memset(&istrm_copy, 0, sizeof(istrm_copy));
    int copy_status = inflateCopy(&istrm_copy, &istrm);

    // step 4: Cleanup and finalize
    gzclose(gf);
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    inflateEnd(&istrm_copy);
    free(input);
    free(comp);
    (void)ver;
    (void)bound;
    (void)def_status;
    (void)copy_status;
    // API sequence test completed successfully
    return 66;
}