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
//<ID> 1505
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *duplicate = NULL;
    cJSON *arr = NULL;
    cJSON *detached = NULL;
    cJSON *child = NULL;
    cJSON_bool cmp = 0;
    char json[] = "{\"alpha\":123, \"beta\": [\"one\", 2, true]}";
    size_t json_len = sizeof(json) - 1;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLength(json, json_len);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(arr, cJSON_CreateString("second"));
    cJSON_AddItemToArray(arr, cJSON_CreateBool(1));
    child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddStringToObject(child, "inner", "value");

    // step 3: Operate / Validate
    duplicate = cJSON_Duplicate(parsed, 1);
    cmp = cJSON_Compare(parsed, duplicate, 1);
    detached = cJSON_DetachItemFromArray(arr, 1);
    cJSON_AddItemToObject(root, "reused", detached);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "child");
    (void)cmp;

    // step 4: Cleanup
    cJSON_Delete(duplicate);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}