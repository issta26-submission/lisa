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
//<ID> 925
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 300UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong bound1 = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound1);
    memset(comp, 0, (size_t)bound1);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);

    // step 2: Compress with compress2 and then uncompress2 to validate round-trip
    uLongf comp_len_f = (uLongf)bound1;
    compress2(comp, &comp_len_f, orig, ORIG_LEN, 6);
    uLong comp_len = (uLong)comp_len_f;
    uLongf dest_len_f = (uLongf)ORIG_LEN;
    uLong src_len_for_uncompress = comp_len;
    uncompress2(decomp, &dest_len_f, comp, &src_len_for_uncompress);

    // step 3: Create a deflate stream, produce another compressed buffer, then inflate it
    uLong bound2 = compressBound(ORIG_LEN);
    Bytef *comp2 = (Bytef *)malloc((size_t)bound2);
    memset(comp2, 0, (size_t)bound2);
    Bytef *decomp2 = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp2, 0, (size_t)ORIG_LEN);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp2;
    dstrm.avail_out = (uInt)bound2;
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

    // step 4: Cleanup
    free(orig);
    free(comp);
    free(decomp);
    free(comp2);
    free(decomp2);
    (void)ver;
    (void)comp_len;
    (void)dest_len_f;
    // API sequence test completed successfully
    return 66;
}