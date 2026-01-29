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
//<ID> 1146
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize buffers
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'X', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_size = (uLongf)bound;
    compress2(comp, &comp_size, orig, ORIG_LEN, 6);

    // step 2: Write compressed bytes into a gzip file and read them back
    gzFile gf_w = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gf_w, comp, (unsigned int)comp_size);
    gzclose(gf_w);
    gzFile gf_r = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)comp_size);
    memset(read_buf, 0, (size_t)comp_size);
    z_size_t bytes_read = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)comp_size, gf_r);
    gzclearerr(gf_r);

    // step 3: Initialize inflate, perform decompression using inflate, call inflateBackEnd
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)bytes_read;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, Z_FINISH);
    inflateBackEnd(&istrm);
    inflateEnd(&istrm);

    // step 4: Validate checksums, cleanup resources
    uLong crc_orig = crc32(0UL, orig, (uInt)ORIG_LEN);
    uLong crc_decomp = crc32(0UL, decomp, (uInt)ORIG_LEN);
    (void)crc_orig;
    (void)crc_decomp;
    gzclose(gf_r);
    free(orig);
    free(comp);
    free(read_buf);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}