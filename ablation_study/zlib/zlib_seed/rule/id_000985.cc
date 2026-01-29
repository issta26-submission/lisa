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
//<ID> 985
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 16UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    uLong crc_orig = crc32(0L, orig, (uInt)ORIG_LEN);

    // step 2: Write a few bytes to a gzip file using gzputc and close writer
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gw, (int)orig[0]);
    gzputc(gw, (int)orig[1]);
    gzputc(gw, (int)orig[2]);
    gzputc(gw, (int)orig[3]);
    gzclose_w(gw);

    // step 3: Compress in-memory, initialize inflate, reset, inflate and keep state
    compress2(comp, &comp_len, orig, (uLong)ORIG_LEN, 6);
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflateReset(&istrm);
    inflate(&istrm, 0);
    inflateResetKeep(&istrm);

    // step 4: Read back one character with gzgetc_, compute crc of decompressed data, cleanup
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    int read_char = gzgetc_(gr);
    uLong crc_decomp = crc32(0L, decomp, (uInt)ORIG_LEN);
    gzclose(gr);
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)read_char;
    (void)crc_orig;
    (void)crc_decomp;
    // API sequence test completed successfully
    return 66;
}