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
//<ID> 35
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream dstrm;
    z_stream istrm;
    unsigned char input_data[] = "The quick brown fox jumps over the lazy dog";
    uInt input_len = (uInt)(sizeof(input_data) - 1);
    unsigned char comp_buf[512];
    unsigned char recon_buf[512];
    gzFile gzf;
    off_t file_offset;
    unsigned long codes_used;
    int ret_deflate_init;
    int ret_inflate_init;
    int ret_deflate;
    int ret_inflate;
    int ret_deflate_end;
    int ret_inflate_end;
    int bytes_written;
    int ret_gzopen;
    int ret_gzclose;

    // step 2: Setup
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    ret_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    ret_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)input_data;
    dstrm.avail_in = input_len;
    dstrm.next_out = (Bytef *)comp_buf;
    dstrm.avail_out = (uInt)sizeof(comp_buf);
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    ret_gzopen = (gzf != (gzFile)0) ? 0 : -1;

    // step 3: Core operations (compress, write, then inflate from compressed buffer and inspect)
    ret_deflate = deflate(&dstrm, 0);
    unsigned int compressed_len = (unsigned int)dstrm.total_out;
    bytes_written = gzwrite(gzf, comp_buf, compressed_len);
    istrm.next_in = (Bytef *)comp_buf;
    istrm.avail_in = compressed_len;
    istrm.next_out = (Bytef *)recon_buf;
    istrm.avail_out = (uInt)sizeof(recon_buf);
    ret_inflate = inflate(&istrm, 0);
    codes_used = inflateCodesUsed(&istrm);
    file_offset = gzoffset(gzf);
    gzclearerr(gzf);

    // step 4: Cleanup
    ret_deflate_end = deflateEnd(&dstrm);
    ret_inflate_end = inflateEnd(&istrm);
    ret_gzclose = gzclose(gzf);

    // API sequence test completed successfully
    (void)ret_deflate_init; (void)ret_inflate_init; (void)ret_deflate; (void)ret_inflate;
    (void)bytes_written; (void)compressed_len; (void)codes_used; (void)file_offset;
    (void)ret_deflate_end; (void)ret_inflate_end; (void)ret_gzopen; (void)ret_gzclose;
    return 66;
}