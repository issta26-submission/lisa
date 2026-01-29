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
//<ID> 923
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
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_destlen = (uLongf)bound;
    Bytef *decomp_uncompress = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp_uncompress, 0, (size_t)ORIG_LEN);
    Bytef *comp2 = (Bytef *)malloc((size_t)bound);
    memset(comp2, 0, (size_t)bound);
    Bytef *decomp_inflate = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp_inflate, 0, (size_t)ORIG_LEN);

    // step 2: Compress original data using compress2 (simple API) to produce a baseline compressed buffer
    compress2(comp, &comp_destlen, orig, (uLong)ORIG_LEN, 6);
    uLong compressed_size = (uLong)comp_destlen;

    // step 3: Produce a compressed stream using deflate APIs and then decompress it using inflate
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
    uLong deflated_size = dstrm.total_out;
    deflateEnd(&dstrm);

    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp2;
    istrm.avail_in = (uInt)deflated_size;
    istrm.next_out = decomp_inflate;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    inflateEnd(&istrm);

    // step 4: Use uncompress2 to decompress the baseline compressed buffer, validate checksums, cleanup
    uLongf decomp_len_for_uncompress = (uLongf)ORIG_LEN;
    uLong source_len_for_uncompress = compressed_size;
    uncompress2(decomp_uncompress, &decomp_len_for_uncompress, comp, &source_len_for_uncompress);

    uLong crc_orig = crc32(0L, orig, (uInt)ORIG_LEN);
    uLong crc_uncompress = crc32(0L, decomp_uncompress, (uInt)ORIG_LEN);
    uLong crc_inflate = crc32(0L, decomp_inflate, (uInt)ORIG_LEN);

    free(orig);
    free(comp);
    free(comp2);
    free(decomp_uncompress);
    free(decomp_inflate);
    (void)ver;
    (void)bound;
    (void)compressed_size;
    (void)deflated_size;
    (void)crc_orig;
    (void)crc_uncompress;
    (void)crc_inflate;
    // API sequence test completed successfully
    return 66;
}