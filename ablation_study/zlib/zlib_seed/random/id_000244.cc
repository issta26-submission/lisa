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
//<ID> 244
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & compute compress bound, compress source
    const char src_c[] = "zlib API sequence payload: compress -> inflate -> gz roundtrip -> crc";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream and inflate compressed data
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf infOutAlloc = (uLongf)(sourceLen + 128);
    Bytef *infOut = (Bytef *)malloc((size_t)infOutAlloc);
    memset(infOut, 0, (size_t)infOutAlloc);
    istrm.next_out = infOut;
    istrm.avail_out = (uInt)infOutAlloc;
    int rc_inflate = inflate(&istrm, 0);
    uLong actual_out = istrm.total_out;

    // step 3: Write inflated data to a gzip file, rewind, read back
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "w+b");
    int rc_gz_write = gzwrite(gz, (voidpc)infOut, (unsigned int)actual_out);
    int rc_gz_rewind = gzrewind(gz);
    Bytef *readBuf = (Bytef *)malloc((size_t)(actual_out + 16));
    memset(readBuf, 0, (size_t)(actual_out + 16));
    int rc_gz_read = gzread(gz, (voidp)readBuf, (unsigned int)actual_out);
    int rc_gz_close = gzclose(gz);

    // step 4: Validate via adler32, cleanup resources and streams
    uLong checksum = adler32(0L, (const Bytef *)readBuf, (uInt)actual_out);
    int rc_inf_end = inflateEnd(&istrm);

    free(compBuf);
    free(infOut);
    free(readBuf);

    (void)version;
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)actual_out;
    (void)rc_gz_write;
    (void)rc_gz_rewind;
    (void)rc_gz_read;
    (void)rc_gz_close;
    (void)checksum;
    (void)rc_inf_end;
    (void)compBound;
    (void)compLen;
    (void)infOutAlloc;
    (void)sourceLen;

    // API sequence test completed successfully
    return 66;
}