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
//<ID> 804
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and allocate buffers using compressBound
    const char src[] = "zlib API sequence payload: prepare, deflate, write/read gz, and query offsets";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    const char *version = zlibVersion();

    // step 2: Initialize deflate stream, supply input and output buffers, and call deflate
    z_stream strm;
    memset(&strm, 0, (int)sizeof(z_stream));
    int rc_def_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)sourceLen;
    strm.next_out = compBuf;
    strm.avail_out = (uInt)compAlloc;
    int rc_def = deflate(&strm, 0);
    uLong compressedProduced = strm.total_out;

    // step 3: Persist original source to a gz file, then reopen and read it back; query positions with gztell and gzoffset64
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gw, source, (unsigned int)sourceLen);
    int rc_gz_close_w = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)(sourceLen + 32));
    memset(readBuf, 0, (size_t)(sourceLen + 32));
    int bytesRead = gzread(gr, readBuf, (unsigned int)(sourceLen + 32));
    off_t pos = gztell(gr);
    off64_t pos64 = gzoffset64(gr);
    int rc_gz_close_r = gzclose(gr);

    // step 4: Cleanup deflate stream and allocated memory, mark used results to avoid warnings
    int rc_def_end = deflateEnd(&strm);
    free(compBuf);
    free(readBuf);
    (void)version;
    (void)rc_def_init;
    (void)rc_def;
    (void)compressedProduced;
    (void)rc_gz_write;
    (void)rc_gz_close_w;
    (void)bytesRead;
    (void)pos;
    (void)pos64;
    (void)rc_gz_close_r;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}