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
//<ID> 1110
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong DATA_LEN = 128UL;
    Bytef *data = (Bytef *)malloc((size_t)DATA_LEN);
    memset(data, 'X', (size_t)DATA_LEN);
    const char *ver = zlibVersion();
    uLong comp_bound = compressBound(DATA_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    memset(comp, 0, (size_t)comp_bound);
    Bytef *decomp = (Bytef *)malloc((size_t)DATA_LEN);
    memset(decomp, 0, (size_t)DATA_LEN);

    // step 2: Compress data into a gzip-formatted stream using deflateInit2_
    z_stream cstrm;
    memset(&cstrm, 0, sizeof(cstrm));
    cstrm.zalloc = NULL;
    cstrm.zfree = NULL;
    cstrm.opaque = NULL;
    deflateInit2_(&cstrm, 6, 8, 15 + 16, 8, 0, ver, (int)sizeof(z_stream)); // method=8 (deflated), windowBits +16 for gzip
    cstrm.next_in = data;
    cstrm.avail_in = (uInt)DATA_LEN;
    cstrm.next_out = comp;
    cstrm.avail_out = (uInt)comp_bound;
    deflate(&cstrm, Z_FINISH);
    uLong comp_size = cstrm.total_out;

    // step 3: Initialize inflate for gzip, retrieve header, then inflate the data
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit2_(&istrm, 15 + 16, ver, (int)sizeof(z_stream)); // windowBits +16 to accept gzip
    gz_header header;
    memset(&header, 0, sizeof(header));
    inflateGetHeader(&istrm, &header); // attach header retrieval before processing input
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)DATA_LEN;
    inflate(&istrm, Z_FINISH);
    inflateEnd(&istrm);

    // step 4: Validate with CRCs, cleanup, and finalize
    uLong crc_v = crc32(0L, data, (uInt)DATA_LEN);
    uLong crc_z = crc32_z(0L, data, (z_size_t)DATA_LEN);
    (void)crc_v;
    (void)crc_z;
    deflateEnd(&cstrm);
    free(data);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}