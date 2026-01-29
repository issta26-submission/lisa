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
//<ID> 598
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
static unsigned int test_zlib_in_func(void *desc, unsigned char **buf) {
    (void)desc;
    *buf = NULL;
    return 0;
}
static int test_zlib_out_func(void *desc, unsigned char *buf, unsigned int len) {
    (void)desc;
    (void)buf;
    (void)len;
    return 0;
}

int test_zlib_api_sequence() {
    z_stream src = {0};
    z_stream dst = {0};
    const char *ver = zlibVersion();
    uLong flags = zlibCompileFlags();
    (void)flags;

    inflateInit_(&src, ver, (int)sizeof(z_stream));
    inflateCopy(&dst, &src);

    inflateBack(&src, (in_func)test_zlib_in_func, NULL, (out_func)test_zlib_out_func, NULL);

    gzFile gf = gzdopen(1, "wb");
    gzputc(gf, 'A');
    gzclose(gf);

    inflateEnd(&dst);
    inflateEnd(&src);

    return 66;
}