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
//<ID> 928
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 1024UL;
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

    // step 2: Compress using compress2 and then uncompress2
    uLongf comp1_len_f = (uLongf)bound;
    compress2(comp1, &comp1_len_f, orig, ORIG_LEN, 6);
    uLong comp1_src_len = (uLong)comp1_len_f;
    uLongf decomp1_len_f = (uLongf)ORIG_LEN;
    uncompress2(decomp1, &decomp1_len_f, comp1, &comp1_src_len);

    // step 3: Compress using deflate APIs and then inflate the result
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
    uLong comp2_size = dstrm.total_out;
    deflateEnd(&dstrm);

    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp2;
    istrm.avail_in = (uInt)comp2_size;
    istrm.next_out = decomp2;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    inflateEnd(&istrm);

    // step 4: Validate basic flow, cleanup and finish
    (void)decomp1_len_f;
    (void)decomp2;
    free(orig);
    free(comp1);
    free(comp2);
    free(decomp1);
    free(decomp2);
    // API sequence test completed successfully
    return 66;
}