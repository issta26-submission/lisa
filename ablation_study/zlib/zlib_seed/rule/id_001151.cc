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
//<ID> 1151
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare original buffer and metadata
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong adler_orig = adler32(0UL, orig, (uInt)ORIG_LEN);
    uLong crc_orig = crc32(0UL, orig, (uInt)ORIG_LEN);

    // step 2: Compress original data
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_size = (uLongf)bound;
    compress2(comp, &comp_size, orig, ORIG_LEN, 6);

    // step 3: Initialize inflate, perform decompression, and demonstrate dictionary APIs
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, Z_FINISH);
    Bytef dict_buf[32];
    memset(dict_buf, 'D', sizeof(dict_buf));
    inflateSetDictionary(&istrm, dict_buf, (uInt)sizeof(dict_buf));
    inflateUndermine(&istrm, 1);
    uLong adler_decomp = adler32(0UL, decomp, (uInt)ORIG_LEN);
    uLong crc_decomp = crc32(0UL, decomp, (uInt)ORIG_LEN);

    // step 4: Write and read a gzip file line to exercise gzgets, then cleanup
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzputs(gw, ver);
    gzputs(gw, "\n");
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    char linebuf[128];
    memset(linebuf, 0, sizeof(linebuf));
    gzgets(gr, linebuf, (int)sizeof(linebuf));
    gzclose(gr);
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(decomp);
    (void)adler_orig;
    (void)crc_orig;
    (void)adler_decomp;
    (void)crc_decomp;
    (void)ver;
    (void)linebuf;
    // API sequence test completed successfully
    return 66;
}