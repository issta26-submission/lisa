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
//<ID> 1163
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API sequence payload demonstrating deflate copy, header, adler32 and inflateBackEnd";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    z_stream dstrm_copy;
    z_stream bstrm;
    gz_header head;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&dstrm_copy, 0, (size_t)sizeof(z_stream));
    memset(&bstrm, 0, (size_t)sizeof(z_stream));
    memset(&head, 0, (size_t)sizeof(gz_header));
    head.text = 1;
    head.os = 255;

    // step 2: Setup (initialize deflate, set header, prepare buffers)
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 31, 8, 0, version, (int)sizeof(z_stream));
    int rc_set_header = deflateSetHeader(&dstrm, &head);
    uLong bound = compressBound(payload_len);
    Bytef *comp_buf = new Bytef[(size_t)(bound ? bound : 1)];
    memset(comp_buf, 0, (size_t)(bound ? bound : 1));
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)(bound ? bound : 1);

    // step 3: Core operations (deflate, copy state, checksum, inflateBack init/end)
    int rc_deflate = deflate(&dstrm, 0);
    int rc_deflate_copy = deflateCopy(&dstrm_copy, &dstrm);
    uLong comp_size = dstrm.total_out;
    uLong adler_orig = adler32_z(0UL, (const Bytef *)payload, (z_size_t)payload_len);
    uLong adler_comp = adler32_z(0UL, comp_buf, (z_size_t)(comp_size ? comp_size : 0));
    unsigned char *window = new unsigned char[32768];
    memset(window, 0, (size_t)32768);
    int rc_inflate_back_init = inflateBackInit_(&bstrm, 15, window, version, (int)sizeof(z_stream));
    int rc_inflate_back_end = inflateBackEnd(&bstrm);

    // step 4: Validate and cleanup
    int rc_deflate_end_src = deflateEnd(&dstrm);
    int rc_deflate_end_copy = deflateEnd(&dstrm_copy);
    delete [] comp_buf;
    delete [] window;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_set_header;
    (void)rc_deflate;
    (void)rc_deflate_copy;
    (void)adler_orig;
    (void)adler_comp;
    (void)comp_size;
    (void)rc_inflate_back_init;
    (void)rc_inflate_back_end;
    (void)rc_deflate_end_src;
    (void)rc_deflate_end_copy;
    // API sequence test completed successfully
    return 66;
}