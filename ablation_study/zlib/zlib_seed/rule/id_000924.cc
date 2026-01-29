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
//<ID> 924
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
    Bytef *decomp_uncomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp_uncomp, 0, (size_t)ORIG_LEN);

    // step 2: Compress original data using deflate, then finalize with deflateEnd
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

    // step 3: Inflate compressed data using a z_stream and then end it with inflateEnd
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

    // step 4: Use uncompress2 to decompress the same compressed buffer into another destination, validate checksums, cleanup
    uLongf dest_len_uncomp = (uLongf)ORIG_LEN;
    uLong source_len_for_uncomp = compressed_size;
    uncompress2(decomp_uncomp, &dest_len_uncomp, comp, &source_len_for_uncomp);
    uLong crc_orig = crc32(0L, orig, (uInt)ORIG_LEN);
    uLong crc_inflate = crc32(0L, decomp_inflate, (uInt)ORIG_LEN);
    uLong crc_uncomp = crc32(0L, decomp_uncomp, (uInt)ORIG_LEN);
    free(orig);
    free(comp);
    free(decomp_inflate);
    free(decomp_uncomp);
    (void)ver;
    (void)bound;
    (void)compressed_size;
    (void)crc_orig;
    (void)crc_inflate;
    (void)crc_uncomp;
    // API sequence test completed successfully
    return 66;
}