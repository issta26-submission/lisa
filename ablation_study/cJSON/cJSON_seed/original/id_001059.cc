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
//<ID> 1059
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"items\":[{\"id\":1,\"name\":\"one\"},{\"id\":2,\"name\":\"two\"}],\"status\":null}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    cJSON *first_name = cJSON_GetObjectItem(first_item, "name");
    cJSON *second_name = cJSON_GetObjectItem(second_item, "name");
    cJSON_bool first_invalid = cJSON_IsInvalid(first_item);

    // step 3: Operate
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddItemToObject(summary, "first_name", cJSON_CreateString(cJSON_GetStringValue(first_name)));
    cJSON_AddItemToObject(summary, "second_name", cJSON_CreateString(cJSON_GetStringValue(second_name)));
    cJSON_AddItemToObject(summary, "first_was_invalid", cJSON_CreateBool(first_invalid));
    char *pretty = cJSON_Print(summary);
    char *prealloc = (char *)cJSON_malloc(512);
    memset(prealloc, 0, 512);
    cJSON_PrintPreallocated(summary, prealloc, 512, 1);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(prealloc);
    cJSON_Delete(summary);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}