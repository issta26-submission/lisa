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
//<ID> 762
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and allocate buffers
    const char src[] = "zlib API sequence payload for compress, gzwrite, gzseek and inflateValidate";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf destLen = (uLongf)compAlloc;

    // step 2: Compress the source into compBuf
    int rc_compress = compress(compBuf, &destLen, source, sourceLen);

    // step 3: Persist original data to a gz file, write and seek within it
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gf, source, (unsigned int)sourceLen);
    off_t rc_gz_seek = gzseek(gf, (off_t)0, 0);
    int rc_gz_close = gzclose(gf);

    // step 4: Prepare a z_stream for validation of the compressed data and validate
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)destLen;
    int rc_inflate_validate = inflateValidate(&istrm, 1);

    free(compBuf);
    (void)rc_compress;
    (void)rc_gz_write;
    (void)rc_gz_seek;
    (void)rc_gz_close;
    (void)rc_inflate_validate;
    // API sequence test completed successfully
    return 66;
}