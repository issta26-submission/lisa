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
//<ID> 1160
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload to exercise compress/deflate/copy/headers/checksum/inflateBack";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream dsrc;
    z_stream dcopy;
    z_stream istrm_back;
    gz_header head;
    memset(&dsrc, 0, (size_t)sizeof(z_stream));
    memset(&dcopy, 0, (size_t)sizeof(z_stream));
    memset(&istrm_back, 0, (size_t)sizeof(z_stream));
    memset(&head, 0, (size_t)sizeof(gz_header));
    head.text = 1;
    head.os = 255;

    // step 2: Setup
    uLong bound = compressBound(payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    memset(comp_buf, 0, (size_t)(bound ? bound : 1));
    int rc_deflate_init = deflateInit2_(&dsrc, 6, 8, 31, 8, 0, version, (int)sizeof(z_stream));
    int rc_set_header = deflateSetHeader(&dsrc, &head);
    dsrc.next_in = (Bytef *)payload;
    dsrc.avail_in = (uInt)payload_len;
    dsrc.next_out = comp_buf;
    dsrc.avail_out = (uInt)(bound ? bound : 1);
    int rc_deflate_copy = deflateCopy(&dcopy, &dsrc);

    // step 3: Core operations
    int rc_deflate = deflate(&dsrc, 0);
    uLong comp_size = dsrc.total_out;
    uLong checksum_orig = adler32_z(0UL, (const Bytef *)payload, (z_size_t)payload_len);
    uLong checksum_comp = adler32_z(0UL, comp_buf, (z_size_t)(comp_size ? comp_size : 0));
    unsigned char *window = new unsigned char[32768];
    memset(window, 0, (size_t)32768);
    int rc_inflate_back_init = inflateBackInit_(&istrm_back, 15, window, version, (int)sizeof(z_stream));
    int rc_inflate_back_end = inflateBackEnd(&istrm_back);

    // step 4: Validate and cleanup
    int rc_deflate_end_src = deflateEnd(&dsrc);
    int rc_deflate_end_copy = deflateEnd(&dcopy);
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
    (void)rc_deflate_end_src;
    (void)rc_deflate_end_copy;
    // API sequence test completed successfully
    return 66;
}