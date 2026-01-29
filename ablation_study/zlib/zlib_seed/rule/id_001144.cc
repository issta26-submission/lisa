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
//<ID> 1144
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare original data and compressed buffer
    const uLong ORIG_LEN = 128UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'X', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_size = (uLongf)bound;
    compress2(comp, &comp_size, orig, ORIG_LEN, 6);
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    gzclose(gw);

    // step 2: Initialize inflate stream and perform inflate on raw compressed buffer
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
    inflateEnd(&istrm);

    // step 3: Read back the gz file using gzfread and clear any errors
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *readbuf = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(readbuf, 0, (size_t)ORIG_LEN);
    gzfread((voidp)readbuf, (z_size_t)1, (z_size_t)ORIG_LEN, gr);
    gzclearerr(gr);
    gzclose(gr);
    uLong crc_orig = crc32(0UL, orig, (uInt)ORIG_LEN);
    uLong crc_read = crc32(0UL, readbuf, (uInt)ORIG_LEN);
    (void)crc_orig;
    (void)crc_read;

    // step 4: Demonstrate inflateBackInit_ / inflateBackEnd lifecycle and cleanup
    unsigned char *window = (unsigned char *)malloc((size_t)32768);
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(backstrm));
    inflateBackInit_(&backstrm, 15, window, ver, (int)sizeof(z_stream));
    inflateBackEnd(&backstrm);
    free(window);
    free(orig);
    free(comp);
    free(decomp);
    free(readbuf);

    // API sequence test completed successfully
    return 66;
}