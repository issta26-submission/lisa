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
//<ID> 874
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
    memset(orig, 'X', (size_t)ORIG_LEN);
    uLong crc_full = crc32(0UL, orig, (uInt)ORIG_LEN);
    uLong half_len = ORIG_LEN / 2;
    uLong crc_first = crc32(0UL, orig, (uInt)half_len);
    uLong crc_second = crc32(0UL, orig + half_len, (uInt)(ORIG_LEN - half_len));
    uLong combined_crc = crc32_combine64(crc_first, crc_second, (off64_t)(ORIG_LEN - half_len));
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Setup: compress with compress2 and initialize streams for deflate/inflate
    compress2(comp, &compLen, orig, ORIG_LEN, 6);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    uLongf streamBound = compressBound(ORIG_LEN);
    Bytef *streamComp = (Bytef *)malloc((size_t)streamBound);
    memset(streamComp, 0, (size_t)streamBound);

    // step 3: Operate & validate
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = streamComp;
    dstrm.avail_out = (uInt)streamBound;
    deflate(&dstrm, 0);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    Bytef *inflate_out = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(inflate_out, 0, (size_t)ORIG_LEN);
    istrm.next_out = inflate_out;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflateSyncPoint(&istrm);
    gzFile gf = gzdopen(1, "wb");
    unsigned int produced = (unsigned int)((uLongf)streamBound - (uLongf)dstrm.avail_out);
    gzwrite(gf, streamComp, produced);
    gzclose(gf);

    // step 4: Cleanup
    inflateEnd(&istrm);
    deflateEnd(&dstrm);
    free(orig);
    free(comp);
    free(streamComp);
    free(inflate_out);
    (void)ver;
    (void)crc_full;
    (void)crc_first;
    (void)crc_second;
    (void)combined_crc;
    (void)bound;
    (void)compLen;
    (void)streamBound;
    // API sequence test completed successfully
    return 66;
}