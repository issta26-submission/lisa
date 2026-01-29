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
//<ID> 1165
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API sequence payload for compress/deflate/inflate usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(payload_len);
    Bytef *comp_buf = new Bytef[(size_t)(bound ? bound : 1)];
    memset(comp_buf, 0, (size_t)(bound ? bound : 1));
    z_stream dstrm;
    z_stream dstrm_copy;
    z_stream istrm;
    z_stream back_strm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&dstrm_copy, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    memset(&back_strm, 0, (size_t)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (size_t)sizeof(gz_header));
    head.text = 1;
    head.os = 255;

    // step 2: Setup / Configure deflate and header, prepare buffers
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 31, 8, 0, version, (int)sizeof(z_stream));
    int rc_set_header = deflateSetHeader(&dstrm, &head);
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)(bound ? bound : 1);

    // step 3: Core operations — deflate, copy, write, inflate setup, checksums, inflateBack init/end
    int rc_deflate = deflate(&dstrm, 0);
    uLong comp_size = dstrm.total_out;
    int rc_deflate_copy = deflateCopy(&dstrm_copy, &dstrm);
    int rc_deflate_end_src = deflateEnd(&dstrm);
    gzFile gzf = gzopen("zlib_api_sequence_out.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, comp_buf, (unsigned int)(comp_size ? comp_size : 0));
    int rc_gzclose = gzclose_w(gzf);
    uLong adler_orig = adler32_z(0UL, (const Bytef *)payload, (z_size_t)payload_len);
    uLong adler_comp = adler32_z(0UL, comp_buf, (z_size_t)(comp_size ? comp_size : 0));
    int rc_inflate_init = inflateInit2_(&istrm, 31, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_size ? comp_size : 0);
    Bytef *decomp_buf = new Bytef[(size_t)(payload_len ? payload_len : 1)];
    memset(decomp_buf, 0, (size_t)(payload_len ? payload_len : 1));
    istrm.next_out = decomp_buf;
    istrm.avail_out = (uInt)(payload_len ? payload_len : 1);
    int rc_inflate = inflate(&istrm, 0);
    // initialize an inflateBack stream and then clean it up with inflateBackEnd
    Bytef *window_buf = new Bytef[1];
    memset(window_buf, 0, 1);
    int rc_inflateback_init = inflateBackInit_(&back_strm, 31, window_buf, version, (int)sizeof(z_stream));
    int rc_inflateback_end = inflateBackEnd(&back_strm);

    // step 4: Validate and cleanup
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_deflate_end_copy = deflateEnd(&dstrm_copy);
    delete [] comp_buf;
    delete [] decomp_buf;
    delete [] window_buf;
    (void)version;
    (void)bound;
    (void)rc_deflate_init;
    (void)rc_set_header;
    (void)rc_deflate;
    (void)comp_size;
    (void)rc_deflate_copy;
    (void)rc_deflate_end_src;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)adler_orig;
    (void)adler_comp;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflateback_init;
    (void)rc_inflateback_end;
    (void)rc_inflate_end;
    (void)rc_deflate_end_copy;
    // API sequence test completed successfully
    return 66;
}