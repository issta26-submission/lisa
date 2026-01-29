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
//<ID> 231
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *meta_raw = cJSON_CreateRaw("{\"embedded\":true}");

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *num2 = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToArray(values, num1);
    cJSON_AddItemToArray(values, num2);
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddItemToObject(data, "values", values);
    cJSON_AddItemToObject(data, "label", cJSON_CreateString("unit-test"));
    cJSON_AddItemToObject(root, "meta", meta_raw);

    // step 3: Operate & Validate
    cJSON *got_data = cJSON_GetObjectItem(root, "data");
    cJSON *got_values = cJSON_GetObjectItem(got_data, "values");
    int values_count = cJSON_GetArraySize(got_values);
    cJSON *values_copy = cJSON_Duplicate(got_values, 1);
    cJSON_AddItemToObject(root, "values_copy", values_copy);
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON_bool meta_is_raw = cJSON_IsRaw(got_meta);
    cJSON_AddItemToObject(root, "values_count", cJSON_CreateNumber((double)values_count));
    cJSON_AddItemToObject(root, "meta_is_raw", cJSON_CreateNumber((double)meta_is_raw));
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}