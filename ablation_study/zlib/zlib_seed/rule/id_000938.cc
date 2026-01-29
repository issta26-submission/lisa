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
//<ID> 938
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 1024UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len = bound;
    compress2(comp, &comp_len, orig, ORIG_LEN, 6);

    // step 2: Side-effect gzip write using gzFile
    gzFile gf = gzopen("/dev/null", "wb");
    gzwrite(gf, orig, (unsigned int)ORIG_LEN);
    gzclose(gf);

    // step 3: Initialize inflate stream and prepare header retrieval
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit2_(&istrm, 15 + 32, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    gz_header header;
    memset(&header, 0, sizeof(header));
    inflateGetHeader(&istrm, &header);

    // step 4: Operate, validate and cleanup
    inflate(&istrm, 0);
    inflateSyncPoint(&istrm);
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)bound;
    (void)comp_len;
    (void)header.done;
    // API sequence test completed successfully
    return 66;
}