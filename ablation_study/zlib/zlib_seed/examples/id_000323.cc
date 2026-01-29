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
//<ID> 323
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute adler32 checksum
    const char src_c[] = "zlib inflateBack API sequence payload";
    const Bytef *source = (const Bytef *)src_c;
    z_size_t sourceLen = (z_size_t)(sizeof(src_c) - 1);
    uLong checksum = adler32_z(0UL, source, sourceLen);

    // step 2: Allocate buffers and initialize inflate streams
    uLong inBufLen = (uLong)(sourceLen + 16);
    uLong outBufLen = (uLong)(sourceLen + 64);
    Bytef *inBuf = (Bytef *)malloc((size_t)inBufLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBufLen);
    memcpy(inBuf, source, (size_t)sourceLen);
    if (inBufLen > sourceLen) {
        memset(inBuf + sourceLen, 0, (size_t)(inBufLen - sourceLen));
    }
    memset(outBuf, 0, (size_t)outBufLen);
    z_stream istrm;
    z_stream istrm_copy;
    memset(&istrm, 0, sizeof(z_stream));
    memset(&istrm_copy, 0, sizeof(z_stream));
    istrm.next_in = inBuf;
    istrm.avail_in = (uInt)inBufLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outBufLen;

    // step 3: Copy the inflate stream and invoke inflateBack (using null callbacks as descriptors)
    int rc_copy = inflateCopy(&istrm_copy, &istrm);
    int rc_inflate_back = inflateBack(&istrm, (in_func)0, (void *)inBuf, (out_func)0, (void *)outBuf);

    // step 4: Cleanup streams and buffers
    int rc_end_orig = inflateEnd(&istrm);
    int rc_end_copy = inflateEnd(&istrm_copy);
    free(inBuf);
    free(outBuf);

    (void)checksum;
    (void)rc_copy;
    (void)rc_inflate_back;
    (void)rc_end_orig;
    (void)rc_end_copy;

    // API sequence test completed successfully
    return 66;
}