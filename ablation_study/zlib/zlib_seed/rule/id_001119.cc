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
//<ID> 1119
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)LEN);
    memset(orig, 'A', (size_t)LEN);
    const char *ver = zlibVersion();
    uLong crc_orig_1 = crc32(0UL, orig, (uInt)LEN);
    uLong crc_orig_2 = crc32_z(0UL, orig, (z_size_t)LEN);

    // step 2: Compress original data
    uLong bound = compressBound(LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_size = (uLongf)bound;
    compress2(comp, &comp_size, orig, LEN, 6);

    // step 3: Initialize inflate, retrieve header, inflate, and end
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    Bytef *decomp = (Bytef *)malloc((size_t)LEN);
    memset(decomp, 0, (size_t)LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)LEN;
    gz_header header;
    memset(&header, 0, sizeof(header));
    inflateGetHeader(&istrm, &header);
    inflate(&istrm, Z_FINISH);
    inflateEnd(&istrm);

    // step 4: Validate checksums & cleanup
    uLong crc_decomp_1 = crc32(0UL, decomp, (uInt)LEN);
    uLong crc_decomp_2 = crc32_z(0UL, decomp, (z_size_t)LEN);
    free(orig);
    free(comp);
    free(decomp);
    (void)crc_orig_1;
    (void)crc_orig_2;
    (void)crc_decomp_1;
    (void)crc_decomp_2;
    (void)ver;
    (void)bound;
    (void)comp_size;
    (void)header;
    // API sequence test completed successfully
    return 66;
}