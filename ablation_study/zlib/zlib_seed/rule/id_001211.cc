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
//<ID> 1211
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare buffers and compute adler32 checksum, then compress original data
    const uLong ORIG_LEN = 128UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong orig_adler = adler32(0UL, orig, (uInt)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    uLongf comp_len = (uLongf)bound;
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    compress2(comp, &comp_len, orig, ORIG_LEN, 6);

    // step 2: Initialize a deflate stream and adjust its parameters
    const char *ver = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    deflateParams(&dstrm, 9, 1);

    // step 3: Initialize inflateBack and then properly end it
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    unsigned char window[32];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&istrm, 15, window, ver, (int)sizeof(z_stream));
    inflateBackEnd(&istrm);

    // step 4: Write original data to a gzip file, reopen for reading and close read handle, then cleanup
    gzFile gzw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzwrite(gzw, orig, (unsigned int)ORIG_LEN);
    gzclose(gzw);
    gzFile gzr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    char peekbuf[1];
    gzread(gzr, peekbuf, 0);
    gzclose_r(gzr);
    deflateEnd(&dstrm);
    free(orig);
    free(comp);
    // API sequence test completed successfully
    return 66;
}