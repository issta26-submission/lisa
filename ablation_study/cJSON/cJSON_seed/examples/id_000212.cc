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
//<ID> 212
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *strings[] = { "alpha", "beta", "gamma" };
    double numbers[] = { 3.14, 2.718, 1.618 };
    cJSON *str_array = cJSON_CreateStringArray(strings, 3);
    cJSON *dbl_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObjectCS(root, "names", str_array);
    cJSON_AddItemToObjectCS(root, "constants", dbl_array);

    // step 2: Configure
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(root, "optional", maybe_null);
    cJSON_bool is_null_flag = cJSON_IsNull(maybe_null);
    cJSON *null_flag_num = cJSON_CreateNumber((double)is_null_flag);
    cJSON_AddItemToObjectCS(root, "is_optional_null", null_flag_num);
    cJSON *meta = cJSON_CreateObject();
    cJSON *version_num = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObjectCS(meta, "version", version_num);
    cJSON_AddItemToObjectCS(root, "meta", meta);

    // step 3: Operate & Validate
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}