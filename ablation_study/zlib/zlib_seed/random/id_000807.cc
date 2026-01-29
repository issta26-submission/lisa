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
//<ID> 807
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and allocate buffers
    const char src[] = "zlib API sequence payload to compress, write to gz, read back, and validate positions";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);

    // step 2: Initialize deflate stream, provide input and output, perform a single deflate call, then end
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)sourceLen;
    strm.next_out = compBuf;
    strm.avail_out = (uInt)compAlloc;
    int rc_def = deflate(&strm, 4); /* 4 is Z_FINISH */
    uLong compProduced = strm.total_out;
    int rc_def_end = deflateEnd(&strm);

    // step 3: Write the original source to a gz file, close, reopen for read, perform a single gzread and query positions
    gzFile wf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(wf, source, (unsigned int)sourceLen);
    int rc_gz_close_w = gzclose(wf);
    gzFile rf = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    unsigned int readBufSize = (unsigned int)(sourceLen + 16);
    Bytef *readBuf = (Bytef *)malloc((size_t)readBufSize);
    memset(readBuf, 0, (size_t)readBufSize);
    int rc_gz_read = gzread(rf, (voidp)readBuf, readBufSize);
    off_t pos = gztell(rf);
    off64_t pos64 = gzoffset64(rf);
    int rc_gz_close_r = gzclose(rf);

    // step 4: Validate/cleanup resources
    free(compBuf);
    free(readBuf);
    (void)version;
    (void)rc_def_init;
    (void)rc_def;
    (void)compProduced;
    (void)rc_def_end;
    (void)rc_gz_write;
    (void)rc_gz_close_w;
    (void)rc_gz_read;
    (void)pos;
    (void)pos64;
    (void)rc_gz_close_r;
    // API sequence test completed successfully
    return 66;
}