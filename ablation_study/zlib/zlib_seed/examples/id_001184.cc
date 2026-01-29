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
//<ID> 1184
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial buffers
    const char payload[] = "zlib API sequence payload demonstrating gzopen64/inflateBackInit_/deflateReset/zlibCompileFlags usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    uLong flags = zlibCompileFlags();
    uLong comp_bound = compressBound(payload_len);
    comp_bound = (comp_bound ? comp_bound : 1UL);
    Bytef *comp_buf = new Bytef[(size_t)comp_bound];
    memset(comp_buf, 0, (size_t)comp_bound);
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    unsigned char *window = new unsigned char[(1 << 15)];
    memset(window, 0, (size_t)(1 << 15));

    // step 2: Initialize and configure deflate stream, then reset
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)(payload_len ? payload_len : 0);
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)(comp_bound ? comp_bound : 0);
    int rc_deflate = deflate(&dstrm, 0);
    uLong comp_size = dstrm.total_out;
    int rc_deflate_reset = deflateReset(&dstrm);

    // step 3: Initialize inflate-back with a sliding window and write compressed output to gz file
    int rc_inflateback_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    gzFile gzf = gzopen64("zlib_seq_out.gz", "wb+");
    int rc_gzwrite = gzwrite(gzf, (voidpc)comp_buf, (unsigned int)(comp_size ? comp_size : 0));

    // step 4: Validate operations and cleanup resources
    int rc_gzclose_w = gzclose_w(gzf);
    int rc_inflateback_end = inflateBackEnd(&istrm);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_deflate_end = deflateEnd(&dstrm);
    delete [] comp_buf;
    delete [] window;
    (void)version;
    (void)flags;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)comp_size;
    (void)rc_deflate_reset;
    (void)rc_inflateback_init;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_inflateback_end;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}