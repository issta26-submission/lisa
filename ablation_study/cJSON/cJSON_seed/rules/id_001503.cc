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
//<ID> 1503
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *greet = NULL;
    cJSON *detached = NULL;
    cJSON *dup_greet = NULL;
    const char *dup_str = NULL;
    cJSON_bool cmp_res = 0;
    int arr_size = 0;
    char json[] = "{ \"msg\": \"hello\", \"numbers\": [1, 2, 3] }";
    size_t json_len = sizeof(json) - 1;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLength(json, json_len);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(3.0));
    greet = cJSON_CreateString("hello-world");
    cJSON_AddItemToObject(root, "greet", greet);

    // step 3: Operate / Validate
    detached = cJSON_DetachItemFromArray(arr, 0);
    cJSON_AddItemToObject(root, "moved", detached);
    dup_greet = cJSON_Duplicate(greet, 1);
    cmp_res = cJSON_Compare(greet, dup_greet, 1);
    dup_str = cJSON_GetStringValue(dup_greet);
    arr_size = cJSON_GetArraySize(arr);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "greet");
    (void)cmp_res;
    (void)dup_str;
    (void)arr_size;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup_greet);
    // API sequence test completed successfully
    return 66;
}