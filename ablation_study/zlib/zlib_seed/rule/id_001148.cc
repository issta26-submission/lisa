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
//<ID> 1148
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare original and compressed buffers
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'B', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_size = (uLongf)bound;
    compress2(comp, &comp_size, orig, ORIG_LEN, 6);

    // step 2: Initialize inflate, configure stream, and perform decompression
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

    // step 3: Write decompressed data to a gzip file, then read it back using gzfread and clear errors
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gf, decomp, (unsigned int)ORIG_LEN);
    gzclose(gf);
    gzFile gfr = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *readbuf = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(readbuf, 0, (size_t)ORIG_LEN);
    gzfread((voidp)readbuf, (z_size_t)1, (z_size_t)ORIG_LEN, gfr);
    gzclearerr(gfr);
    gzclose(gfr);

    // step 4: Validate checksums, initialize and teardown inflateBack, finalize and cleanup
    uLong crc_orig = crc32(0UL, orig, (uInt)ORIG_LEN);
    uLong crc_read = crc32(0UL, readbuf, (uInt)ORIG_LEN);
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(backstrm));
    int windowBits = 15;
    unsigned char *window = (unsigned char *)malloc((size_t)(1u << windowBits));
    memset(window, 0, (size_t)(1u << windowBits));
    inflateBackInit_(&backstrm, windowBits, window, ver, (int)sizeof(z_stream));
    inflateBackEnd(&backstrm);
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(decomp);
    free(readbuf);
    free(window);
    (void)crc_orig;
    (void)crc_read;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}