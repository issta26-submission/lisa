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
//<ID> 1501
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
    cJSON *detached = NULL;
    cJSON *num_item = NULL;
    cJSON *str_item = NULL;
    cJSON_bool equal = 0;
    char json[] = "{\"arr\":[1,2],\"keep\":\"value\"}";
    size_t buffer_length = (size_t)(sizeof(json) - 1);

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLength(json, buffer_length);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.0));
    str_item = cJSON_CreateString("temp");
    cJSON_AddItemToObject(root, "keep", str_item);

    // step 3: Operate / Validate
    detached = cJSON_DetachItemFromArray(arr, 1);
    num_item = cJSON_CreateNumber(2.0);
    equal = cJSON_Compare(detached, num_item, 1);
    cJSON_Delete(num_item);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "keep");
    (void)equal;

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}