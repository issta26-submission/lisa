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
//<ID> 1507
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
    cJSON *array = NULL;
    cJSON *moved = NULL;
    cJSON *dup = NULL;
    cJSON *name_item = NULL;
    cJSON_bool compare_result = 0;
    char json[] = " { \"arr\": [1, 2, 3], \"obj\": { \"k\": \"v\" }, \"name\": \"alpha\" } ";
    size_t buffer_length = (size_t)(sizeof(json) - 1);

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLength(json, buffer_length);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);
    array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "extra", array);
    cJSON_AddItemToArray(array, cJSON_CreateNumber(99.0));
    cJSON_AddItemToArray(array, cJSON_CreateString("extra"));
    moved = cJSON_DetachItemFromArray(array, 0);
    cJSON_AddItemToObject(root, "moved", moved);
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "obj");

    // step 3: Operate / Validate
    dup = cJSON_Duplicate(parsed, 1);
    compare_result = cJSON_Compare(parsed, dup, 1);
    name_item = cJSON_GetObjectItem(parsed, "name");
    (void)name_item;
    (void)compare_result;

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}