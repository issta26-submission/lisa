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
//<ID> 1277
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data and compute checksum
    const uLong SRC_LEN = 256UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'Z', (size_t)SRC_LEN);
    uLong checksum = adler32(0UL, src, (uInt)SRC_LEN);

    // step 2: Compress data with deflate after initializing stream
    uLong comp_bound = compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    memset(comp, 0, (size_t)comp_bound);
    const char *ver = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)SRC_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)comp_bound;
    deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Initialize inflate-back stream, set dictionary, and teardown
    z_stream iback;
    memset(&iback, 0, sizeof(iback));
    iback.zalloc = NULL;
    iback.zfree = NULL;
    iback.opaque = NULL;
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&iback, 15, window, ver, (int)sizeof(z_stream));
    inflateSetDictionary(&iback, src, (uInt)SRC_LEN);
    inflateBackEnd(&iback);
    free(window);

    // step 4: Write original data to a gzip file and read it back, then cleanup
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw, src, (unsigned int)SRC_LEN);
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *readbuf = (Bytef *)malloc((size_t)SRC_LEN);
    memset(readbuf, 0, (size_t)SRC_LEN);
    gzread(gr, readbuf, (unsigned int)SRC_LEN);
    gzclose(gr);

    free(src);
    free(comp);
    free(readbuf);

    // API sequence test completed successfully
    (void)checksum;
    (void)comp_size;
    return 66;
}