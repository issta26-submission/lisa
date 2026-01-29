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
//<ID> 1319
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"arr\": [true, 123.0, \"three\"]}";
    const char *parse_end = NULL;
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *addedArr = NULL;
    cJSON *arr_dup = NULL;
    cJSON *raw_item = NULL;
    cJSON *dup = NULL;
    cJSON_bool isRaw = 0;
    cJSON_bool eq = 0;

    // step 2: Setup / Configure
    root = cJSON_ParseWithLengthOpts(json, (size_t)(sizeof(json) - 1), &parse_end, 1);
    arr = cJSON_GetObjectItem(root, "arr");
    addedArr = cJSON_AddArrayToObject(root, "addedArray");
    arr_dup = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToArray(addedArr, arr_dup);
    raw_item = cJSON_CreateRaw("raw_payload");
    cJSON_AddItemToArray(addedArr, raw_item);

    // step 3: Operate / Validate
    isRaw = cJSON_IsRaw(raw_item);
    dup = cJSON_Duplicate(addedArr, 1);
    eq = cJSON_Compare(addedArr, dup, 1);
    (void)isRaw;
    (void)eq;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup);
    // API sequence test completed successfully
    return 66;
}