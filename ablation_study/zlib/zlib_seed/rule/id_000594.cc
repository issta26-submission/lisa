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
//<ID> 594
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream src;
    memset(&src, 0, sizeof(src));
    z_stream dest;
    memset(&dest, 0, sizeof(dest));
    const char * ver = zlibVersion();
    uLong flags = zlibCompileFlags();
    (void)flags;
    Bytef * compBuf = (Bytef *)malloc((size_t)32);
    memset(compBuf, 0, (size_t)32);
    src.next_in = compBuf;
    src.avail_in = (uInt)32;

    // step 2: Initialize & copy stream state
    inflateInit_(&src, ver, (int)sizeof(z_stream));
    inflateCopy(&dest, &src);

    // step 3: Use inflateBack with simple callbacks and write a byte to a gz file
    in_func in_cb = static_cast<in_func>(+[](void * in_desc, unsigned char ** buf) -> unsigned int {
        *buf = (unsigned char *)in_desc;
        return (unsigned int)32;
    });
    out_func out_cb = static_cast<out_func>(+[](void * out_desc, unsigned char * buf, unsigned int len) -> int {
        (void)out_desc;
        (void)buf;
        (void)len;
        return 0;
    });
    inflateBack(&src, in_cb, compBuf, out_cb, NULL);
    gzFile gf = gzopen("tmp_zlib_api.gz", "wb");
    gzputc(gf, 'Z');
    gzclose(gf);

    // step 4: Cleanup
    inflateEnd(&src);
    inflateEnd(&dest);
    free(compBuf);
    // API sequence test completed successfully
    return 66;
}