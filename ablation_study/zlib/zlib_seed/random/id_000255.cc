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
//<ID> 255
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress into a buffer
    const char src_c[] = "zlib API sequence payload: inflateBackInit_ + gzseek64 + gzgetc_ + crc32_z";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream and an inflateBack stream (with its window)
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    z_stream backStrm;
    memset(&backStrm, 0, sizeof(z_stream));
    unsigned char *windowBuf = (unsigned char *)malloc(32768);
    memset(windowBuf, 0, 32768);
    int rc_back_init = inflateBackInit_(&backStrm, 15, windowBuf, version, (int)sizeof(z_stream));

    // step 3: Inflate compressed data, compute CRC, write and seek in a gz file, then read one char
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf decompAlloc = (uLongf)(sourceLen + 256);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    int rc_inflate = inflate(&istrm, 0);
    uLong actual_out = istrm.total_out;
    uLong crc = crc32_z(0UL, (const Bytef *)decompBuf, (z_size_t)actual_out);

    gzFile gz = gzopen("test_zlib_api_sequence.gz", "w+b");
    int rc_gz_write = gzwrite(gz, (voidpc)decompBuf, (unsigned int)actual_out);
    off64_t rc_seek = gzseek64(gz, (off64_t)0, 0);
    int ch = gzgetc_(gz);
    int rc_gz_close = gzclose(gz);

    // step 4: Cleanup streams and memory
    int rc_inf_end = inflateEnd(&istrm);
    int rc_back_end = inflateBackEnd(&backStrm);
    free(compBuf);
    free(decompBuf);
    free(windowBuf);

    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_inf_init;
    (void)rc_back_init;
    (void)rc_inflate;
    (void)actual_out;
    (void)crc;
    (void)rc_gz_write;
    (void)rc_seek;
    (void)ch;
    (void)rc_gz_close;
    (void)rc_inf_end;
    (void)rc_back_end;
    (void)sourceLen;
    (void)version;

    // API sequence test completed successfully
    return 66;
}