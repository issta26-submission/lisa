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
//<ID> 929
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
    memset(orig, 'X', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len = bound;
    compress2(comp, &comp_len, orig, ORIG_LEN, 6);

    // step 2: Use uncompress2 to decompress the compressed buffer
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    uLongf decomp_len = ORIG_LEN;
    uLong source_len = (uLong)comp_len;
    uncompress2(decomp, &decomp_len, comp, &source_len);

    // step 3: Compress again via deflate and then call deflateEnd
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    Bytef *comp2 = (Bytef *)malloc((size_t)bound);
    memset(comp2, 0, (size_t)bound);
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp2;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    uLong comp2_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 4: Inflate the deflate-produced buffer and call inflateEnd
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    Bytef *decomp2 = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp2, 0, (size_t)ORIG_LEN);
    istrm.next_in = comp2;
    istrm.avail_in = (uInt)comp2_size;
    istrm.next_out = decomp2;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    inflateEnd(&istrm);

    uLong crc1 = crc32(0L, decomp, (uInt)ORIG_LEN);
    uLong crc2 = crc32(0L, decomp2, (uInt)ORIG_LEN);

    free(orig);
    free(comp);
    free(decomp);
    free(comp2);
    free(decomp2);
    (void)ver;
    (void)decomp_len;
    (void)source_len;
    (void)crc1;
    (void)crc2;
    // API sequence test completed successfully
    return 66;
}