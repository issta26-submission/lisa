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
//<ID> 1415
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "version", 1.0);
    cJSON_AddBoolToObject(meta, "active", (cJSON_bool)1);
    cJSON *first = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(root, "first", first);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *shared_str = cJSON_CreateString("shared");
    cJSON *num_item = cJSON_CreateNumber(3.14);
    cJSON_AddItemReferenceToArray(items, num_item);
    cJSON_AddItemReferenceToArray(items, shared_str);
    cJSON *meta_copy = cJSON_Duplicate(meta, (cJSON_bool)1);
    cJSON_AddItemToObject(root, "meta_copy", meta_copy);

    // step 3: Operate & Validate
    cJSON *meta_ptr = cJSON_GetObjectItem(root, "meta");
    cJSON *meta_copy_ptr = cJSON_GetObjectItem(root, "meta_copy");
    double v1 = cJSON_GetNumberValue(cJSON_GetObjectItem(meta_ptr, "version"));
    double v2 = cJSON_GetNumberValue(cJSON_GetObjectItem(meta_copy_ptr, "version"));
    cJSON_AddNumberToObject(root, "version_sum", v1 + v2);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddRawToObject(root, "snapshot", snapshot);
    cJSON_free((void *)snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}