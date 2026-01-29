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
//<ID> 697
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *strings[] = { "alpha", "beta", "gamma" };
    cJSON *root = cJSON_CreateObject();
    cJSON *str_array = cJSON_CreateStringArray(strings, 3);
    cJSON_AddItemToObject(root, "names", str_array);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", maybe_null);
    cJSON *flag_bool = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    cJSON_bool is_array = cJSON_IsArray(str_array);
    cJSON_bool is_raw = cJSON_IsRaw(maybe_null);
    double meta_value = (double)(is_array + is_raw);
    cJSON_AddNumberToObject(root, "meta_value", meta_value);
    cJSON *root_copy = cJSON_Duplicate(root, 1);

    // step 3: Operate
    char *out_unformatted = cJSON_PrintUnformatted(root_copy);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root_copy, buffer, (int)sizeof(buffer), 0);
    cJSON_free(out_unformatted);

    // step 4: Validate & Cleanup
    cJSON_Delete(root_copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}