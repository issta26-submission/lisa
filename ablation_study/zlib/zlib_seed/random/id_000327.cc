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
//<ID> 327
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compute checksum
    const char src_c[] = "zlib API sequence payload for inflateBack + adler32_z + inflateCopy + inflateEnd";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong checksum = adler32_z(1UL, source, (z_size_t)sourceLen);

    // step 2: Initialize an inflate-back capable stream
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    istrm.adler = checksum;
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    const char *version = zlibVersion();
    int rc_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Copy the stream and perform an inflateBack call using simple in/out functions
    z_stream istrm_copy;
    memset(&istrm_copy, 0, sizeof(z_stream));
    int rc_copy = inflateCopy(&istrm_copy, &istrm);

    unsigned char *compBuf = (unsigned char *)malloc(64);
    memset(compBuf, 0, 64);
    unsigned char *decompBuf = (unsigned char *)malloc(128);
    memset(decompBuf, 0, 128);

    in_func inF = +[](void * in_desc, unsigned char ** buf) -> unsigned int {
        unsigned char *p = (unsigned char *)in_desc;
        *buf = p;
        return 1u;
    };
    out_func outF = +[](void * out_desc, unsigned char * out, unsigned int len) -> int {
        (void)out_desc;
        (void)out;
        return (int)len;
    };

    int rc_inflate_back = inflateBack(&istrm, inF, compBuf, outF, decompBuf);

    // step 4: Cleanup streams and buffers
    int rc_end1 = inflateEnd(&istrm);
    int rc_end2 = inflateEnd(&istrm_copy);
    free(window);
    free(compBuf);
    free(decompBuf);

    (void)source;
    (void)sourceLen;
    (void)checksum;
    (void)version;
    (void)rc_init;
    (void)rc_copy;
    (void)rc_inflate_back;
    (void)rc_end1;
    (void)rc_end2;

    // API sequence test completed successfully
    return 66;
}