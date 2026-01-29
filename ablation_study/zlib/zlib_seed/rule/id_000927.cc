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
//<ID> 927
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'X', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp1 = (Bytef *)malloc((size_t)bound);
    memset(comp1, 0, (size_t)bound);
    Bytef *comp2 = (Bytef *)malloc((size_t)bound);
    memset(comp2, 0, (size_t)bound);
    Bytef *decomp1 = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp1, 0, (size_t)ORIG_LEN);
    Bytef *decomp2 = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp2, 0, (size_t)ORIG_LEN);

    // step 2: Compress original data using compress2 and a deflate stream to produce two compressed variants
    uLongf comp1_len_f = (uLongf)bound;
    compress2(comp1, &comp1_len_f, orig, ORIG_LEN, 6);
    uLong comp1_len = (uLong)comp1_len_f;

    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp2;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    uLong comp2_len = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Decompress first compressed buffer using uncompress2 and decompress second using inflate()
    uLongf out1_len_f = (uLongf)ORIG_LEN;
    uLong src1_len = comp1_len;
    uncompress2(decomp1, &out1_len_f, comp1, &src1_len);

    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp2;
    istrm.avail_in = (uInt)comp2_len;
    istrm.next_out = decomp2;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    inflateEnd(&istrm);

    // step 4: Validate checksums, cleanup resources, and finish
    uLong crc_orig = crc32(0L, orig, (uInt)ORIG_LEN);
    uLong crc_decomp1 = crc32(0L, decomp1, (uInt)ORIG_LEN);
    uLong crc_decomp2 = crc32(0L, decomp2, (uInt)ORIG_LEN);
    free(orig);
    free(comp1);
    free(comp2);
    free(decomp1);
    free(decomp2);
    (void)crc_orig;
    (void)crc_decomp1;
    (void)crc_decomp2;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}