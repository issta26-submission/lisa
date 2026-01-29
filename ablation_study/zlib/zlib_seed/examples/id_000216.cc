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
//<ID> 216
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src[] = "zlib API sequence payload: deflate -> inflate -> gzwrite -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();

    // step 2: Initialize deflate stream and compress into a buffer
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)bound;
    int rc_deflate = deflate(&dstrm, 4); // Z_FINISH (numeric)
    uLong compLen = dstrm.total_out;
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Initialize inflate stream and decompress the data
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLong decompBufSize = sourceLen + 64;
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompBufSize);
    memset(decompBuf, 0, (size_t)decompBufSize);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompBufSize;
    int rc_inflate = inflate(&istrm, 4); // Z_FINISH (numeric)
    uLong decompLen = istrm.total_out;
    int rc_inf_end = inflateEnd(&istrm);

    // step 4: Write decompressed data to a gzip file and cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, (voidpc)decompBuf, (unsigned int)decompLen);
    int rc_gz_close = gzclose(gz);
    free(compBuf);
    free(decompBuf);
    (void)rc_def_init;
    (void)rc_deflate;
    (void)rc_def_end;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)rc_gz_write;
    (void)rc_gz_close;
    (void)compLen;
    (void)decompLen;
    (void)version;

    // API sequence test completed successfully
    return 66;
}