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
//<ID> 1962
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
    cJSON *name = cJSON_CreateString("example_project");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *enabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(config, "enabled", enabled);

    // step 2: Configure
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *it1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, it1);
    cJSON *it2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, it2);
    cJSON *count = cJSON_CreateNumber(2.0);
    cJSON_AddItemToObject(root, "count", count);

    // step 3: Operate and Validate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_name = cJSON_GetObjectItem(got_config, "name");
    cJSON *got_enabled = cJSON_GetObjectItem(got_config, "enabled");
    char *name_val = cJSON_GetStringValue(got_name);
    cJSON_bool enabled_is_false = cJSON_IsFalse(got_enabled);
    double count_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = (char)('0' + (enabled_is_false % 10));
    buffer[1] = (char)('0' + ((name_val != NULL ? (int)name_val[0] : 0) % 10));
    buffer[2] = (char)('0' + (((int)count_val) % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}