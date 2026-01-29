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
//<ID> 1429
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for deflate/init/copy/getdict/gzwrite";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup - initialize deflate stream and prepare buffers
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong defBound = deflateBound(&dstrm, srcLen);
    Bytef *defBuf = new Bytef[(size_t)defBound];
    memset(defBuf, 0, (size_t)defBound);
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = defBuf;
    dstrm.avail_out = (uInt)defBound;

    // step 3: Core operations - perform deflate, copy deflate state, get dictionary, write original payload to gzip
    int rc_deflate = deflate(&dstrm, 0);
    uLong compLen = dstrm.total_out;
    z_stream cstrm;
    memset(&cstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_copy = deflateCopy(&cstrm, &dstrm);
    uInt dictBufSize = 64;
    Bytef *dictBuf = new Bytef[(size_t)dictBufSize];
    memset(dictBuf, 0, (size_t)dictBufSize);
    uInt dictLen = dictBufSize;
    int rc_deflate_get_dict = deflateGetDictionary(&cstrm, dictBuf, &dictLen);
    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)src, (unsigned int)srcLen);
    int rc_gzclose_w = gzclose_w(gzf);
    int rc_deflate_end_src = deflateEnd(&dstrm);
    int rc_deflate_end_copy = deflateEnd(&cstrm);

    // step 4: Cleanup
    delete [] src;
    delete [] defBuf;
    delete [] dictBuf;
    (void)version;
    (void)rc_deflate_init;
    (void)defBound;
    (void)rc_deflate;
    (void)compLen;
    (void)rc_deflate_copy;
    (void)dictLen;
    (void)rc_deflate_get_dict;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_deflate_end_src;
    (void)rc_deflate_end_copy;
    // API sequence test completed successfully
    return 66;
}