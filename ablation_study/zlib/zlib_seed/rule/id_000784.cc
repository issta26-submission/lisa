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
//<ID> 784
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare buffers and data
    const unsigned int INPUT_LEN = 512;
    unsigned char *input = (unsigned char *)malloc((size_t)INPUT_LEN);
    memset(input, 'Z', (size_t)INPUT_LEN);
    unsigned char *outBuf = (unsigned char *)malloc((size_t)INPUT_LEN);
    memset(outBuf, 0, (size_t)INPUT_LEN);
    const char *version = zlibVersion();

    // step 2: Write data to a gzip file using gzopen/gzwrite and close
    gzFile gz_out = gzopen("tmp_zlib_api_sequence.gz", "wb");
    gzwrite(gz_out, (voidpc)input, (unsigned int)INPUT_LEN);
    gzclose(gz_out);

    // step 3: Read back the data using gzopen/gzread into outBuf and close
    gzFile gz_in = gzopen("tmp_zlib_api_sequence.gz", "rb");
    int read_len = gzread(gz_in, (voidp)outBuf, (unsigned int)INPUT_LEN);
    gzclose(gz_in);

    // step 4: Initialize an inflate stream with the read buffer as next_in, then cleanup
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = NULL;
    strm.zfree = NULL;
    strm.opaque = NULL;
    strm.next_in = (Bytef *)outBuf;
    strm.avail_in = (uInt)(read_len > 0 ? read_len : 0);
    inflateInit_(&strm, version, (int)sizeof(z_stream));
    inflateEnd(&strm);
    free(input);
    free(outBuf);
    (void)version;
    // API sequence test completed successfully
    return 66;
}