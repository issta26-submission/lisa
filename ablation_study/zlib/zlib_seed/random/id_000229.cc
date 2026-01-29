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
//<ID> 229
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & compress the source payload
    const char src_c[] = "zlib API sequence payload: compress -> uncompress -> inflate -> gzrewind";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Uncompress using uncompress into a decompression buffer
    uLongf decompLen = (uLongf)(sourceLen + 64);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompLen);
    memset(decompBuf, 0, (size_t)decompLen);
    int rc_uncomp = uncompress(decompBuf, &decompLen, compBuf, (uLong)compLen);

    // step 3: Initialize an inflate stream and run inflate on the compressed data
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf infOutLen = (uLongf)(sourceLen + 128);
    Bytef *infOutBuf = (Bytef *)malloc((size_t)infOutLen);
    memset(infOutBuf, 0, (size_t)infOutLen);
    istrm.next_out = infOutBuf;
    istrm.avail_out = (uInt)infOutLen;
    int rc_inflate = inflate(&istrm, 0);
    uLong actual_inflate_out = istrm.total_out;

    // step 4: Write decompressed data to a gzip file, rewind, read back, and cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "w+b");
    int rc_gz_write = gzwrite(gz, decompBuf, (unsigned int)decompLen);
    int rc_gz_rewind = gzrewind(gz);
    Bytef *readBuf = (Bytef *)malloc((size_t)(actual_inflate_out + 8));
    memset(readBuf, 0, (size_t)(actual_inflate_out + 8));
    int rc_gz_read = gzread(gz, readBuf, (unsigned int)actual_inflate_out);
    int rc_gz_close = gzclose(gz);
    int rc_inf_end = inflateEnd(&istrm);

    free(compBuf);
    free(decompBuf);
    free(infOutBuf);
    free(readBuf);

    (void)rc_comp;
    (void)rc_uncomp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)actual_inflate_out;
    (void)rc_gz_write;
    (void)rc_gz_rewind;
    (void)rc_gz_read;
    (void)rc_gz_close;
    (void)rc_inf_end;
    (void)bound;
    (void)sourceLen;

    // API sequence test completed successfully
    return 66;
}