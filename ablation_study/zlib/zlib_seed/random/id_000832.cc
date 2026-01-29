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
//<ID> 832
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char src[] = "zlib api payload for gzwrite, gzoffset64, inflateBack and crc32";
    const unsigned int srcLen = (unsigned int)(sizeof(src) - 1);
    const Bytef *source = (const Bytef *)src;
    uLong crc_before = crc32(0L, source, (uInt)srcLen);
    uInt outBufSize = (uInt)(srcLen + 128);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBufSize);
    memset(outBuf, 0, (size_t)outBufSize);
    Bytef *inBuf = (Bytef *)malloc((size_t)(srcLen));
    memset(inBuf, 0, (size_t)srcLen);
    memcpy(inBuf, source, (size_t)srcLen);
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));

    // step 2: Configure and operate: write data with gzwrite and inspect offset
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gf, source, (unsigned int)srcLen);
    off64_t offset_after_write = gzoffset64(gf);

    // step 3: Prepare z_stream and invoke inflateBack (callbacks provided as null pointers)
    strm.next_in = inBuf;
    strm.avail_in = (uInt)srcLen;
    strm.next_out = outBuf;
    strm.avail_out = outBufSize;
    int rc_inflate_back = inflateBack(&strm, (in_func)nullptr, (void *)nullptr, (out_func)nullptr, (void *)nullptr);

    // step 4: Validate-flow variables usage and cleanup
    int rc_close = gzclose(gf);
    (void)crc_before;
    (void)rc_gz_write;
    (void)offset_after_write;
    (void)rc_inflate_back;
    (void)rc_close;
    free(inBuf);
    free(outBuf);
    // API sequence test completed successfully
    return 66;
}