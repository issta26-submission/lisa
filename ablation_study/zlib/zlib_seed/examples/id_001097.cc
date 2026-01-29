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
//<ID> 1097
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for deflate, gz write and inflateBackInit";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure (set input, compute bound, allocate output, prepare inflateBack window)
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    uLong out_bound = deflateBound(&dstrm, payload_len);
    Bytef * out_buf = new Bytef[(out_bound ? out_bound : 1)];
    dstrm.next_out = out_buf;
    dstrm.avail_out = (uInt)(out_bound ? out_bound : 1);
    unsigned char * window = new unsigned char[32768];
    int rc_inflateback_init = inflateBackInit_(&istrm, 15, window, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Core operations (perform deflate, write compressed output to gz file, inspect position)
    int rc_deflate = deflate(&dstrm, 0);
    uInt comp_size = (uInt)dstrm.total_out;
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (const void *)out_buf, comp_size);
    off_t gpos = gztell(gzf);
    int rc_gzclose = gzclose(gzf);

    // step 4: Validate and Cleanup (compute checksums, end streams, free buffers)
    uLong crc_payload = crc32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong crc_comp = crc32(0UL, out_buf, (uInt)comp_size);
    uLong crc_combined = crc32_combine(crc_payload, crc_comp, (off_t)comp_size);
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflateback_end = inflateBackEnd(&istrm);
    delete [] out_buf;
    delete [] window;
    (void)rc_deflate_init;
    (void)rc_inflateback_init;
    (void)rc_deflate;
    (void)rc_gzwrite;
    (void)gpos;
    (void)rc_gzclose;
    (void)crc_payload;
    (void)crc_comp;
    (void)crc_combined;
    (void)rc_deflate_end;
    (void)rc_inflateback_end;
    // API sequence test completed successfully
    return 66;
}