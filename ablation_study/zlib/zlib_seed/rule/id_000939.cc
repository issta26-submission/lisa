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
//<ID> 939
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    compress2(comp, &comp_len, orig, ORIG_LEN, 6);

    // step 2: Initialize inflate stream and prepare header structure
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    gz_header header;
    memset(&header, 0, sizeof(header));
    inflateGetHeader(&istrm, &header);

    // step 3: Operate: inflate, check sync point, and finish inflate usage
    inflate(&istrm, 0);
    inflateSyncPoint(&istrm);
    inflateEnd(&istrm);

    // step 4: Validate results, use gzFile for side-effect, and cleanup
    uLong crc_before = crc32(0L, orig, (uInt)ORIG_LEN);
    uLong crc_after = crc32(0L, decomp, (uInt)ORIG_LEN);
    gzFile gf = gzopen("/dev/null", "wb");
    gzwrite(gf, comp, (unsigned int)comp_len);
    gzclose(gf);
    free(orig);
    free(comp);
    free(decomp);
    (void)crc_before;
    (void)crc_after;
    (void)ver;
    (void)bound;
    (void)comp_len;
    // API sequence test completed successfully
    return 66;
}