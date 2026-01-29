#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 970
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    const char *parse_end = "{\"device\":\"sensor\",\"ok\":true,\"count\":7}"; /* ensure non-NULL before use */
    char *unformatted = NULL;
    char *prealloc = NULL;
    char *scratch = NULL;
    const char *raw_json = "{\"device\":\"sensor\",\"ok\":true,\"count\":7}";
    const char *version = NULL;
    int prealloc_len = 256;
    size_t raw_len = strlen(raw_json);
    cJSON_bool prealloc_success = 0;
    size_t scratch_len = 64;

    // step 2: Setup / Configure
    version = cJSON_Version();
    root = cJSON_ParseWithLengthOpts(raw_json, raw_len, &parse_end, 1);
    unformatted = cJSON_PrintUnformatted(root);
    prealloc = (char *)cJSON_malloc((size_t)prealloc_len);
    memset(prealloc, 0, (size_t)prealloc_len);
    prealloc_success = cJSON_PrintPreallocated(root, prealloc, prealloc_len, 0);
    scratch = (char *)cJSON_malloc(scratch_len);
    memset(scratch, 0, scratch_len);

    // step 3: Operate / Validate
    scratch[0] = version ? version[0] : '\0';
    scratch[1] = prealloc[0];
    scratch[2] = unformatted ? unformatted[0] : '\0';
    scratch[3] = parse_end ? *parse_end : '\0';
    scratch[4] = (char)('0' + (int)prealloc_success);

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_free(prealloc);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}