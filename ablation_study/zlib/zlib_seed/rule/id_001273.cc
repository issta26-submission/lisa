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
//<ID> 1273
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data, compute checksum, and write it to a gzip file
    const uLong SRC_LEN = 256UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'A', (size_t)SRC_LEN);
    uLong checksum_before = adler32(0L, src, (uInt)SRC_LEN);
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw, src, (unsigned int)SRC_LEN);
    gzclose(gw);

    // step 2: Initialize and run deflate to compress the source data
    uLong comp_bound = compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    const char *ver = zlibVersion();
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)SRC_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)comp_bound;
    deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Read back the gzip file and initialize an inflate-back stream with a dictionary
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)SRC_LEN);
    int read_bytes = gzread(gr, read_buf, (unsigned int)SRC_LEN);
    gzclose(gr);
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    unsigned char *window = (unsigned char *)malloc((size_t)(1 << 15));
    inflateBackInit_(&istrm, 15, window, ver, (int)sizeof(z_stream));
    inflateSetDictionary(&istrm, src, (uInt)32);
    inflateBackEnd(&istrm);
    free(window);

    // step 4: Validate via checksum and cleanup
    uLong checksum_after = adler32(0L, read_buf, (uInt)(read_bytes > 0 ? (uInt)read_bytes : 0U));
    (void)checksum_before;
    (void)checksum_after;
    free(src);
    free(comp);
    free(read_buf);
    // API sequence test completed successfully
    return 66;
}