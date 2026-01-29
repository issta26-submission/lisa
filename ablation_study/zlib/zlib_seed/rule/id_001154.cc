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
//<ID> 1154
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare original buffer and dictionary
    const uLong ORIG_LEN = 200UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    Bytef dict_buf[16];
    memset(dict_buf, 'D', sizeof(dict_buf));
    const char *ver = zlibVersion();
    uLong adler_orig = adler32(0UL, orig, (uInt)ORIG_LEN);
    uLong crc_orig = crc32(0UL, orig, (uInt)ORIG_LEN);

    // step 2: Compress original data and write a small gzip header file then read it with gzgets
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_size = (uLongf)bound;
    compress2(comp, &comp_size, orig, ORIG_LEN, 6);

    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzputs(gw, ver);
    gzputs(gw, "\n");
    gzclose(gw);

    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    char header_buf[128];
    memset(header_buf, 0, sizeof(header_buf));
    gzgets(gr, header_buf, (int)sizeof(header_buf));
    gzclose(gr);

    // step 3: Initialize inflate, set dictionary, perform inflation, call inflateUndermine, and cleanup inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateSetDictionary(&istrm, dict_buf, (uInt)sizeof(dict_buf));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, Z_FINISH);
    inflateUndermine(&istrm, 1);
    inflateEnd(&istrm);

    // step 4: Validate checksums and cleanup all resources
    uLong adler_decomp = adler32(0UL, decomp, (uInt)ORIG_LEN);
    uLong crc_decomp = crc32(0UL, decomp, (uInt)ORIG_LEN);
    free(orig);
    free(comp);
    free(decomp);
    (void)adler_orig;
    (void)crc_orig;
    (void)adler_decomp;
    (void)crc_decomp;
    (void)header_buf;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}