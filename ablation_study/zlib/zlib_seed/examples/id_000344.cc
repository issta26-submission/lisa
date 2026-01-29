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
//<ID> 344
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, buffers and obtain zlib version
    const char src[] = "zlib API sequence payload: write with gzwrite -> read with gzread -> init/end inflate";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong readBufAlloc = (uLong)(sourceLen + 128);
    Bytef *readBuf = (Bytef *)malloc((size_t)readBufAlloc);
    memset(readBuf, 0, (size_t)readBufAlloc);

    // step 2: Open a gzip file for writing and write the source into it
    gzFile wf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_write = gzwrite(wf, (voidpc)source, (unsigned int)sourceLen);
    int rc_close_w = gzclose(wf);

    // step 3: Re-open the gzip file for reading and read back the contents
    gzFile rf = gzopen("test_zlib_api_sequence.gz", "rb");
    int rc_read = gzread(rf, (voidp)readBuf, (unsigned int)readBufAlloc);
    int rc_close_r = gzclose(rf);

    // step 4: Initialize an inflate stream with the zlib version, associate the read buffer, then cleanup
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = readBuf;
    strm.avail_in = (uInt)rc_read;
    int rc_inf_end = inflateEnd(&strm);

    free(readBuf);

    (void)sourceLen;
    (void)rc_write;
    (void)rc_close_w;
    (void)rc_read;
    (void)rc_close_r;
    (void)rc_inf_init;
    (void)rc_inf_end;
    (void)version;

    // API sequence test completed successfully
    return 66;
}