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
//<ID> 1045
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
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong flags = zlibCompileFlags();
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);

    // step 2: Initialize deflate and perform compression
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = NULL;
    strm.zfree = NULL;
    strm.opaque = NULL;
    deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    strm.next_in = orig;
    strm.avail_in = (uInt)ORIG_LEN;
    strm.next_out = comp;
    strm.avail_out = (uInt)bound;
    deflate(&strm, 4); /* Z_FINISH */
    uLong comp_size = strm.total_out;

    // step 3: Write a byte from the compressed output to a gzip file and adjust gz buffering
    gzFile gw = gzopen("test_zlib_api_sequence_temp.gz", "wb");
    gzbuffer(gw, 4096);
    gzputc(gw, (int)comp[0]);
    gzclose(gw);

    // step 4: Reset and cleanup deflate state and buffers
    deflateReset(&strm);
    deflateEnd(&strm);
    free(orig);
    free(comp);
    (void)ver;
    (void)flags;
    (void)comp_size;
    // API sequence test completed successfully
    return 66;
}