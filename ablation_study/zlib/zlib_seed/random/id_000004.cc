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
//<ID> 4
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const unsigned char sample[] = "Hello zlib API sequence";
    uLong input_len = (uLong)(sizeof(sample) - 1);
    unsigned char *in_buf = new unsigned char[input_len];
    memcpy(in_buf, sample, (size_t)input_len);
    uLong bound = compressBound(input_len);
    unsigned char *comp_buf = new unsigned char[bound];
    strm.next_in = (Bytef *)in_buf;
    strm.avail_in = (uInt)input_len;
    strm.next_out = (Bytef *)comp_buf;
    strm.avail_out = (uInt)bound;

    // step 2
    deflateInit_(&strm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflate(&strm, 0);
    uLong comp_size = strm.total_out;
    deflateEnd(&strm);

    // step 3
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char *decomp_buf = new unsigned char[input_len];
    istrm.next_in = (Bytef *)comp_buf;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = (Bytef *)decomp_buf;
    istrm.avail_out = (uInt)input_len;
    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    inflate(&istrm, 0);
    inflateSyncPoint(&istrm);
    inflateBackEnd(&istrm);
    inflateEnd(&istrm);

    // step 4
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzprintf(gz, "zlib API sequence test file\n");
    gzclose(gz);
    delete[] in_buf;
    delete[] comp_buf;
    delete[] decomp_buf;

    // API sequence test completed successfully
    return 66;
}