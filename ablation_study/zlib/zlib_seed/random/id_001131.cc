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
//<ID> 1131
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API sequence payload demonstrating deflate/inflateBack usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (initialize deflate and prepare buffers)
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong comp_bound_ul = deflateBound(&dstrm, payload_len);
    uLong alloc_comp = (comp_bound_ul ? comp_bound_ul : 1UL);
    Bytef *comp_buf = new Bytef[(size_t)alloc_comp];
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)(alloc_comp > (uLong)0xFFFFFFFFU ? 0xFFFFFFFFU : (uInt)alloc_comp);

    // step 3: Core operations (compress, write to gz, initialize inflateBack)
    int rc_deflate = deflate(&dstrm, 0);
    uInt comp_size = (uInt)dstrm.total_out;
    gzFile gzf = gzopen("zlib_api_sequence_test.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, comp_buf, (unsigned int)comp_size);
    unsigned char *window_buf = new unsigned char[32768];
    int rc_inflateback_init = inflateBackInit_(&istrm, 15, window_buf, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = comp_size;
    Bytef *out_buf = new Bytef[(size_t)((payload_len + 64) ? (payload_len + 64) : 1)];
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(payload_len + 64);

    // step 4: Validate and cleanup
    uLong crc_orig = crc32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong crc_comp = crc32(0UL, (const Bytef *)comp_buf, (uInt)comp_size);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_gzclose_r = gzclose_r(gzf);
    delete [] comp_buf;
    delete [] window_buf;
    delete [] out_buf;
    (void)rc_deflate_init;
    (void)comp_bound_ul;
    (void)alloc_comp;
    (void)rc_deflate;
    (void)comp_size;
    (void)rc_gzwrite;
    (void)rc_inflateback_init;
    (void)crc_orig;
    (void)crc_comp;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    (void)rc_gzclose_r;
    // API sequence test completed successfully
    return 66;
}