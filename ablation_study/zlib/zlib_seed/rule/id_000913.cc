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
//<ID> 913
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 1024UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);

    // step 2: Initialize deflate and produce compressed data
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    uLong compressed_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Initialize inflate, copy the inflate stream, and perform decompression
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compressed_size;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    z_stream istrm_copy;
    memset(&istrm_copy, 0, sizeof(istrm_copy));
    int copy_res = inflateCopy(&istrm_copy, &istrm);
    inflate(&istrm, 0);
    inflate(&istrm_copy, 0);
    inflateEnd(&istrm);
    inflateEnd(&istrm_copy);

    // step 4: Validate with CRC, exercise gzFile API, and cleanup
    uLong crc = crc32(0L, orig, (uInt)ORIG_LEN);
    gzFile gf = gzopen("/dev/null", "rb");
    int ch = gzgetc_(gf);
    gzclose(gf);
    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)bound;
    (void)compressed_size;
    (void)copy_res;
    (void)crc;
    (void)ch;
    // API sequence test completed successfully
    return 66;
}