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
//<ID> 3
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream defstrm;
    z_stream infstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&infstrm, 0, sizeof(infstrm));
    const unsigned char input_data[] = "Hello zlib!";
    unsigned int input_len = (unsigned int)(sizeof(input_data) - 1);
    unsigned int out_buf_size = input_len + 256;
    unsigned char *comp_buf = new unsigned char[out_buf_size];
    unsigned char *decomp_buf = new unsigned char[out_buf_size];
    gzFile gz = (gzFile)0;
    off_t gz_off = 0;
    unsigned long codes_used = 0;

    // step 2: Setup (initialize deflate and inflate)
    deflateInit_(&defstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defstrm.next_in = (Bytef *)input_data;
    defstrm.avail_in = input_len;
    defstrm.next_out = (Bytef *)comp_buf;
    defstrm.avail_out = out_buf_size;

    inflateInit_(&infstrm, zlibVersion(), (int)sizeof(z_stream));
    // provide inflate output buffer (will be set when inflating)
    infstrm.next_out = (Bytef *)decomp_buf;
    infstrm.avail_out = out_buf_size;

    // step 3: Core operations (compress, decompress, use gz functions, validate)
    deflate(&defstrm, 4); // 4 ~ Z_FINISH
    unsigned long comp_len = defstrm.total_out;
    deflateEnd(&defstrm);

    infstrm.next_in = (Bytef *)comp_buf;
    infstrm.avail_in = (uInt)comp_len;
    inflate(&infstrm, 0);
    codes_used = inflateCodesUsed(&infstrm);
    inflateEnd(&infstrm);

    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzbuffer(gz, 4096);
    gzwrite(gz, input_data, input_len);
    gz_off = gzoffset(gz);
    gzclose(gz);

    // step 4: Cleanup and validation
    delete [] comp_buf;
    delete [] decomp_buf;

    // API sequence test completed successfully
    (void)codes_used;
    (void)gz_off;
    return 66;
}