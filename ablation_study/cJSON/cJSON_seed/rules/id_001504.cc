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
//<ID> 1504
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
    cJSON *arr = NULL;
    cJSON *num_item = NULL;
    cJSON *str_item = NULL;
    cJSON *detached = NULL;
    cJSON *dup = NULL;
    cJSON_bool equal = 0;
    char json[] = " { \"a\": 123, \"flag\": true } ";
    size_t len = sizeof(json) - 1;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLength(json, len);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num_item);
    str_item = cJSON_CreateString("elem");
    cJSON_AddItemToArray(arr, str_item);

    // step 3: Operate / Validate
    detached = cJSON_DetachItemFromArray(arr, 1);
    dup = cJSON_Duplicate(detached, 1);
    equal = cJSON_Compare(detached, dup, 1);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "imported");
    (void)equal;

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}