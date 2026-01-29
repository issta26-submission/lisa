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
//<ID> 1008
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
    memset(orig, 'X', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    Bytef *decomp_stream = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp_stream, 0, (size_t)ORIG_LEN);
    Bytef *decomp_uncomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp_uncomp, 0, (size_t)ORIG_LEN);
    compress2(comp, &comp_len, orig, (uLong)ORIG_LEN, 6);

    // step 2: Initialize and configure an inflate stream, then prime it
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit2_(&istrm, 15, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = decomp_stream;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflatePrime(&istrm, 1, 1);
    inflateEnd(&istrm);

    // step 3: Operate: in-memory decompression using uncompress2
    uLongf dest_len_f = (uLongf)ORIG_LEN;
    uLong src_len_ul = (uLong)comp_len;
    uncompress2(decomp_uncomp, &dest_len_f, comp, &src_len_ul);

    // step 4: Validate via gzip file operations and cleanup
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    gzclose_w(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    gzclearerr(gr);
    gzclose(gr);
    free(orig);
    free(comp);
    free(decomp_stream);
    free(decomp_uncomp);
    (void)ver;
    (void)dest_len_f;
    (void)src_len_ul;
    // API sequence test completed successfully
    return 66;
}