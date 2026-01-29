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
//<ID> 1217
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data, compute checksum, and compress
    const uLong ORIG_LEN = 320UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong orig_adler = adler32(0UL, orig, (uInt)ORIG_LEN);
    uLong comp_bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    memset(comp, 0, (size_t)comp_bound);
    uLongf comp_size = (uLongf)comp_bound;
    compress2(comp, &comp_size, orig, ORIG_LEN, 6);

    // step 2: Initialize a deflate stream and adjust parameters
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateParams(&dstrm, 9, 1);
    deflateEnd(&dstrm);

    // step 3: Initialize inflate-back stream (with a window) and tear it down
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    z_stream bstrm;
    memset(&bstrm, 0, sizeof(bstrm));
    bstrm.zalloc = NULL;
    bstrm.zfree = NULL;
    bstrm.opaque = NULL;
    inflateBackInit_(&bstrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    inflateBackEnd(&bstrm);

    // step 4: Decompress for validation, write/read a gzip file, cleanup
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    uLongf decomp_len = ORIG_LEN;
    uncompress(decomp, &decomp_len, comp, (uLong)comp_size);
    uLong decomp_adler = adler32(0UL, decomp, (uInt)decomp_len);
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    gzclose_r(gr);
    free(orig);
    free(comp);
    free(decomp);
    free(window);
    // API sequence test completed successfully
    return 66;
}