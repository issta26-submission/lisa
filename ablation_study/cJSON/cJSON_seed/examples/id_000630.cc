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
//<ID> 630
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num_item = cJSON_CreateNumber(1.0);
    cJSON *str_item = cJSON_CreateString("two");
    cJSON *child = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddStringToObject(child, "name", "example");
    cJSON_AddNumberToObject(child, "version_num", 2.0);
    cJSON *dup_child = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "child_copy", dup_child);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    char *pretty = cJSON_Print(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_child = cJSON_GetObjectItem(parsed, "child");
    cJSON *name_item = cJSON_GetObjectItem(parsed_child, "name");
    const char *name = cJSON_GetStringValue(name_item);
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;name=%s", version ? version : "null", name ? name : "null");
    cJSON_DeleteItemFromObject(parsed, "child");

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(pretty);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}