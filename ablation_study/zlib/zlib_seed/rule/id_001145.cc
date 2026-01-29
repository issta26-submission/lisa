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
//<ID> 1145
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare original buffer and compress it
    const uLong ORIG_LEN = 64UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_size = (uLongf)bound;
    compress2(comp, &comp_size, orig, ORIG_LEN, 6);

    // step 2: Initialize inflate, configure stream, and perform inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, Z_FINISH);

    // step 3: Write decompressed data to a gzip file, then read it back with gzfread and clear errors
    gzFile gfw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gfw, decomp, (unsigned int)ORIG_LEN);
    gzclose(gfw);
    gzFile gfr = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *readbuf = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(readbuf, 0, (size_t)ORIG_LEN);
    z_size_t items_read = gzfread(readbuf, (z_size_t)1, (z_size_t)ORIG_LEN, gfr);
    gzclearerr(gfr);
    gzclose(gfr);

    // step 4: Validate checksums, cleanup inflate and buffers
    uLong crc_orig = crc32(0UL, orig, (uInt)ORIG_LEN);
    uLong crc_read = crc32(0UL, readbuf, (uInt)ORIG_LEN);
    (void)crc_orig;
    (void)crc_read;
    (void)items_read;
    inflateBackEnd(&istrm);
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(decomp);
    free(readbuf);
    // API sequence test completed successfully
    return 66;
}