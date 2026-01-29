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
//<ID> 1276
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute checksum
    const uLong SRC_LEN = 256UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'X', (size_t)SRC_LEN);
    uLong src_adler = adler32(0UL, src, (uInt)SRC_LEN);

    // step 2: Initialize deflate stream and compress source
    const char *ver = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong comp_bound = compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)SRC_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)comp_bound;
    deflate(&dstrm, 4); /* Z_FINISH */
    uLong comp_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Write original data to a gzip file and read it back using gzread
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzwrite(gw, src, (unsigned int)SRC_LEN);
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    Bytef *readback = (Bytef *)malloc((size_t)SRC_LEN);
    int rb = gzread(gr, readback, (unsigned int)SRC_LEN);
    (void)rb;
    gzclose(gr);

    // step 4: Initialize inflate stream, set a dictionary, perform inflate, then cleanup including inflateBackEnd
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    /* configure a dictionary (use a prefix of the original source) */
    inflateSetDictionary(&istrm, src, (uInt)16);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    uLong decomp_len = SRC_LEN + 64;
    Bytef *decomp = (Bytef *)malloc((size_t)decomp_len);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)decomp_len;
    inflate(&istrm, 4); /* Z_FINISH */
    inflateBackEnd(&istrm);
    inflateEnd(&istrm);

    // cleanup
    free(src);
    free(comp);
    free(readback);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}