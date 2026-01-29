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
//<ID> 397
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    const char src[] = "zlib API sequence payload: gzdopen -> gzwrite -> gzread -> inflateInit_ -> inflateEnd -> zError";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    unsigned int write_len = (unsigned int)sourceLen;
    unsigned int read_buf_size = 256;
    Bytef *read_buf = (Bytef *)malloc((size_t)read_buf_size);
    memset(read_buf, 0, (size_t)read_buf_size);

    const char *tmpname = "test_zlib_tmp.gz";
    gzFile gz_out = gzopen(tmpname, "wb");
    int rc_gzwrite = gzwrite(gz_out, (voidpc)source, write_len);
    int rc_close_out = gzclose(gz_out);

    gzFile gz_in = gzopen(tmpname, "rb");
    int rc_gzread = gzread(gz_in, (voidp)read_buf, read_buf_size);
    int rc_close_in = gzclose(gz_in);

    const char *version = zlibVersion();
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    int rc_inflate_end = inflateEnd(&strm);

    const char *err_write = zError(rc_gzwrite);
    const char *err_read = zError(rc_gzread);

    free(read_buf);

    (void)sourceLen;
    (void)rc_gzwrite;
    (void)rc_gzread;
    (void)rc_inflate_init;
    (void)rc_inflate_end;
    (void)err_write;
    (void)err_read;
    (void)rc_close_out;
    (void)rc_close_in;
    return 66;
}