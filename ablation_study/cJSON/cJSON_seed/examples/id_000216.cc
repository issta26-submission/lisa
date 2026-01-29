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
//<ID> 216
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(root, "nullable", maybe_null);

    // step 2: Configure
    double numbers[3];
    numbers[0] = 1.5;
    numbers[1] = -2.0;
    numbers[2] = 3.1415;
    cJSON *double_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObjectCS(root, "double_array", double_array);
    const char *strings[2];
    strings[0] = "alpha";
    strings[1] = "beta";
    cJSON *string_array = cJSON_CreateStringArray(strings, 2);
    cJSON_AddItemToObjectCS(root, "string_array", string_array);

    // step 3: Operate & Validate
    cJSON_bool null_flag = cJSON_IsNull(maybe_null);
    cJSON *null_flag_number = cJSON_CreateNumber((double)null_flag);
    cJSON_AddItemToObjectCS(root, "null_flag", null_flag_number);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}