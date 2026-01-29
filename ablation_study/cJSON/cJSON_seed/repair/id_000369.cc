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
//<ID> 369
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *container = cJSON_AddObjectToObject(root, "container");
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(container, "values", values);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);

    // step 2: Configure
    cJSON *inner = cJSON_AddObjectToObject(container, "inner");
    cJSON_AddNullToObject(inner, "optional");
    cJSON *label = cJSON_CreateString("sample");
    cJSON_AddItemToObject(inner, "label", label);

    // step 3: Operate and Validate
    cJSON *got_values = cJSON_GetObjectItemCaseSensitive(container, "values");
    cJSON_bool is_array = cJSON_IsArray(got_values);
    cJSON *first_item = cJSON_GetArrayItem(got_values, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    double computed_val = first_val + (double)is_array * 3.5;
    cJSON *computed = cJSON_CreateNumber(computed_val);
    cJSON_AddItemToObject(root, "computed", computed);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}