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
//<ID> 1189
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"arr\":[1,false,null],\"flag\":true}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *created_arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.5);
    cJSON *n2 = cJSON_CreateFalse();
    cJSON *n3 = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToArray(created_arr, n1);
    cJSON_AddItemToArray(created_arr, n2);
    cJSON_AddItemToArray(created_arr, n3);
    cJSON_AddItemToObject(root, "created", created_arr);
    cJSON_AddBoolToObject(root, "active", 0);
    cJSON_AddNullToObject(root, "missing");

    // step 3: Operate & Validate
    cJSON *parsed_arr = cJSON_GetObjectItem(parsed, "arr");
    cJSON *parsed_item1 = cJSON_GetArrayItem(parsed_arr, 1);
    cJSON *dup_item = cJSON_Duplicate(parsed_item1, 1);
    cJSON *target = cJSON_GetArrayItem(created_arr, 1);
    cJSON_ReplaceItemViaPointer(created_arr, target, dup_item);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}