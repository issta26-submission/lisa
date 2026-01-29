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
//<ID> 346
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare payload and query zlib version
    const char *version = zlibVersion();
    const char payload[] = "zlib API sequence payload: gzopen -> gzwrite -> gzread with inflateEnd cleanup";
    unsigned int payloadLen = (unsigned int)(sizeof(payload) - 1);

    // step 2: Write compressed data to a gzip file using gzopen + gzwrite
    gzFile write_file = gzopen("test_zlib_api_sequence.gz", "wb");
    int write_ret = gzwrite(write_file, (voidpc)payload, payloadLen);
    int write_close_ret = gzclose(write_file);

    // step 3: Read back decompressed data using gzopen + gzread
    char read_buf[512];
    memset(read_buf, 0, sizeof(read_buf));
    gzFile read_file = gzopen("test_zlib_api_sequence.gz", "rb");
    int read_ret = gzread(read_file, (void *)read_buf, payloadLen);
    int read_close_ret = gzclose(read_file);

    // step 4: Initialize an inflate stream and then clean it up with inflateEnd
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int inf_end_ret = inflateEnd(&istrm);

    (void)version;
    (void)payloadLen;
    (void)write_ret;
    (void)write_close_ret;
    (void)read_ret;
    (void)read_close_ret;
    (void)read_buf;
    (void)inf_init_ret;
    (void)inf_end_ret;

    // API sequence test completed successfully
    return 66;
}