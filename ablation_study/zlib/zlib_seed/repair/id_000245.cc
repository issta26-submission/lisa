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
//<ID> 245
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream defstrm;
    z_stream infstrm;
    Bytef input_buf[] = "zlib API sequence: deflateTune -> deflate -> write via gz (with buffer) -> gzseek64 -> inflate";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef compbuf[65536];
    Bytef decompbuf[65536];
    const char *ver;
    gzFile gz;
    int ret_deflate_init;
    int ret_deflate_tune;
    int ret_deflate_call;
    int ret_deflate_end;
    int ret_gz_buf;
    int ret_gz_write;
    int ret_gz_close;
    off64_t gz_pos;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_end;
    uLong comp_len;
    uLong decomp_len;

    // step 2: Initialize streams and perform deflate (initialize -> configure -> operate)
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&infstrm, 0, sizeof(infstrm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(decompbuf, 0, sizeof(decompbuf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&defstrm, 6, ver, (int)sizeof(z_stream));
    ret_deflate_tune = deflateTune(&defstrm, 128, 64, 258, 4096);
    defstrm.next_in = input_buf;
    defstrm.avail_in = (uInt)input_len;
    defstrm.next_out = compbuf;
    defstrm.avail_out = (uInt)sizeof(compbuf);
    ret_deflate_call = deflate(&defstrm, 4);
    comp_len = defstrm.total_out;
    ret_deflate_end = deflateEnd(&defstrm);

    // step 3: Use gz API to write original data with buffering and perform a 64-bit seek
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_buf = gzbuffer(gz, 8192);
    ret_gz_write = gzwrite(gz, (voidpc)input_buf, (unsigned int)input_len);
    gz_pos = gzseek64(gz, 0, 0);
    ret_gz_close = gzclose(gz);

    // step 4: Inflate the compressed data, validate flow, and cleanup
    ret_inflate_init = inflateInit_(&infstrm, ver, (int)sizeof(z_stream));
    infstrm.next_in = compbuf;
    infstrm.avail_in = (uInt)comp_len;
    infstrm.next_out = decompbuf;
    infstrm.avail_out = (uInt)sizeof(decompbuf);
    ret_inflate_call = inflate(&infstrm, 4);
    decomp_len = infstrm.total_out;
    ret_inflate_end = inflateEnd(&infstrm);

    (void)ret_deflate_init;
    (void)ret_deflate_tune;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_gz_buf;
    (void)ret_gz_write;
    (void)ret_gz_close;
    (void)gz_pos;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_end;
    (void)comp_len;
    (void)decomp_len;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}