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
//<ID> 2253
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *enabled = cJSON_AddTrueToObject(config, "enabled");
    cJSON *meta_ref = cJSON_CreateObjectReference(meta);
    cJSON_AddItemToObject(config, "meta_ref", meta_ref);

    // step 3: Operate and Validate
    cJSON *got_enabled = cJSON_GetObjectItem(config, "enabled");
    cJSON_bool is_true = cJSON_IsTrue(got_enabled);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = (char)('0' + (int)is_true);
    buffer[1] = buffer[0];
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}