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
//<ID> 1096
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API sequence payload for combined deflate and gz usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    const char *ver = zlibVersion();

    // step 2: Setup (initialize deflate, compute bound, prepare buffers, initialize inflateBack)
    int rc_deflate_init = deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong comp_bound = deflateBound(&dstrm, payload_len);
    Bytef *comp_buf = new Bytef[(comp_bound ? comp_bound : 1)];
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)comp_bound;
    int rc_deflate = deflate(&dstrm, 0);
    uLong comp_used = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);
    unsigned char *inflate_window = new unsigned char[32768];
    int rc_inflate_back_init = inflateBackInit_(&istrm, 15, inflate_window, ver, (int)sizeof(z_stream));

    // step 3: Operate (write compressed buffer to a gz file and get file offset)
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    unsigned int write_len = (unsigned int)comp_used;
    int rc_gzwrite = gzwrite(gw, (const void *)comp_buf, write_len);
    off_t pos = gztell(gw);
    int rc_gzclose = gzclose(gw);

    // step 4: Validate and cleanup (checksums, teardown, free buffers)
    uLong ad_payload = adler32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong ad_comp = adler32(0UL, comp_buf, (uInt)comp_used);
    int rc_inflate_back_end = inflateBackEnd(&istrm);
    delete [] comp_buf;
    delete [] inflate_window;
    (void)rc_deflate_init;
    (void)comp_bound;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)rc_inflate_back_init;
    (void)rc_gzwrite;
    (void)pos;
    (void)rc_gzclose;
    (void)ad_payload;
    (void)ad_comp;
    (void)rc_inflate_back_end;
    // API sequence test completed successfully
    return 66;
}