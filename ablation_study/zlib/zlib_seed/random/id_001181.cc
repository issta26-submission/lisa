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
//<ID> 1181
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API sequence payload for inflateBackInit_/deflateReset/gzopen64/zlibCompileFlags usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    uLong comp_buf_size = compressBound(payload_len);
    comp_buf_size = (comp_buf_size ? comp_buf_size : 1);
    Bytef *comp_buf = new Bytef[(size_t)comp_buf_size];
    memset(comp_buf, 0, (size_t)comp_buf_size);
    unsigned char *window = new unsigned char[(1 << 15)];
    memset(window, 0, (size_t)(1 << 15));
    z_stream istrm;
    z_stream dstrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    memset(&dstrm, 0, (size_t)sizeof(z_stream));

    // step 2: Initialize inflate-back stream and deflate stream
    int rc_inflateback_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 15, 8, 0, version, (int)sizeof(z_stream));

    // step 3: Compress payload, reset deflate, write compressed data via gzopen64
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)comp_buf_size;
    int rc_deflate = deflate(&dstrm, 0);
    uLong comp_size = dstrm.total_out;
    int rc_deflate_reset = deflateReset(&dstrm);
    gzFile gzf = gzopen64("zlib_seq_out.gz", "wb+");
    int rc_gzwrite = gzwrite(gzf, (voidpc)comp_buf, (unsigned int)(comp_size ? comp_size : 0));

    // step 4: Cleanup and validation
    int rc_gzclose_w = gzclose_w(gzf);
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflateback_end = inflateBackEnd(&istrm);
    int rc_inflate_end = inflateEnd(&istrm);
    uLong flags = zlibCompileFlags();
    delete [] comp_buf;
    delete [] window;
    (void)version;
    (void)rc_inflateback_init;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)comp_size;
    (void)rc_deflate_reset;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_deflate_end;
    (void)rc_inflateback_end;
    (void)rc_inflate_end;
    (void)flags;
    // API sequence test completed successfully
    return 66;
}