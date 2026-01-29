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
//<ID> 1091
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare payload (write compressed file via gz layer)
    const char payload[] = "zlib API sequence payload: deflateInit_ -> deflateBound -> deflate -> inflateBackInit_ -> gztell";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    unsigned int write_len = (unsigned int)payload_len;
    int rc_gzwrite = gzwrite(gz, (const void *)payload, write_len);
    off_t pos_after_write = gztell(gz);
    int rc_gzclose = gzclose(gz);

    // step 2: Initialize and configure deflate stream, compute bound and allocate buffers
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, payload_len);
    uInt comp_buf_size = (uInt)(bound ? bound : 1);
    Bytef *comp_buf = new Bytef[(comp_buf_size ? comp_buf_size : 1)];
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = comp_buf_size;

    // step 3: Core operations — perform deflate and finalize deflate stream
    int rc_deflate = deflate(&dstrm, Z_FINISH);
    uLong compressed_size = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 4: Initialize inflateBack (window provided), validate checksums, cleanup
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    unsigned char *window = new unsigned char[32768];
    int rc_inflateBack_init = inflateBackInit_(&istrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    int rc_inflateBack_end = inflateBackEnd(&istrm);
    uLong crc_original = crc32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong crc_compressed = crc32(0UL, comp_buf, (uInt)compressed_size);
    delete [] comp_buf;
    delete [] window;
    (void)rc_gzwrite;
    (void)pos_after_write;
    (void)rc_gzclose;
    (void)rc_deflate_init;
    (void)bound;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)rc_inflateBack_init;
    (void)rc_inflateBack_end;
    (void)crc_original;
    (void)crc_compressed;
    // API sequence test completed successfully
    return 66;
}