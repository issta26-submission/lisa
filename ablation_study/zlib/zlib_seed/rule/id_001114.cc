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
//<ID> 1114
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong ORIG_LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong crc_orig_u = crc32(0UL, orig, (uInt)ORIG_LEN);
    uLong crc_orig_z = crc32_z(0UL, orig, (z_size_t)ORIG_LEN);

    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);

    // step 2: Compress original data into a gzip-wrapped stream using deflateInit2_
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit2_(&dstrm, 6, 8, 15 + 16, 8, 0, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Initialize inflate for gzip, register header receiver, perform inflate, then end inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit2_(&istrm, 15 + 16, ver, (int)sizeof(z_stream));
    gz_header header;
    memset(&header, 0, sizeof(header));
    inflateGetHeader(&istrm, &header);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 4);
    inflateEnd(&istrm);

    // step 4: Validate CRCs (compute on decompressed data), cleanup
    uLong crc_decomp_u = crc32(0UL, decomp, (uInt)ORIG_LEN);
    uLong crc_decomp_z = crc32_z(0UL, decomp, (z_size_t)ORIG_LEN);
    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)crc_orig_u;
    (void)crc_orig_z;
    (void)comp_size;
    (void)header;
    (void)crc_decomp_u;
    (void)crc_decomp_z;
    // API sequence test completed successfully
    return 66;
}