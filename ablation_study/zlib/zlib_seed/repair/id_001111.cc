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
//<ID> 1111
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize source data and write a .gz file
    const Bytef source[] = "zlib API sequence payload for gzopen/gzwrite/gzrewind/gzread test.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gw_write_ret = gzwrite(gw, source, (unsigned int)source_len);
    int gw_close_ret = gzclose(gw);

    // step 2: Open the .gz file for reading, rewind, and prepare read buffer
    gzFile gr = gzopen("tmp_zlib_api_seq.gz", "rb");
    int gr_rewind_ret = gzrewind(gr);
    uLong read_alloc = source_len + 64;
    void *read_buf = malloc((size_t)read_alloc);
    memset(read_buf, 0, (size_t)read_alloc);

    // step 3: Read from the gz file and initialize an inflate stream for validation
    int bytes_read = gzread(gr, read_buf, (unsigned int)read_alloc);
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));

    // step 4: Validate with crc32, cleanup inflate stream and gz file, free buffers
    uLong checksum = crc32(0UL, (const Bytef *)read_buf, (uInt)bytes_read);
    int inf_end_ret = inflateEnd(&inf_strm);
    int gr_close_ret = gzclose(gr);
    free(read_buf);

    (void)version;
    (void)gw_write_ret;
    (void)gw_close_ret;
    (void)gr_rewind_ret;
    (void)bytes_read;
    (void)inf_init_ret;
    (void)checksum;
    (void)inf_end_ret;
    (void)gr_close_ret;
    return 66;
    // API sequence test completed successfully
}