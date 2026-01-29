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
//<ID> 1278
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence example payload";
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

    // step 2: Setup (initialize deflate and prepare buffers)
    int rc_deflate_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    defstrm.next_in = src;
    defstrm.avail_in = (uInt)srcLen;
    Bytef *defOut = new Bytef[(size_t)(srcLen + 64UL)];
    memset(defOut, 0, (size_t)(srcLen + 64UL));
    defstrm.next_out = defOut;
    defstrm.avail_out = (uInt)(srcLen + 64UL);

    // step 3: Core operations (copy deflate state, initialize inflate, gz I/O, validation and flags)
    int rc_deflate_copy = deflateCopy(&defcopy, &defstrm);
    defcopy.next_out = defOut;
    defcopy.avail_out = (uInt)(srcLen + 64UL);
    int rc_inflate_init = inflateInit_(&instrm, version, (int)sizeof(z_stream));
    instrm.next_in = defOut;
    instrm.avail_in = 0;
    int rc_inflate_validate = inflateValidate(&instrm, 1);
    gzFile gzf = gzopen("test_zlib_seq.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidp)src, (unsigned int)srcLen);
    off_t rc_gzseek = gzseek(gzf, (off_t)0, 0);
    uLong compile_flags = zlibCompileFlags();

    // step 4: Cleanup and finalization
    int rc_gzclose = gzclose(gzf);
    int rc_deflate_end_copy = deflateEnd(&defcopy);
    int rc_deflate_end = deflateEnd(&defstrm);
    int rc_inflate_end = inflateEnd(&instrm);
    delete [] src;
    delete [] defOut;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_deflate_copy;
    (void)rc_inflate_init;
    (void)rc_inflate_validate;
    (void)rc_gzwrite;
    (void)rc_gzseek;
    (void)compile_flags;
    (void)rc_gzclose;
    (void)rc_deflate_end_copy;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}