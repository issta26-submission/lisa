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
//<ID> 1274
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data, compute checksum, and compress with deflate
    const uLong SRC_LEN = 256UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'A', (size_t)SRC_LEN);
    uLong src_adler = adler32(0UL, src, (uInt)SRC_LEN);

    uLong comp_bound = compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    memset(comp, 0, (size_t)comp_bound);

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

    // step 2: Create a gzip file with the original data, then read it back with gzread
    gzFile gw = gzopen64("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw, src, (unsigned int)SRC_LEN);
    gzclose(gw);
    gzFile gr = gzopen64("test_zlib_api_sequence.gz", "rb");
    Bytef *readbuf = (Bytef *)malloc((size_t)SRC_LEN);
    memset(readbuf, 0, (size_t)SRC_LEN);
    int read_bytes = gzread(gr, readbuf, (unsigned int)SRC_LEN);
    (void)read_bytes;
    gzclose(gr);

    // step 3: Initialize an inflate-back stream, set a dictionary derived from the source, then end back mode
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&istrm, 15, window, ver, (int)sizeof(z_stream));
    uInt dict_len = (uInt)16;
    Bytef *dict = (Bytef *)malloc((size_t)dict_len);
    memcpy(dict, src, (size_t)dict_len);
    inflateSetDictionary(&istrm, dict, dict_len);
    inflateBackEnd(&istrm);
    inflateEnd(&istrm);

    // step 4: Cleanup all resources
    free(window);
    free(dict);
    free(comp);
    free(src);
    free(readbuf);

    // API sequence test completed successfully
    return 66;
}