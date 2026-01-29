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
//<ID> 1164
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for deflate/copy/adler/inflateBack usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    z_stream dstrm_copy;
    z_stream backstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&dstrm_copy, 0, (size_t)sizeof(z_stream));
    memset(&backstrm, 0, (size_t)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (size_t)sizeof(gz_header));
    head.text = 1;
    head.os = 255;
    uLong cbound = compressBound(payload_len);
    uLongf comp2_len = (uLongf)(cbound ? cbound : 1);
    Bytef *comp2_buf = new Bytef[(comp2_len ? comp2_len : 1)];
    memset(comp2_buf, 0, (size_t)(comp2_len ? comp2_len : 1));

    // step 2: Setup
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 31, 8, 0, version, (int)sizeof(z_stream));
    int rc_set_header = deflateSetHeader(&dstrm, &head);
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    uLong bound = deflateBound(&dstrm, payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    memset(comp_buf, 0, (size_t)(bound ? bound : 1));
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)(bound ? bound : 1);

    // step 3: Core operations
    int rc_deflate = deflate(&dstrm, 0);
    uLong comp_size = dstrm.total_out;
    int rc_deflate_copy = deflateCopy(&dstrm_copy, &dstrm);
    int rc_compress = compress(comp2_buf, &comp2_len, (const Bytef *)payload, payload_len);
    uLong ad_orig = adler32_z(0UL, (const Bytef *)payload, (z_size_t)payload_len);
    uLong ad_comp = adler32_z(0UL, comp_buf, (z_size_t)(comp_size ? comp_size : 0));
    unsigned char *window = new unsigned char[32768];
    memset(window, 0, (size_t)32768);
    int rc_inflate_back_init = inflateBackInit_(&backstrm, 15, window, version, (int)sizeof(z_stream));

    // step 4: Validate and cleanup
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_deflate_end_copy = deflateEnd(&dstrm_copy);
    int rc_inflate_back_end = inflateBackEnd(&backstrm);
    delete [] comp_buf;
    delete [] comp2_buf;
    delete [] window;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_set_header;
    (void)rc_deflate;
    (void)rc_deflate_copy;
    (void)rc_compress;
    (void)ad_orig;
    (void)ad_comp;
    (void)bound;
    (void)cbound;
    (void)comp_size;
    (void)comp2_len;
    (void)rc_inflate_back_init;
    (void)rc_deflate_end;
    (void)rc_deflate_end_copy;
    (void)rc_inflate_back_end;
    // API sequence test completed successfully
    return 66;
}