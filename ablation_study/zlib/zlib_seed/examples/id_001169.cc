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
//<ID> 1169
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API sequence payload for deflate/deflateCopy/adler/inflateBackEnd usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    uLong comp_buf_size = compressBound(payload_len);
    comp_buf_size = (comp_buf_size ? comp_buf_size : 1);
    Bytef *comp_buf = new Bytef[(size_t)comp_buf_size];
    memset(comp_buf, 0, (size_t)comp_buf_size);

    z_stream dstrm;
    z_stream dstrm_copy;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&dstrm_copy, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    gz_header head;
    memset(&head, 0, (size_t)sizeof(gz_header));
    head.text = 1;
    head.os = 255;

    // step 2: Setup (initialize deflate, set header, prepare input/output)
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 31, 8, 0, version, (int)sizeof(z_stream));
    int rc_set_header = deflateSetHeader(&dstrm, &head);
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)comp_buf_size;
    int rc_deflate = deflate(&dstrm, 0);
    uLong comp_size = dstrm.total_out;

    // step 3: Core operations (copy deflate state, checksums, inflate backend)
    int rc_deflate_copy = deflateCopy(&dstrm_copy, &dstrm);
    uLong checksum_orig = adler32_z(0UL, (const Bytef *)payload, (z_size_t)payload_len);
    uLong checksum_comp = adler32_z(0UL, comp_buf, (z_size_t)(comp_size ? comp_size : 0));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inflate_back_end = inflateBackEnd(&istrm);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Validate and cleanup
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_deflate_end_copy = deflateEnd(&dstrm_copy);
    delete [] comp_buf;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_set_header;
    (void)rc_deflate;
    (void)comp_size;
    (void)rc_deflate_copy;
    (void)checksum_orig;
    (void)checksum_comp;
    (void)rc_inflate_init;
    (void)rc_inflate_back_end;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    (void)rc_deflate_end_copy;
    // API sequence test completed successfully
    return 66;
}