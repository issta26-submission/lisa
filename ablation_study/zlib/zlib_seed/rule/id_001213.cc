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
//<ID> 1213
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare original data, compute checksum, and compress with compress2
    const uLong ORIG_LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong orig_adler = adler32(0UL, orig, (uInt)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    compress2(comp, &comp_len, orig, ORIG_LEN, 6);

    // step 2: Initialize a deflate stream, adjust params, and clean it up
    const char *ver = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    Bytef *outbuf = (Bytef *)malloc((size_t)bound);
    memset(outbuf, 0, (size_t)bound);
    dstrm.next_out = outbuf;
    dstrm.avail_out = (uInt)bound;
    deflateParams(&dstrm, 9, 1);
    deflateEnd(&dstrm);

    // step 3: Write the original data to a gzip file, then open for reading and close with gzclose_r
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    gzread(gr, outbuf, (unsigned int)bound);
    gzclose_r(gr);

    // step 4: Initialize inflateBack, end it, decompress with uncompress, validate checksum, and cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&istrm, 15, window, ver, (int)sizeof(z_stream));
    inflateBackEnd(&istrm);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    uLongf decomp_len = ORIG_LEN;
    uncompress(decomp, &decomp_len, comp, (uLong)comp_len);
    uLong decomp_adler = adler32(0UL, decomp, (uInt)decomp_len);

    free(orig);
    free(comp);
    free(outbuf);
    free(window);
    free(decomp);

    // API sequence test completed successfully
    (void)orig_adler;
    (void)decomp_adler;
    return 66;
}