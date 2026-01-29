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
//<ID> 792
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char raw_payload[] = "{\"embedded\":1}";
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *num_item = cJSON_CreateNumber(3.14159);
    cJSON *str_item = cJSON_CreateString("example");
    cJSON *raw_item = cJSON_CreateRaw(raw_payload);
    cJSON *null_item = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToArray(array, num_item);
    cJSON_AddItemToArray(array, str_item);
    cJSON_AddItemToArray(array, raw_item);
    cJSON_AddItemToArray(array, null_item);
    cJSON_AddItemToObject(root, "items", array);
    cJSON_bool raw_flag = cJSON_IsRaw(raw_item);
    cJSON_bool null_flag = cJSON_IsNull(null_item);
    cJSON_AddNumberToObject(root, "raw_flag", (double)raw_flag);
    cJSON_AddNumberToObject(root, "null_flag", (double)null_flag);

    // step 3: Operate
    cJSON *dup_array = cJSON_Duplicate(array, 1);
    cJSON_bool arrays_equal = cJSON_Compare(array, dup_array, 1);
    cJSON_AddNumberToObject(root, "arrays_equal", (double)arrays_equal);
    cJSON *third = cJSON_GetArrayItem(array, 2);
    cJSON_bool third_is_raw = cJSON_IsRaw(third);
    cJSON_AddNumberToObject(root, "third_is_raw", (double)third_is_raw);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate & Cleanup
    cJSON_Delete(dup_array);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}