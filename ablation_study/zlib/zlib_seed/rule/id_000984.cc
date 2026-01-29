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
//<ID> 984
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 64UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    uLong crc_orig = crc32(0L, orig, (uInt)ORIG_LEN);

    // step 2: Write a small gzip file using gzputc
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzputc(gw, 'H');
    gzputc(gw, 'i');
    gzputc(gw, '!');
    gzputc(gw, '\n');
    gzclose_w(gw);

    // step 3: Compress in-memory, initialize inflate, reset streams, and inflate
    compress2(comp, &comp_len, orig, (uLong)ORIG_LEN, 6);
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflateReset(&istrm);
    inflate(&istrm, 0);
    inflateResetKeep(&istrm);
    inflateEnd(&istrm);

    // step 4: Read back from gzip file with gzgetc_, cleanup and validate
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int first_char = gzgetc_(gr);
    gzclose(gr);
    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)crc_orig;
    (void)first_char;
    // API sequence test completed successfully
    return 66;
}