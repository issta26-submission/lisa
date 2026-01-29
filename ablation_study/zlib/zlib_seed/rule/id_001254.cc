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
//<ID> 1254
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and create a compressed buffer using compress2 (one-shot)
    const uLong SRC_LEN = 256UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'C', (size_t)SRC_LEN);
    uLongf compBound = (uLongf)compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)compBound);
    uLongf compLen = compBound;
    compress2(comp, &compLen, src, SRC_LEN, 6);

    // step 2: Initialize a deflate stream with deflateInit2_ and then call deflateResetKeep, finally cleanup deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    const char *ver = zlibVersion();
    deflateInit2_(&dstrm, 6, 8 /* Z_DEFLATED */, 15, 8 /* memLevel */, 0 /* strategy */, ver, (int)sizeof(z_stream));
    deflateResetKeep(&dstrm);
    deflateEnd(&dstrm);

    // step 3: Initialize inflate stream, validate it with inflateValidate, then decompress the compressed buffer with inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateValidate(&istrm, 15);
    Bytef *out = (Bytef *)malloc((size_t)SRC_LEN);
    memset(out, 0, (size_t)SRC_LEN);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = out;
    istrm.avail_out = (uInt)SRC_LEN;
    inflate(&istrm, 0);

    // step 4: Compute CRCs for source and decompressed output, cleanup inflate and free buffers
    uLong crc_src = crc32_z(0UL, src, (z_size_t)SRC_LEN);
    uLong crc_out = crc32_z(0UL, out, (z_size_t)istrm.total_out);
    (void)crc_src;
    (void)crc_out;
    inflateEnd(&istrm);
    free(src);
    free(comp);
    free(out);
    // API sequence test completed successfully
    return 66;
}