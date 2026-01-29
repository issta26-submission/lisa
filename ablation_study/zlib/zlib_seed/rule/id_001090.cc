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
//<ID> 1090
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffer
    const uInt BUFLEN = 128U;
    Bytef *buf = (Bytef *)malloc((size_t)BUFLEN);
    memset(buf, 'Z', (size_t)BUFLEN);
    const char *ver = zlibVersion();

    // step 2: Initialize inflateBack stream and window
    z_stream bstrm;
    memset(&bstrm, 0, sizeof(bstrm));
    bstrm.zalloc = NULL;
    bstrm.zfree = NULL;
    bstrm.opaque = NULL;
    unsigned char *window = (unsigned char *)malloc(1 << 15);
    memset(window, 0, (size_t)(1 << 15));
    inflateBackInit_(&bstrm, 15, window, ver, (int)sizeof(z_stream));

    // step 3: gzFile operations using gzdopen, gzwrite, gzgetc_ and gztell
    gzFile gz = gzdopen(1, "wb");
    gzwrite(gz, buf, (unsigned int)BUFLEN);
    off_t pos = gztell(gz);
    int ch = gzgetc_(gz);

    // step 4: Cleanup and finalize APIs
    inflateBackEnd(&bstrm);
    gzclose(gz);
    free(window);
    free(buf);
    (void)pos;
    (void)ch;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}