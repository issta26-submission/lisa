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
//<ID> 2108
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *num = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(num, 123.0);
    cJSON_AddItemToObject(config, "threshold", num);

    // step 2: Configure
    cJSON_AddStringToObject(root, "name", "fuzz_test");
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddStringToObject(meta, "version", "1.0");

    // step 3: Operate and Validate
    cJSON *detached_threshold = cJSON_DetachItemFromObject(config, "threshold");
    cJSON_SetNumberHelper(detached_threshold, 42.42);
    cJSON_AddItemToObject(root, "exposed_threshold", detached_threshold);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 0);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}