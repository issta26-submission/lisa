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
//<ID> 1185
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for gzopen64/inflateBackInit_/deflateReset/zlibCompileFlags usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    uLong compile_flags = zlibCompileFlags();
    z_stream istrm;
    z_stream dstrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    unsigned char *window = new unsigned char[(1 << 15)];
    memset(window, 0, (size_t)(1 << 15));
    Bytef *outbuf = new Bytef[(size_t)(payload_len + 128)];
    memset(outbuf, 0, (size_t)(payload_len + 128));

    // step 2: Setup (initialize deflate stream and inflate-back)
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_deflate_reset = deflateReset(&dstrm);
    int rc_inflateback_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Core operations (write compressed payload to gz file and prepare deflate buffers)
    gzFile gzf = gzopen64("zlib_seq_out.gz", "wb+");
    int rc_gzwrite = gzwrite(gzf, (voidpc)payload, (unsigned int)(payload_len ? payload_len : 0));
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)(payload_len ? payload_len : 0);
    dstrm.next_out = outbuf;
    dstrm.avail_out = (uInt)(payload_len + 128);
    int rc_deflate_call = deflate(&dstrm, 0);
    int rc_gzflush = gzflush(gzf, 2);
    int rc_gzclose_w = gzclose_w(gzf);

    // step 4: Validate and cleanup
    int rc_inflateback_end = inflateBackEnd(&istrm);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_deflate_end = deflateEnd(&dstrm);
    delete [] window;
    delete [] outbuf;
    (void)version;
    (void)compile_flags;
    (void)rc_deflate_init;
    (void)rc_deflate_reset;
    (void)rc_inflateback_init;
    (void)rc_gzwrite;
    (void)rc_deflate_call;
    (void)rc_gzflush;
    (void)rc_gzclose_w;
    (void)rc_inflateback_end;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}