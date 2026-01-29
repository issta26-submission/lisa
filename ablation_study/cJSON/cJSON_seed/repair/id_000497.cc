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
//<ID> 497
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"alpha\":10,\"beta\":\"two\"}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "original", parsed);
    cJSON *sub = cJSON_AddObjectToObject(root, "sub");

    // step 2: Configure
    cJSON *alpha_item = cJSON_GetObjectItem(parsed, "alpha");
    double alpha_val = cJSON_GetNumberValue(alpha_item);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(alpha_val));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(alpha_val * 2.0));
    cJSON_AddItemToArray(arr, cJSON_CreateString("computed"));
    cJSON_AddItemToObject(sub, "numbers", arr);
    cJSON_AddItemToObject(sub, "computed", cJSON_CreateNumber(alpha_val + 5.0));
    cJSON_AddItemToObject(root, "status", cJSON_CreateString("ok"));

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Validate and Cleanup
    int size = cJSON_GetArraySize(arr);
    cJSON *third = cJSON_GetArrayItem(arr, 2);
    char *third_s = cJSON_GetStringValue(third);
    (void)third_s;
    (void)size;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}