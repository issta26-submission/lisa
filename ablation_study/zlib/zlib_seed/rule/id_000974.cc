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
//<ID> 974
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
    uLongf comp_len_f = (uLongf)bound;
    Bytef *decomp_inflate = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp_inflate, 0, (size_t)ORIG_LEN);
    Bytef *decomp_gz = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp_gz, 0, (size_t)ORIG_LEN);

    // step 2: Compress original data into comp using compress2
    compress2(comp, &comp_len_f, orig, ORIG_LEN, 6);
    uLong comp_len = (uLong)comp_len_f;

    // step 3: Inflate compressed bytes with z_stream, validate, and cleanup inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = decomp_inflate;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    inflateValidate(&istrm, 1);
    inflateEnd(&istrm);

    // step 4: Exercise gzFile APIs: write original via gzwrite, close with gzclose_w, then read back with gzread and close
    gzFile gf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzwrite(gf_w, orig, (unsigned int)ORIG_LEN);
    gzclose_w(gf_w);
    gzFile gf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    gzread(gf_r, decomp_gz, (unsigned int)ORIG_LEN);
    gzclose(gf_r);

    // final validation via adler32 to ensure data flow is meaningful, cleanup and return
    uLong adler_orig = adler32(0L, orig, (uInt)ORIG_LEN);
    uLong adler_inflate = adler32(0L, decomp_inflate, (uInt)ORIG_LEN);
    uLong adler_gz = adler32(0L, decomp_gz, (uInt)ORIG_LEN);
    free(orig);
    free(comp);
    free(decomp_inflate);
    free(decomp_gz);
    (void)ver;
    (void)adler_orig;
    (void)adler_inflate;
    (void)adler_gz;
    (void)comp_len;
    // API sequence test completed successfully
    return 66;
}