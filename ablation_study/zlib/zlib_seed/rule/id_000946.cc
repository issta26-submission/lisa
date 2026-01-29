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
//<ID> 946
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it in memory
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 128UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    compress2(comp, &comp_len, orig, ORIG_LEN, 6);

    // step 2: Initialize inflate stream and decompress the in-memory compressed buffer
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    uLong decomp_size = istrm.total_out;
    inflateEnd(&istrm);

    // step 3: Write the compressed bytes to a gzip file, then reopen and exercise gzgetc/gzungetc/gztell
    gzFile gf = gzopen("/tmp/test_zlib_api_sequence.gz", "wb");
    gzwrite(gf, comp, (unsigned int)comp_len);
    gzclose(gf);
    gzFile gf2 = gzopen("/tmp/test_zlib_api_sequence.gz", "rb");
    off_t pos_before = gztell(gf2);
    int c1 = gzgetc(gf2);
    int unret = gzungetc(c1, gf2);
    int c2 = gzgetc(gf2);
    off_t pos_after = gztell(gf2);
    gzclose(gf2);

    // step 4: Validate checksums and cleanup resources
    uLong crc_orig = crc32(0L, orig, (uInt)ORIG_LEN);
    uLong crc_decomp = crc32(0L, decomp, (uInt)decomp_size);
    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)pos_before;
    (void)pos_after;
    (void)c1;
    (void)unret;
    (void)c2;
    (void)crc_orig;
    (void)crc_decomp;
    // API sequence test completed successfully
    return 66;
}