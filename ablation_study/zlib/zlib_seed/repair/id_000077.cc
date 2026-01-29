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
//<ID> 77
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef comp_data[] = {
        0x78, 0x9c, 0xf3, 0x48, 0xcd, 0xc9, 0xc9, 0x07, 0x00, 0x06, 0x2c, 0x02, 0x15
    };
    uInt comp_len = (uInt)sizeof(comp_data);
    Bytef outbuf[256];
    gzFile gzf;
    const char * ver;
    int ret_init;
    int ret_inflate;
    int bytes_written;
    uLong decompressed_len;

    // step 2: Setup (initialize stream and open gz file)
    memset(&strm, 0, sizeof(strm));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    ret_init = inflateInit_(&strm, ver, (int)sizeof(z_stream));
    strm.next_in = comp_data;
    strm.avail_in = comp_len;
    strm.next_out = outbuf;
    strm.avail_out = (uInt)sizeof(outbuf);
    gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");

    // step 3: Core operations (inflate then write decompressed data to gz file)
    ret_inflate = inflate(&strm, 0);
    decompressed_len = strm.total_out;
    bytes_written = gzwrite(gzf, outbuf, (unsigned int)decompressed_len);

    // step 4: Cleanup (end stream and close file)
    inflateEnd(&strm);
    gzclose(gzf);

    (void)ver;
    (void)ret_init;
    (void)ret_inflate;
    (void)decompressed_len;
    (void)bytes_written;

    // API sequence test completed successfully
    return 66;
}