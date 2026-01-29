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
//<ID> 1118
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'X', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong crc_before = crc32(0UL, orig, (uInt)ORIG_LEN);
    uLong crc_before_z = crc32_z(crc_before, orig, (z_size_t)ORIG_LEN);

    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);

    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;

    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;

    // step 2: Compress original data using deflate
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, Z_FINISH);
    uLong comp_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Initialize inflate (with gzip auto-detect), attach header retrieval, and inflate
    inflateInit2_(&istrm, 15 + 32, ver, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(head));
    inflateGetHeader(&istrm, &head);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, Z_FINISH);
    uLong decompressed_bytes = istrm.total_out;
    uLong crc_after = crc32(0UL, decomp, (uInt)decompressed_bytes);
    uLong crc_after_z = crc32_z(crc_after, decomp, (z_size_t)decompressed_bytes);
    inflateEnd(&istrm);

    // step 4: Cleanup
    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)crc_before;
    (void)crc_before_z;
    (void)crc_after;
    (void)crc_after_z;
    (void)decompressed_bytes;
    // API sequence test completed successfully
    return 66;
}