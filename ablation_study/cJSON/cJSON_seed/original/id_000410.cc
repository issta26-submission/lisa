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
//<ID> 410
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw = "{\"arr\":[1,2,3],\"obj\":{\"a\":5}}";
    const char *parse_end = NULL;
    cJSON_bool require_null = 1;
    cJSON *root = cJSON_ParseWithLengthOpts(raw, 29, &parse_end, require_null);

    // step 2: Configure
    cJSON *arr = cJSON_GetObjectItemCaseSensitive(root, "arr");
    cJSON *second_elem_before = cJSON_GetArrayItem(arr, 1);
    cJSON *newobj = cJSON_CreateObject();
    cJSON *obj_val = cJSON_CreateNumber(7.75);
    cJSON_AddItemToObject(newobj, "n", obj_val);
    cJSON_AddItemToObject(root, "newobj", newobj);

    // step 3: Operate
    cJSON *to_insert = cJSON_CreateNumber(99.5);
    cJSON_InsertItemInArray(arr, 1, to_insert);
    cJSON *inserted = cJSON_GetArrayItem(arr, 1);
    double inserted_val = cJSON_GetNumberValue(inserted);
    cJSON *scaled = cJSON_CreateNumber(inserted_val * 2.0);
    cJSON_AddItemToObject(root, "scaled", scaled);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate and Cleanup
    double original_second = cJSON_GetNumberValue(second_elem_before);
    cJSON *orig_copy = cJSON_CreateNumber(original_second);
    cJSON_AddItemToObject(root, "original_second", orig_copy);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}