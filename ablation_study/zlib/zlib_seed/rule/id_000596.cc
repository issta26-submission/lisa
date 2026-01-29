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
//<ID> 596
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

static unsigned char dummy_in_buf[1];

static unsigned int dummy_in(void *in_desc, unsigned char **buf) {
    (void)in_desc;
    *buf = dummy_in_buf;
    return 0;
}

static int dummy_out(void *out_desc, unsigned char *buf, unsigned int len) {
    (void)out_desc;
    (void)buf;
    (void)len;
    return 0;
}

int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream origStrm;
    memset(&origStrm, 0, sizeof(origStrm));
    z_stream copyStrm;
    memset(&copyStrm, 0, sizeof(copyStrm));
    z_stream backStrm;
    memset(&backStrm, 0, sizeof(backStrm));
    const char * ver = zlibVersion();
    uLong flags = zlibCompileFlags();
    (void)flags;
    Bytef * input = (Bytef *)malloc((size_t)64);
    memset(input, 'A', 64);
    uLong inputLen = (uLong)64;
    Bytef * outBuf = (Bytef *)malloc((size_t)128);
    memset(outBuf, 0, 128);

    // step 2: Initialize streams and back buffer
    inflateInit_(&origStrm, ver, (int)sizeof(z_stream));
    origStrm.next_in = input;
    origStrm.avail_in = (uInt)inputLen;
    origStrm.next_out = outBuf;
    origStrm.avail_out = (uInt)128;
    unsigned char * window = (unsigned char *)malloc(1);
    memset(window, 0, 1);
    inflateBackInit_(&backStrm, 15, window, ver, (int)sizeof(z_stream));

    // step 3: Operate: copy stream, perform back inflate call (with safe dummy callbacks), write via gzputc
    inflateCopy(&copyStrm, &origStrm);
    inflateBack(&backStrm, (in_func)dummy_in, (void *)0, (out_func)dummy_out, (void *)0);
    gzFile gz = gzopen("tmp_test_zlib_api.gz", "wb");
    gzputc(gz, 'Z');
    gzclose(gz);

    // step 4: Cleanup
    inflateBackEnd(&backStrm);
    inflateEnd(&origStrm);
    inflateEnd(&copyStrm);
    free(input);
    free(outBuf);
    free(window);
    // API sequence test completed successfully
    return 66;
}