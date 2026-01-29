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
//<ID> 1271
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence test payload";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    z_stream defstrm;
    z_stream defcopy;
    z_stream instrm;
    memset(&defstrm, 0, (size_t)sizeof(z_stream));
    memset(&defcopy, 0, (size_t)sizeof(z_stream));
    memset(&instrm, 0, (size_t)sizeof(z_stream));
    Bytef *bufOut = new Bytef[(size_t)(srcLen + 64)];
    memset(bufOut, 0, (size_t)(srcLen + 64));

    // step 2: Setup (initialize deflate stream and prepare copy)
    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    defstrm.next_in = src;
    defstrm.avail_in = (uInt)srcLen;
    defstrm.next_out = bufOut;
    defstrm.avail_out = (uInt)(srcLen + 64);
    int rc_deflate_copy = deflateCopy(&defcopy, &defstrm);

    // step 3: Core operations (compile flags, gzip I/O and inflate validation)
    uLong compile_flags = zlibCompileFlags();
    gzFile gzf = gzopen("zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (const void *)src, (unsigned int)srcLen);
    off_t rc_gzseek = gzseek(gzf, (off_t)0, 0);
    int rc_inflate_init = inflateInit_(&instrm, version, (int)sizeof(z_stream));
    instrm.next_in = bufOut;
    instrm.avail_in = (uInt)(srcLen + 64);
    int rc_inflate_validate = inflateValidate(&instrm, 1);

    // step 4: Cleanup and finalization
    int rc_def_end_src = deflateEnd(&defstrm);
    int rc_def_end_copy = deflateEnd(&defcopy);
    int rc_inflate_end = inflateEnd(&instrm);
    int rc_gzclose = gzclose(gzf);
    delete [] src;
    delete [] bufOut;
    (void)version;
    (void)rc_def_init;
    (void)rc_deflate_copy;
    (void)compile_flags;
    (void)rc_gzwrite;
    (void)rc_gzseek;
    (void)rc_inflate_init;
    (void)rc_inflate_validate;
    (void)rc_def_end_src;
    (void)rc_def_end_copy;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}