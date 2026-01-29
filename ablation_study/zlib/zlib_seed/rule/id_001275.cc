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
//<ID> 1275
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
    uLong checksum = adler32(0L, src, (uInt)SRC_LEN);
    (void)checksum;

    // step 2: Initialize deflate, compress the source into a buffer
    const char *ver = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong comp_bound = compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    memset(comp, 0, (size_t)comp_bound);
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)SRC_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)comp_bound;
    deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;
    (void)comp_size;
    deflateEnd(&dstrm);

    // step 3: Write original data to a gzip file, then read it back with gzread
    gzFile gw = gzopen("test_zlib_api_sequence_temp.gz", "wb");
    gzwrite(gw, src, (unsigned int)SRC_LEN);
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_temp.gz", "rb");
    unsigned int read_len = (unsigned int)SRC_LEN;
    Bytef *readbuf = (Bytef *)malloc((size_t)read_len);
    memset(readbuf, 0, (size_t)read_len);
    int r = gzread(gr, readbuf, read_len);
    (void)r;
    gzclose(gr);

    // step 4: Initialize inflate, set a dictionary, finalize inflate backend and cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateSetDictionary(&istrm, src, (uInt)32);
    inflateBackEnd(&istrm);
    inflateEnd(&istrm);

    free(src);
    free(comp);
    free(readbuf);
    // API sequence test completed successfully
    return 66;
}