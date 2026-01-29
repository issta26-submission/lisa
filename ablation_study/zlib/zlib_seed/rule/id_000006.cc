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
//<ID> 6
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

static unsigned int my_in(void *in_desc, unsigned char **buf) {
    *buf = (unsigned char *)in_desc;
    return 0;
}

static int my_out(void *out_desc, unsigned char *buf, unsigned int len) {
    (void)out_desc;
    (void)buf;
    (void)len;
    return 0;
}

int test_zlib_api_sequence() {
    z_stream src;
    z_stream dest;
    z_stream def;
    memset(&src, 0, sizeof(src));
    memset(&dest, 0, sizeof(dest));
    memset(&def, 0, sizeof(def));

    gzFile gz = gzopen("/dev/null", "wb");

    inflateBackInit_(&src, 15, NULL, zlibVersion(), sizeof(z_stream));
    deflateInit_(&def, 6, zlibVersion(), sizeof(z_stream));

    inflateCopy(&dest, &src);
    deflateResetKeep(&def);

    unsigned char inbuf[1] = {0};
    inflateBack(&src, (in_func)my_in, inbuf, (out_func)my_out, NULL);

    inflateBackEnd(&src);
    inflateEnd(&dest);

    gzclearerr(gz);
    gzclose(gz);
    inflateEnd(&src);
    deflateEnd(&def);

    return 66;
}