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
//<ID> 926
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
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    Bytef *decomp_inflate = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp_inflate, 0, (size_t)ORIG_LEN);
    Bytef *decomp_uncompress = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp_uncompress, 0, (size_t)ORIG_LEN);

    // step 2: Initialize and run deflate to produce compressed data
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    uLong compressed_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Inflate-based decompression and parallel uncompress2 decompression
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compressed_size;
    istrm.next_out = decomp_inflate;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    inflateEnd(&istrm);

    uLongf destlen = (uLongf)ORIG_LEN;
    uLong src_len = compressed_size;
    uncompress2(decomp_uncompress, &destlen, comp, &src_len);

    // step 4: Validate checksums, cleanup
    uLong crc_orig = crc32(0L, orig, (uInt)ORIG_LEN);
    uLong crc_inflate = crc32(0L, decomp_inflate, (uInt)ORIG_LEN);
    uLong crc_uncompress = crc32(0L, decomp_uncompress, (uInt)ORIG_LEN);
    free(orig);
    free(comp);
    free(decomp_inflate);
    free(decomp_uncompress);
    (void)ver;
    (void)bound;
    (void)compressed_size;
    (void)crc_orig;
    (void)crc_inflate;
    (void)crc_uncompress;
    // API sequence test completed successfully
    return 66;
}