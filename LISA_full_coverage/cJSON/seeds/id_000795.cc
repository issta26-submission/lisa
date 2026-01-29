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
//<ID> 795
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *null_item = cJSON_CreateNull();
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON *num_item = cJSON_CreateNumber(3.1415);
    cJSON *str_item = cJSON_CreateString("example");

    // step 2: Configure
    cJSON_AddItemToArray(array, null_item);
    cJSON_AddItemToArray(array, raw_item);
    cJSON_AddItemToArray(array, num_item);
    cJSON_AddItemToArray(array, str_item);
    cJSON_AddItemToObject(root, "data", array);
    cJSON_AddNumberToObject(root, "count", (double)cJSON_GetArraySize(array));

    // step 3: Operate
    cJSON_bool was_null = cJSON_IsNull(null_item);
    cJSON_bool was_raw = cJSON_IsRaw(raw_item);
    cJSON *dup_raw = cJSON_Duplicate(raw_item, 1);
    cJSON_bool raw_eq_dup = cJSON_Compare(raw_item, dup_raw, 1);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_Minify(out);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(dup_raw);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}