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
//<ID> 1115
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare data buffers
    const uLong ORIG_LEN = 1024UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'X', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong crc_orig_1 = crc32(0L, orig, (uInt)ORIG_LEN);
    uLong crc_orig_2 = crc32_z(0L, orig, (z_size_t)ORIG_LEN);

    // step 2: Compress original data into a gzip-format buffer using deflateInit2_
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit2_(&dstrm, 6, Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY, ver, (int)sizeof(z_stream));
    uLong comp_bound = deflateBound(&dstrm, ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    memset(comp, 0, (size_t)comp_bound);
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)comp_bound;
    deflate(&dstrm, Z_FINISH);
    uLong comp_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Initialize inflate with gzip header parsing, attach gz_header, inflate to decompress, compute CRCs, then end inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit2_(&istrm, 15 + 16, ver, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(head));
    inflateGetHeader(&istrm, &head);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, Z_FINISH);
    uLong crc_decomp_1 = crc32(0L, decomp, (uInt)ORIG_LEN);
    uLong crc_decomp_2 = crc32_z(0L, decomp, (z_size_t)ORIG_LEN);
    inflateEnd(&istrm);

    // step 4: Validate flow and cleanup
    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)crc_orig_1;
    (void)crc_orig_2;
    (void)crc_decomp_1;
    (void)crc_decomp_2;
    // API sequence test completed successfully
    return 66;
}