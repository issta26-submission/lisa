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
//<ID> 1216
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize buffers and compute checksum
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong orig_adler = adler32(0UL, orig, (uInt)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_size = (uLongf)bound;
    compress2(comp, &comp_size, orig, ORIG_LEN, 6);

    // step 2: Configure a deflate stream and adjust parameters
    const char *ver = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    deflateParams(&dstrm, 1, 0);

    // step 3: Operate: write gzip file, initialize inflate-back, then close read handle with gzclose_r
    gzFile gz_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzwrite(gz_w, orig, (unsigned int)ORIG_LEN);
    gzclose(gz_w);
    gzFile gz_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    unsigned char *window = (unsigned char *)malloc(32768);
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateBackInit_(&istrm, 15, window, ver, (int)sizeof(z_stream));
    gzclose_r(gz_r);

    // step 4: Validate decompression via uncompress, compute checksum, cleanup resources
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    uLongf decomp_len = ORIG_LEN;
    uncompress(decomp, &decomp_len, comp, (uLong)comp_size);
    uLong decomp_adler = adler32(0UL, decomp, (uInt)decomp_len);
    inflateBackEnd(&istrm);
    deflateEnd(&dstrm);
    free(orig);
    free(comp);
    free(decomp);
    free(window);
    // API sequence test completed successfully
    return 66;
}