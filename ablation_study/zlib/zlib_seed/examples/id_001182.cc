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
//<ID> 1182
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload demonstrating deflateReset/inflateBackInit_/gzopen64/zlibCompileFlags usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    uLong compile_flags = zlibCompileFlags();
    uLong comp_buf_size = compressBound(payload_len);
    comp_buf_size = (comp_buf_size ? comp_buf_size : 1);
    Bytef *comp_buf = new Bytef[(size_t)comp_buf_size];
    memset(comp_buf, 0, (size_t)comp_buf_size);
    unsigned char *window = new unsigned char[(1 << 15)];
    memset(window, 0, (size_t)(1 << 15));
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (initialize deflate and prepare streams)
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 15, 8, 0, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)comp_buf_size;
    int rc_deflate_run = deflate(&dstrm, 0);
    uLong comp_size = dstrm.total_out;
    int rc_deflate_reset = deflateReset(&dstrm);
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)comp_buf_size;
    int rc_deflate_run2 = deflate(&dstrm, 0);
    comp_size = dstrm.total_out;

    // step 3: Core operations (inflate-back init and gz file operations)
    int rc_inflateback_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    gzFile gzf = gzopen64("zlib_seq_out.gz", "wb+");
    unsigned int gz_buf_size = (unsigned int)((compile_flags & 0xFFFFu) + 1u);
    int rc_gzbuffer = gzbuffer(gzf, gz_buf_size);
    int rc_gzwrite = gzwrite(gzf, (voidpc)comp_buf, (unsigned int)(comp_size ? comp_size : 0));
    int rc_gzclose_w = gzclose_w(gzf);
    int rc_inflateback_end = inflateBackEnd(&istrm);

    // step 4: Cleanup and finalize
    int rc_deflate_end = deflateEnd(&dstrm);
    delete [] comp_buf;
    delete [] window;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_deflate_run;
    (void)rc_deflate_reset;
    (void)rc_deflate_run2;
    (void)rc_inflateback_init;
    (void)rc_gzbuffer;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_inflateback_end;
    (void)rc_deflate_end;
    (void)compile_flags;
    // API sequence test completed successfully
    return 66;
}