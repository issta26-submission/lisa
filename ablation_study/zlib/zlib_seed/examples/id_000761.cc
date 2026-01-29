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
//<ID> 761
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize — prepare source, allocate buffers and compress the data
    const char src[] = "zlib API sequence payload for compress, gzwrite, gzseek, gzclose and inflateValidate";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf destLen = (uLongf)compAlloc;
    int rc_compress = compress(compBuf, &destLen, source, sourceLen);
    uLong compProduced = (uLong)destLen;

    // step 2: Configure — open a gz file for writing
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");

    // step 3: Operate — write the compressed data into the gz file and seek
    int rc_gz_write = gzwrite(gf, compBuf, (unsigned int)compProduced);
    off_t pos_after_write = gzseek(gf, 0, 0);
    int rc_gz_close = gzclose(gf);

    // step 4: Validate and Cleanup — prepare a z_stream for validation and free resources
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compProduced;
    istrm.total_in = 0;
    int rc_inflate_validate = inflateValidate(&istrm, 0);

    free(compBuf);

    (void)rc_compress;
    (void)compProduced;
    (void)rc_gz_write;
    (void)pos_after_write;
    (void)rc_gz_close;
    (void)rc_inflate_validate;
    // API sequence test completed successfully
    return 66;
}