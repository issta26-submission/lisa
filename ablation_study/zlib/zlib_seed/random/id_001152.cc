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
//<ID> 1152
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API sequence payload for deflate/inflate/gz usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    z_stream istrm;
    z_stream istrm_copy;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm_copy, 0, (size_t)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (size_t)sizeof(gz_header));
    const char namebuf[] = "example-name";
    head.name = (Bytef *)namebuf;
    head.name_max = (uInt)(sizeof(namebuf));
    head.comment = (Bytef *)"example-comment";
    head.comm_max = (uInt)16;
    head.hcrc = 0;

    // step 2: Setup (initialize deflate, set header, prepare buffers)
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_deflate_set_header = deflateSetHeader(&dstrm, &head);
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    uLong bound = deflateBound(&dstrm, payload_len);
    size_t out_size = (size_t)(bound + 1UL);
    Bytef *comp_buf = new Bytef[out_size];
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)out_size;

    // step 3: Core operations (deflate, write to gz files, initialize inflate with windowBits, copy inflate state, inflate)
    int rc_deflate = deflate(&dstrm, 0);
    uLong comp_size = dstrm.total_out;
    gzFile gzw = gzopen("zlib_api_sequence_out.gz", "wb");
    gzFile gzw64 = gzopen64("zlib_api_sequence_out64.gz", "wb");
    int rc_gzwrite1 = gzwrite(gzw, comp_buf, (unsigned int)comp_size);
    int rc_gzwrite2 = gzwrite(gzw64, comp_buf, (unsigned int)comp_size);
    int rc_inflate_init2 = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_size;
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_copy = inflateCopy(&istrm_copy, &istrm);
    int rc_inflate_copy_end = inflateEnd(&istrm_copy);

    // step 4: Validate and cleanup
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_gzclose1 = gzclose_w(gzw);
    int rc_gzclose2 = gzclose_w(gzw64);
    delete [] comp_buf;
    (void)rc_deflate_init;
    (void)rc_deflate_set_header;
    (void)rc_deflate;
    (void)bound;
    (void)out_size;
    (void)rc_gzwrite1;
    (void)rc_gzwrite2;
    (void)rc_inflate_init2;
    (void)rc_inflate;
    (void)rc_inflate_copy;
    (void)rc_inflate_copy_end;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    (void)rc_gzclose1;
    (void)rc_gzclose2;
    // API sequence test completed successfully
    return 66;
}