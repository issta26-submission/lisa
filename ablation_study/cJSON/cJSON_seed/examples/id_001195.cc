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
//<ID> 1195
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"arr\":[false,123,\"x\"],\"val\":true}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *created_arr = cJSON_CreateArray();

    // step 2: Configure
    cJSON *item0 = cJSON_CreateFalse();
    cJSON *item1 = cJSON_CreateNumber(10.0);
    cJSON *item2 = cJSON_CreateString("hello");
    cJSON_AddItemToArray(created_arr, item0);
    cJSON_AddItemToArray(created_arr, item1);
    cJSON_AddItemToArray(created_arr, item2);
    cJSON_AddItemToObject(root, "created", created_arr);
    cJSON *explicit_true = cJSON_CreateTrue();

    // step 3: Operate & Validate
    cJSON_bool replaced = cJSON_ReplaceItemInArray(created_arr, 1, explicit_true);
    cJSON *parsed_arr = cJSON_GetObjectItem(parsed, "arr");
    cJSON *parsed_first = cJSON_GetArrayItem(parsed_arr, 0);
    cJSON *created_first = cJSON_GetArrayItem(created_arr, 0);
    cJSON_bool cmp_first = cJSON_Compare(created_first, parsed_first, 1);
    cJSON *parsed_val = cJSON_GetObjectItem(parsed, "val");
    cJSON *created_second = cJSON_GetArrayItem(created_arr, 1);
    cJSON_bool cmp_second = cJSON_Compare(created_second, parsed_val, 1);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    (void)replaced;
    (void)cmp_first;
    (void)cmp_second;
    return 66;
}