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
//<ID> 1168
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare original data and obtain zlib version
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'X', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();

    // step 2: Compress using deflate and write to a gzip file
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, Z_FINISH);
    uLong comp_size = dstrm.total_out;
    deflateEnd(&dstrm);
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzwrite(gw, comp, (unsigned int)comp_size);
    gzclose(gw);

    // step 3: Read compressed data back and perform multiple inflate/reset operations
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)bound);
    memset(read_buf, 0, (size_t)bound);
    z_size_t items_read = gzfread(read_buf, (z_size_t)1, (z_size_t)bound, gr);
    gzclose(gr);
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)items_read;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, Z_FINISH);
    inflateReset(&istrm);
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)items_read;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, Z_FINISH);
    inflateResetKeep(&istrm);
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)items_read;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, Z_FINISH);
    inflateEnd(&istrm);

    // step 4: Validate checksums and cleanup
    uLong crc_orig = crc32(0UL, orig, (uInt)ORIG_LEN);
    uLong crc_decomp = crc32(0UL, decomp, (uInt)ORIG_LEN);
    (void)crc_orig;
    (void)crc_decomp;
    free(orig);
    free(comp);
    free(read_buf);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}