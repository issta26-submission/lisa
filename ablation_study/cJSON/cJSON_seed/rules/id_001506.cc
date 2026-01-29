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
//<ID> 1506
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num = NULL;
    cJSON *str = NULL;
    cJSON *detached = NULL;
    cJSON *parsed = NULL;
    cJSON *cmp_item = NULL;
    cJSON_bool equal = 0;
    int arr_size = 0;
    char json[] = "{ \"x\": 123, \"y\": \"z\" }";
    size_t json_len = (size_t)(sizeof(json) - 1);

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    num = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(arr, num);
    str = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str);
    parsed = cJSON_ParseWithLength(json, json_len);
    cJSON_AddItemToObject(root, "imported", parsed);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    detached = cJSON_DetachItemFromArray(arr, 1);
    cmp_item = cJSON_CreateString("hello");
    equal = cJSON_Compare(detached, cmp_item, 1);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "imported");
    (void)arr_size;
    (void)equal;

    // step 4: Cleanup
    cJSON_Delete(cmp_item);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}