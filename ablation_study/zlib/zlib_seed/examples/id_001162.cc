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
//<ID> 1162
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API exercise payload for compress/deflate/back usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    uLong comp_bound = compressBound(payload_len);
    Bytef *comp_buf = new Bytef[(comp_bound ? comp_bound : 1)];
    memset(comp_buf, 0, (size_t)(comp_bound ? comp_bound : 1));
    z_stream dstrm;
    z_stream dstrm_copy;
    z_stream bstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&dstrm_copy, 0, (size_t)sizeof(z_stream));
    memset(&bstrm, 0, (size_t)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (size_t)sizeof(gz_header));
    head.text = 1;
    head.os = 255;

    // step 2: Setup and configure deflate stream
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 31, 8, 0, version, (int)sizeof(z_stream));
    int rc_set_header = deflateSetHeader(&dstrm, &head);
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)(comp_bound ? comp_bound : 1);

    // step 3: Operate - copy stream, run deflate, compute checksums
    int rc_deflate_copy = deflateCopy(&dstrm_copy, &dstrm);
    int rc_deflate = deflate(&dstrm, 0);
    uLong comp_size = dstrm.total_out;
    uLong checksum_orig = adler32_z(0UL, (const Bytef *)payload, (z_size_t)payload_len);
    uLong checksum_comp = adler32_z(0UL, comp_buf, (z_size_t)(comp_size ? comp_size : 0));

    // step 4: Inflate-back cleanup and finalization
    unsigned char *window = new unsigned char[1];
    memset(window, 0, 1);
    int rc_inflate_back_init = inflateBackInit_(&bstrm, 15, window, version, (int)sizeof(z_stream));
    int rc_inflate_back_end = inflateBackEnd(&bstrm);
    int rc_deflate_end_orig = deflateEnd(&dstrm);
    int rc_deflate_end_copy = deflateEnd(&dstrm_copy);
    delete [] comp_buf;
    delete [] window;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_set_header;
    (void)rc_deflate_copy;
    (void)rc_deflate;
    (void)comp_size;
    (void)checksum_orig;
    (void)checksum_comp;
    (void)rc_inflate_back_init;
    (void)rc_inflate_back_end;
    (void)rc_deflate_end_orig;
    (void)rc_deflate_end_copy;
    // API sequence test completed successfully
    return 66;
}