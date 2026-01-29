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
//<ID> 366
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_AddObjectToObject(root, "data");
    cJSON_AddNullToObject(data, "optional");
    cJSON *values = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(3.0);
    cJSON *n2 = cJSON_CreateNumber(4.5);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToObject(data, "values", values);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "label", "sensorA");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *flags = cJSON_CreateArray();
    cJSON_AddItemToArray(flags, cJSON_CreateTrue());
    cJSON_AddItemToObject(root, "flags", flags);

    // step 3: Operate and Validate
    cJSON *got_data = cJSON_GetObjectItemCaseSensitive(root, "data");
    cJSON *got_values = cJSON_GetObjectItemCaseSensitive(got_data, "values");
    cJSON_bool is_arr = cJSON_IsArray(got_values);
    cJSON *first = cJSON_GetArrayItem(got_values, 0);
    double first_val = cJSON_GetNumberValue(first);
    double computed = first_val + (double)is_arr * 2.5;
    cJSON *result = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "result", result);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}