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
//<ID> 1273
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "Zlib API sequence sample";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    z_stream defsrc;
    z_stream defdst;
    z_stream instrm;
    memset(&defsrc, 0, (size_t)sizeof(z_stream));
    memset(&defdst, 0, (size_t)sizeof(z_stream));
    memset(&instrm, 0, (size_t)sizeof(z_stream));
    uLong compile_flags = zlibCompileFlags();
    defsrc.adler = compile_flags;

    // step 2: Setup
    int rc_def_init = deflateInit_(&defsrc, 6, version, (int)sizeof(z_stream));
    Bytef *outbuf = new Bytef[(size_t)(srcLen + 32)];
    memset(outbuf, 0, (size_t)(srcLen + 32));
    defsrc.next_in = src;
    defsrc.avail_in = (uInt)srcLen;
    defsrc.next_out = outbuf;
    defsrc.avail_out = (uInt)(srcLen + 32);
    int rc_inflate_init = inflateInit_(&instrm, version, (int)sizeof(z_stream));
    instrm.next_in = src;
    instrm.avail_in = (uInt)srcLen;

    // step 3: Core operations
    int rc_def_params = deflateParams(&defsrc, 6, 0);
    int rc_def_copy = deflateCopy(&defdst, &defsrc);
    gzFile gzf = gzopen("zlib_seq_output.gz", "wb");
    off_t gzpos = gzseek(gzf, (off_t)0, 0);
    int rc_inflate_validate = inflateValidate(&instrm, 1);
    (void)gzpos;

    // step 4: Cleanup and finalization
    int rc_def_end_src = deflateEnd(&defsrc);
    int rc_def_end_dst = deflateEnd(&defdst);
    int rc_inflate_end = inflateEnd(&instrm);
    int rc_gzclose = gzclose(gzf);
    delete [] src;
    delete [] outbuf;
    (void)version;
    (void)compile_flags;
    (void)rc_def_init;
    (void)rc_inflate_init;
    (void)rc_def_params;
    (void)rc_def_copy;
    (void)rc_inflate_validate;
    (void)rc_def_end_src;
    (void)rc_def_end_dst;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}