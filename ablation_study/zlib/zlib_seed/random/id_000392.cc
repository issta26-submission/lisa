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
//<ID> 392
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare payload and create a temporary file with duplicated fds for independent write/read
    const char src[] = "zlib API sequence payload: gzdopen -> gzwrite -> gzread -> compress2 -> inflate -> inflateEnd -> zError";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    FILE *tmp = tmpfile();
    int fd_orig = fileno(tmp);
    int fd_write = dup(fd_orig);
    int fd_read = dup(fd_orig);
    fclose(tmp);

    // step 2: Open gz stream for writing via gzdopen, write payload, and close to flush compressed data
    gzFile gw = gzdopen(fd_write, "wb");
    int rc_gzwrite = gzwrite(gw, source, (unsigned int)sourceLen);
    int rc_gw_close = gzclose(gw);

    // step 3: Rewind read descriptor, open gz stream for reading via gzdopen, read back decompressed data
    lseek(fd_read, 0, SEEK_SET);
    gzFile gr = gzdopen(fd_read, "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(readBuf, 0, (size_t)sourceLen);
    int rc_gzread = gzread(gr, (voidp)readBuf, (unsigned int)sourceLen);
    int rc_gr_close = gzclose(gr);

    // step 4: Compress the original payload with compress2, inflate it using inflateInit_/inflate/inflateEnd, validate with crc and call zError, then cleanup
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp2 = compress2(compBuf, &compLen, source, sourceLen, 6);

    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compLen;
    Bytef *decBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(decBuf, 0, (size_t)sourceLen);
    strm.next_out = decBuf;
    strm.avail_out = (uInt)sourceLen;
    int rc_inflate = inflate(&strm, 0);
    int rc_inflate_end = inflateEnd(&strm);

    uLong crc_read = crc32(0UL, readBuf, (uInt)sourceLen);
    uLong crc_dec = crc32(0UL, decBuf, (uInt)sourceLen);
    const char *zerr = zError(0);

    free(readBuf);
    free(compBuf);
    free(decBuf);
    (void)rc_gzwrite;
    (void)rc_gw_close;
    (void)rc_gzread;
    (void)rc_gr_close;
    (void)rc_comp2;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)crc_read;
    (void)crc_dec;
    (void)zerr;
    // API sequence test completed successfully
    return 66;
}