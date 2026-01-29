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
//<ID> 760
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char src[] = "zlib API sequence payload for compress, gzwrite, gzseek and inflateValidate";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compAlloc = compressBound(sourceLen);
    uLongf destLen = (uLongf)compAlloc;
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);

    // step 2: Configure and compress the source buffer
    int rc_compress = compress(compBuf, &destLen, source, sourceLen);
    uLong compProduced = (uLong)destLen;

    // step 3: Operate on compressed data with gz APIs
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gf, compBuf, (unsigned int)compProduced);
    off_t seek_pos = gzseek(gf, (off_t)0, 0);

    // step 4: Validate inflate state and cleanup
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compProduced;
    int rc_inflate_validate = inflateValidate(&istrm, 0);
    int rc_gz_close = gzclose(gf);
    free(compBuf);

    (void)rc_compress;
    (void)compProduced;
    (void)rc_gz_write;
    (void)seek_pos;
    (void)rc_inflate_validate;
    (void)rc_gz_close;
    // API sequence test completed successfully
    return 66;
}