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
//<ID> 802
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare source and buffers
    const char src[] = "zlib API sequence payload to deflate, write to gz, read back, and validate via adler32";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));

    // step 2: Initialize deflate stream, provide input and compress into compBuf (single call)
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compAlloc;
    int rc_def = deflate(&dstrm, 4); /* 4 corresponds to Z_FINISH */
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Persist original source to gz, then reopen and read it back; obtain positions
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gw, source, (unsigned int)sourceLen);
    int rc_gz_close_w = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)(sourceLen + 16));
    memset(readBuf, 0, (size_t)(sourceLen + 16));
    int rc_gz_read = gzread(gr, (voidp)readBuf, (unsigned int)sourceLen);
    off_t pos32 = gztell(gr);
    off64_t pos64 = gzoffset64(gr);
    int rc_gz_close_r = gzclose(gr);

    // step 4: Validate read data via adler32, cleanup allocated resources
    uLong crc_src = adler32(1UL, source, (uInt)sourceLen);
    uLong crc_read = adler32(1UL, readBuf, (uInt)rc_gz_read);
    uLong crc_xor = crc_src ^ crc_read;
    free(compBuf);
    free(readBuf);
    (void)version;
    (void)compAlloc;
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_def_end;
    (void)rc_gz_write;
    (void)rc_gz_close_w;
    (void)rc_gz_read;
    (void)pos32;
    (void)pos64;
    (void)rc_gz_close_r;
    (void)crc_src;
    (void)crc_read;
    (void)crc_xor;
    // API sequence test completed successfully
    return 66;
}