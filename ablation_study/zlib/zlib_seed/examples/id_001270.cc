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
//<ID> 1270
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "Zlib fuzz payload for API sequence";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    uLong flags = zlibCompileFlags();
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen_f = (uLongf)bound;
    int rc_comp = compress2(compBuf, &compLen_f, src, srcLen, 6);
    uLong compLen = (uLong)compLen_f;

    // step 2: Setup (initialize streams and copy deflate state)
    z_stream defstrm;
    z_stream defcopy;
    z_stream instrm;
    memset(&defstrm, 0, (size_t)sizeof(z_stream));
    memset(&defcopy, 0, (size_t)sizeof(z_stream));
    memset(&instrm, 0, (size_t)sizeof(z_stream));
    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    defstrm.next_in = compBuf;
    defstrm.avail_in = (uInt)compLen;
    Bytef *defOut = new Bytef[(size_t)(compLen + 16UL)];
    memset(defOut, 0, (size_t)(compLen + 16UL));
    defstrm.next_out = defOut;
    defstrm.avail_out = (uInt)(compLen + 16UL);
    defstrm.data_type = (int)(flags & 0xFF);
    int rc_def_copy = deflateCopy(&defcopy, &defstrm);

    // step 3: Core operations (inflate validation and gzip I/O with seek)
    int rc_inflate_init = inflateInit_(&instrm, version, (int)sizeof(z_stream));
    instrm.next_in = compBuf;
    instrm.avail_in = (uInt)compLen;
    int rc_inflate_valid = inflateValidate(&instrm, 0);
    gzFile gzf = gzopen("zlib_seq_gzout.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (const void *)compBuf, (unsigned int)compLen);
    off_t seek_pos = gzseek(gzf, 0, 0);
    int rc_gzclose = gzclose(gzf);

    // step 4: Cleanup and finalization
    int rc_def_end1 = deflateEnd(&defstrm);
    int rc_def_end2 = deflateEnd(&defcopy);
    int rc_inflate_end = inflateEnd(&instrm);
    delete [] src;
    delete [] compBuf;
    delete [] defOut;
    (void)version;
    (void)rc_comp;
    (void)bound;
    (void)compLen_f;
    (void)rc_def_init;
    (void)rc_def_copy;
    (void)rc_inflate_init;
    (void)rc_inflate_valid;
    (void)rc_gzwrite;
    (void)seek_pos;
    (void)rc_gzclose;
    (void)rc_def_end1;
    (void)rc_def_end2;
    (void)rc_inflate_end;
    (void)flags;
    // API sequence test completed successfully
    return 66;
}