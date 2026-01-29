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
//<ID> 1259
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(settings, "values", values);

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(3.14);
    cJSON *str = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(values, num);
    cJSON_AddItemToArray(values, str);
    cJSON_AddStringToObject(settings, "name", "config1");
    cJSON_AddBoolToObject(settings, "active", 1);

    // step 3: Operate
    cJSON *got_settings = cJSON_GetObjectItem(root, "settings");
    cJSON *got_values = cJSON_GetObjectItem(got_settings, "values");
    cJSON *first = cJSON_GetArrayItem(got_values, 0);
    cJSON_bool first_invalid = cJSON_IsInvalid(first);
    (void)first_invalid;
    cJSON *detached = cJSON_DetachItemViaPointer(got_values, str);
    cJSON_AddItemToObject(root, "detached", detached);
    int buf_len = 512;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);

    // step 4: Validate & Cleanup
    cJSON *got_detached = cJSON_GetObjectItem(root, "detached");
    cJSON_bool detached_invalid = cJSON_IsInvalid(got_detached);
    (void)detached_invalid;
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}