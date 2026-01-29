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
//<ID> 979
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
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    Bytef *readBuf = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(readBuf, 0, (size_t)ORIG_LEN);

    // step 2: Write original data to a gzip file and close writer with gzclose_w
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    gzclose_w(gw);

    // step 3: Compress in-memory then initialize inflate, perform inflate and validate
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
    inflate(&istrm, 0);
    inflateValidate(&istrm, 1);
    inflateEnd(&istrm);

    // step 4: Read back from gzip file with gzread, validate and cleanup
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    int bytes_read = gzread(gr, readBuf, (unsigned int)ORIG_LEN);
    gzclose(gr);
    uLong adler_orig = adler32(0L, orig, (uInt)ORIG_LEN);
    uLong adler_read = adler32(0L, readBuf, (uInt)ORIG_LEN);
    uLong adler_decomp = adler32(0L, decomp, (uInt)ORIG_LEN);
    free(orig);
    free(comp);
    free(decomp);
    free(readBuf);
    (void)ver;
    (void)bytes_read;
    (void)adler_orig;
    (void)adler_read;
    (void)adler_decomp;
    // API sequence test completed successfully
    return 66;
}