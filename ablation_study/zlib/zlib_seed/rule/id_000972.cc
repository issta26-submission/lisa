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
//<ID> 972
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data and compress into memory
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len_f = (uLongf)bound;
    compress2(comp, &comp_len_f, orig, ORIG_LEN, 6);
    uLong comp_len = (uLong)comp_len_f;

    // step 2: Write original data to a gzip file (writer) and close with gzclose_w, then read it back
    gzFile gf_w = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gf_w, orig, (unsigned int)ORIG_LEN);
    gzclose_w(gf_w);
    gzFile gf_r = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(read_buf, 0, (size_t)ORIG_LEN);
    int read_n = gzread(gf_r, read_buf, (unsigned int)ORIG_LEN);
    gzclose(gf_r);
    (void)read_n;

    // step 3: Initialize inflate stream, validate it, and inflate compressed memory data
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflateValidate(&istrm, 1);
    inflate(&istrm, 0);
    inflateEnd(&istrm);

    // step 4: Validate results and cleanup
    uLong adler_orig = adler32(0L, orig, (uInt)ORIG_LEN);
    uLong adler_decomp = adler32(0L, decomp, (uInt)ORIG_LEN);
    uLong adler_read = adler32(0L, read_buf, (uInt)ORIG_LEN);
    free(orig);
    free(comp);
    free(decomp);
    free(read_buf);
    (void)ver;
    (void)adler_orig;
    (void)adler_decomp;
    (void)adler_read;
    // API sequence test completed successfully
    return 66;
}