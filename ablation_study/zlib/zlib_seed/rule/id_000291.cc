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
//<ID> 291
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    unsigned char in_buf[128];
    memset(in_buf, 'A', sizeof(in_buf));
    uInt in_len = (uInt)sizeof(in_buf);
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&strm, 15, window, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    Bytef dict[] = "example-dictionary";
    uInt dictLen = (uInt)(sizeof(dict) - 1);
    inflateSetDictionary(&strm, dict, dictLen);
    uLong checksum = adler32(0u, in_buf, in_len);
    (void)checksum;

    // step 3: Operate
    deflateResetKeep(&strm);
    struct InDesc { unsigned char *data; unsigned int len; unsigned int pos; };
    struct OutDesc { unsigned char *buf; unsigned int pos; unsigned int size; };
    InDesc in_desc = { in_buf, in_len, 0 };
    unsigned char out_buffer[256];
    memset(out_buffer, 0, sizeof(out_buffer));
    OutDesc out_desc = { out_buffer, 0, (unsigned int)sizeof(out_buffer) };
    in_func in_cb = [](void *d, unsigned char **p) -> unsigned int {
        InDesc *id = (InDesc *)d;
        unsigned int rem = id->len - id->pos;
        *p = id->data + id->pos;
        id->pos = id->len;
        return rem;
    };
    out_func out_cb = [](void *d, unsigned char *buf, unsigned int len) -> int {
        OutDesc *od = (OutDesc *)d;
        memcpy(od->buf + od->pos, buf, len);
        od->pos += len;
        return (int)len;
    };
    inflateBack(&strm, in_cb, &in_desc, out_cb, &out_desc);

    // step 4: Validate & Cleanup
    inflateBackEnd(&strm);
    free(window);
    return 66;
    // API sequence test completed successfully
}