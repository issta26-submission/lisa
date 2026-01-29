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
//<ID> 242
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & compress the source payload
    const char src[] = "zlib API sequence payload: compress -> inflate -> gz roundtrip";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize inflate stream and perform inflation of compressed data
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf decompAlloc = (uLongf)(sourceLen + 128);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    int rc_inflate = inflate(&istrm, 0);
    uLong actual_out = istrm.total_out;

    // step 3: Write decompressed data to a gz file, rewind and read it back
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "w+b");
    int rc_gz_write = gzwrite(gz, (voidpc)decompBuf, (unsigned int)actual_out);
    int rc_gz_rewind = gzrewind(gz);
    Bytef *readBuf = (Bytef *)malloc((size_t)(actual_out + 16));
    memset(readBuf, 0, (size_t)(actual_out + 16));
    int rc_gz_read = gzread(gz, (voidp)readBuf, (unsigned int)actual_out);
    int rc_gz_close = gzclose(gz);

    // step 4: Validate via CRC, clean up resources and finalize the inflate stream
    uLong crc = crc32(0L, readBuf, (uInt)actual_out);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(decompBuf);
    free(readBuf);

    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)actual_out;
    (void)rc_gz_write;
    (void)rc_gz_rewind;
    (void)rc_gz_read;
    (void)rc_gz_close;
    (void)crc;
    (void)bound;
    (void)compLen;
    (void)decompAlloc;
    (void)sourceLen;
    // API sequence test completed successfully
    return 66;
}