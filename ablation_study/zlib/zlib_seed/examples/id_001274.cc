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
    // step 1: Declarations
    const char payload[] = "Example payload for zlib API sequence";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    uLong bound = compressBound(srcLen);
    Bytef *outBuf = new Bytef[(size_t)bound];
    memset(outBuf, 0, (size_t)bound);
    z_stream defstrm;
    z_stream defcopystrm;
    z_stream instrm;
    memset(&defstrm, 0, (size_t)sizeof(z_stream));
    memset(&defcopystrm, 0, (size_t)sizeof(z_stream));
    memset(&instrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (initialize deflate and inflate streams and prepare buffers)
    int rc_deflate_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    defstrm.next_in = src;
    defstrm.avail_in = (uInt)srcLen;
    defstrm.next_out = outBuf;
    defstrm.avail_out = (uInt)bound;

    // step 3: Core operations (compress, copy deflate state, write gzip and validate inflate)
    int rc_deflate = deflate(&defstrm, Z_FINISH);
    int rc_deflate_copy = deflateCopy(&defcopystrm, &defstrm);
    gzFile gzf = gzopen("zlib_api_seq_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, outBuf, (unsigned int)defstrm.total_out);
    off_t curpos = gzseek(gzf, 0, SEEK_CUR);
    int rc_inflate_init = inflateInit_(&instrm, version, (int)sizeof(z_stream));
    int rc_inflate_validate = inflateValidate(&instrm, 1);
    uLong compile_flags = zlibCompileFlags();

    // step 4: Cleanup and finalization
    int rc_deflate_end = deflateEnd(&defstrm);
    int rc_deflate_copy_end = deflateEnd(&defcopystrm);
    int rc_inflate_end = inflateEnd(&instrm);
    int rc_gzclose = gzclose(gzf);
    delete [] src;
    delete [] outBuf;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_deflate_copy;
    (void)rc_gzwrite;
    (void)curpos;
    (void)rc_inflate_init;
    (void)rc_inflate_validate;
    (void)compile_flags;
    (void)rc_deflate_end;
    (void)rc_deflate_copy_end;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}