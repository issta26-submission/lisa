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
//<ID> 217
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *nullable = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(root, "nullable", nullable);

    // step 2: Configure
    double numbers[] = {3.14, -2.71, 0.0};
    cJSON *double_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObjectCS(root, "double_values", double_array);
    const char *const strings[] = {"alpha", "beta", "gamma"};
    cJSON *string_array = cJSON_CreateStringArray(strings, 3);
    cJSON_AddItemToObjectCS(root, "string_values", string_array);
    cJSON *fixed_number = cJSON_CreateNumber(123.456);
    cJSON_AddItemToObjectCS(root, "fixed_number", fixed_number);

    // step 3: Operate & Validate
    cJSON_bool was_null = cJSON_IsNull(nullable);
    cJSON *null_indicator = cJSON_CreateNumber((double)was_null);
    cJSON_AddItemToObjectCS(root, "nullable_indicator", null_indicator);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}