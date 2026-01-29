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
//<ID> 1270
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data, compute checksum, and allocate buffers
    const uLong SRC_LEN = 128UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'A', (size_t)SRC_LEN);
    uLong src_adler = adler32(0L, src, (uInt)SRC_LEN);
    uLong comp_bound = compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    uLong read_len = SRC_LEN;
    Bytef *read_buf = (Bytef *)malloc((size_t)read_len);

    // step 2: Initialize and run deflate (compress) using deflateInit_ and deflate
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

    // step 3: Write original data to a gzip file, then read it back with gzread
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw, src, (unsigned int)SRC_LEN);
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    gzread(gr, read_buf, (unsigned int)read_len);
    gzclose(gr);

    // step 4: Initialize inflate-back stream, set dictionary from source, then cleanup
    unsigned char *window = (unsigned char *)malloc(32768);
    z_stream bstrm;
    memset(&bstrm, 0, sizeof(bstrm));
    bstrm.zalloc = NULL;
    bstrm.zfree = NULL;
    bstrm.opaque = NULL;
    inflateBackInit_(&bstrm, 15, window, ver, (int)sizeof(z_stream));
    inflateSetDictionary(&bstrm, src, (uInt)SRC_LEN);
    inflateBackEnd(&bstrm);

    free(src);
    free(comp);
    free(read_buf);
    free(window);
    // API sequence test completed successfully
    return 66;
}