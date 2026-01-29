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
//<ID> 870
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    uLong crc_orig = crc32(0UL, orig, (uInt)ORIG_LEN);
    uLong bound_comp = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound_comp);
    uLongf compLen = (uLongf)bound_comp;

    // step 2: Compress with compress2 and then run deflate on compressed data
    compress2(comp, &compLen, orig, ORIG_LEN, 6);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = comp;
    dstrm.avail_in = (uInt)compLen;
    uLong defBound = deflateBound(&dstrm, (uLong)compLen);
    Bytef *defbuf = (Bytef *)malloc((size_t)defBound);
    dstrm.next_out = defbuf;
    dstrm.avail_out = (uInt)defBound;
    deflate(&dstrm, 4);
    uLong def_out_len = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Inflate, validate via inflateSyncPoint and crc32_combine64, and write via gzdopen
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = defbuf;
    istrm.avail_in = (uInt)def_out_len;
    Bytef *outbuf = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(outbuf, 0, (size_t)ORIG_LEN);
    istrm.next_out = outbuf;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    inflateSyncPoint(&istrm);
    uLong crc_decompressed = crc32(0UL, outbuf, (uInt)ORIG_LEN);
    uLong crc_combined = crc32_combine64(crc_orig, crc_decompressed, (off64_t)ORIG_LEN);
    gzFile gw = gzdopen(1, "wb");
    gzwrite(gw, outbuf, (unsigned int)ORIG_LEN);
    gzclose(gw);

    // step 4: Cleanup
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(defbuf);
    free(outbuf);
    (void)ver;
    (void)crc_orig;
    (void)crc_decompressed;
    (void)crc_combined;
    (void)compLen;
    (void)def_out_len;
    (void)bound_comp;
    (void)defBound;
    // API sequence test completed successfully
    return 66;
}