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
//<ID> 1158
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "key", child);
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "nested", nested);

    // step 2: Configure
    char *prev_value1 = cJSON_SetValuestring(child, "updated");
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON_bool child_is_invalid = cJSON_IsInvalid(child);
    cJSON_AddNumberToObject(root, "counter", 1.0);

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromObject(root, "key");
    char *prev_value2 = cJSON_SetValuestring(detached, "detached_value");
    cJSON_AddItemToObject(root, "reattached", detached);

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)prev_value1;
    (void)root_is_obj;
    (void)child_is_invalid;
    (void)prev_value2;
    return 66;
}