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
//<ID> 831
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial write
    const char src[] = "zlib api sequence payload for gzwrite, gzoffset64, inflateBack and crc32";
    const unsigned int srcLen = (unsigned int)(sizeof(src) - 1);
    const Bytef *source = (const Bytef *)src;
    uLong crc_initial = crc32(0L, source, (uInt)srcLen);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gf, (voidpc)source, (unsigned int)srcLen);
    off64_t offset_after_write = gzoffset64(gf);

    // step 2: Prepare inflateBack environment
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc((size_t)32768);
    memset(window, 0, (size_t)32768);
    int rc_inflate_init = inflateBackInit_(&strm, 15, window, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Call inflateBack with simple callbacks (no capture lambdas converted to function pointers)
    in_func in_cb = (in_func)+[](void *in_desc, unsigned char **bufp) -> unsigned int {
        if (bufp) { *bufp = (unsigned char *)in_desc; }
        return 0u;
    };
    out_func out_cb = (out_func)+[](void *out_desc, unsigned char *buf, unsigned int len) -> int {
        (void)out_desc;
        (void)buf;
        (void)len;
        return 0;
    };
    unsigned char dummy_comp[1] = { 0 };
    void *in_desc = (void *)dummy_comp;
    void *out_desc = NULL;
    int rc_inflate_back = inflateBack(&strm, in_cb, in_desc, out_cb, out_desc);
    int rc_inflate_back_end = inflateBackEnd(&strm);

    // step 4: Cleanup and validation
    int rc_gz_close = gzclose(gf);
    uLong derived = crc_initial ^ (uLong)offset_after_write;
    (void)rc_gz_write;
    (void)offset_after_write;
    (void)rc_inflate_init;
    (void)rc_inflate_back;
    (void)rc_inflate_back_end;
    (void)rc_gz_close;
    (void)derived;
    free(window);
    // API sequence test completed successfully
    return 66;
}