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
//<ID> 232
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & compute bounds
    const char src_c[] = "zlib API sequence payload: compute bounds -> compress -> inflate -> gzip IO";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong cbound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)cbound);
    memset(compBuf, 0, (size_t)cbound);
    uLongf compLen = (uLongf)cbound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize deflate stream and query deflateBound and compile flags / CRC table
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong dbound = deflateBound(&dstrm, sourceLen);
    uLong flags = zlibCompileFlags();
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_crc = crc_tbl ? crc_tbl[0] : (z_crc_t)0;

    // step 3: Inflate compressed data, write to gz file, rewind and read back
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf decompLen = (uLongf)(sourceLen + 64);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompLen);
    memset(decompBuf, 0, (size_t)decompLen);
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompLen;
    int rc_inflate = inflate(&istrm, 0);
    uLong actual_out = istrm.total_out;
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "w+b");
    int rc_gz_write = gzwrite(gz, (voidpc)decompBuf, (unsigned int)actual_out);
    int rc_gz_rewind = gzrewind(gz);
    Bytef *readBuf = (Bytef *)malloc((size_t)(actual_out + 8));
    memset(readBuf, 0, (size_t)(actual_out + 8));
    int rc_gz_read = gzread(gz, readBuf, (unsigned int)actual_out);
    int rc_gz_close = gzclose(gz);

    // step 4: Cleanup deflate & inflate streams and free buffers
    int rc_inf_end = inflateEnd(&istrm);
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(decompBuf);
    free(readBuf);

    (void)rc_comp;
    (void)rc_def_init;
    (void)dbound;
    (void)flags;
    (void)first_crc;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)actual_out;
    (void)rc_gz_write;
    (void)rc_gz_rewind;
    (void)rc_gz_read;
    (void)rc_gz_close;
    (void)rc_inf_end;
    (void)rc_def_end;
    (void)cbound;
    (void)sourceLen;

    // API sequence test completed successfully
    return 66;
}