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
//<ID> 210
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize source and version
    const char src_c[] = "zlib API sequence payload: compress2 -> uncompress -> gzwrite -> deflateInit_/inflateInit_/deflateEnd/inflateEnd cleanup";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();

    // step 2: Compress the source with compress2 and prepare streams
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp2 = compress2(compBuf, &compLen, source, sourceLen, 6);

    z_stream defstrm;
    memset(&defstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));

    z_stream instrm;
    memset(&instrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&instrm, version, (int)sizeof(z_stream));

    // step 3: Decompress into buffer and write to gzip file
    uLongf decompLen = (uLongf)(sourceLen + 64);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompLen);
    memset(decompBuf, 0, (size_t)decompLen);
    int rc_uncomp = uncompress(decompBuf, &decompLen, compBuf, (uLong)compLen);

    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, (voidpc)decompBuf, (unsigned int)decompLen);
    int rc_gz_close = gzclose(gz);

    // step 4: Cleanup streams and buffers
    int rc_def_end = deflateEnd(&defstrm);
    int rc_inf_end = inflateEnd(&instrm);
    free(compBuf);
    free(decompBuf);

    (void)rc_comp2;
    (void)rc_def_init;
    (void)rc_inf_init;
    (void)rc_uncomp;
    (void)rc_gz_write;
    (void)rc_gz_close;
    (void)rc_def_end;
    (void)rc_inf_end;
    (void)version;
    (void)sourceLen;
    (void)compLen;
    // API sequence test completed successfully
    return 66;
}