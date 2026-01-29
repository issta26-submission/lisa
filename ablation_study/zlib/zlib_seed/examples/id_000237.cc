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
//<ID> 237
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initial compression
    const char src_c[] = "zlib API sequence payload: test compressBound, deflateBound, gzwrite/gzread, inflate";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong comp_bound = compressBound(sourceLen);
    uLongf compLen = (uLongf)comp_bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)comp_bound);
    memset(compBuf, 0, (size_t)comp_bound);
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize deflate stream and compute deflateBound, also collect compile flags and CRC table
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong def_bound = deflateBound(&dstrm, sourceLen);
    const z_crc_t *crc_tbl = get_crc_table();
    uLong compile_flags = zlibCompileFlags();

    // step 3: Write compressed buffer into a gzip file, rewind and read back
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "w+b");
    int rc_gz_write = gzwrite(gz, (voidpc)compBuf, (unsigned int)compLen);
    int rc_gz_rewind = gzrewind(gz);
    Bytef *readBuf = (Bytef *)malloc((size_t)compLen);
    memset(readBuf, 0, (size_t)compLen);
    int rc_gz_read = gzread(gz, (voidp)readBuf, (unsigned int)compLen);
    int rc_gz_close = gzclose(gz);

    // step 4: Inflate the read buffer back to original and cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = readBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf decompLen = (uLongf)(sourceLen + 64);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompLen);
    memset(decompBuf, 0, (size_t)decompLen);
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompLen;
    int rc_inflate = inflate(&istrm, 0);
    uLong actual_out = istrm.total_out;
    int rc_inf_end = inflateEnd(&istrm);
    int rc_def_end = deflateEnd(&dstrm);

    free(compBuf);
    free(readBuf);
    free(decompBuf);

    (void)rc_comp;
    (void)rc_def_init;
    (void)def_bound;
    (void)crc_tbl;
    (void)compile_flags;
    (void)rc_gz_write;
    (void)rc_gz_rewind;
    (void)rc_gz_read;
    (void)rc_gz_close;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)actual_out;
    (void)rc_inf_end;
    (void)rc_def_end;
    (void)comp_bound;
    (void)sourceLen;

    // API sequence test completed successfully
    return 66;
}