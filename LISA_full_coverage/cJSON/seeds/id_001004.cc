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
//<ID> 1004
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"numbers\": [10, 20, 30], \"meta\": {\"active\": false}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(initial_json, &parse_end, 1);

    // step 2: Configure
    cJSON *numbers = cJSON_GetObjectItem(root, "numbers");
    cJSON *first_elem = cJSON_GetArrayItem(numbers, 0);
    cJSON_bool first_is_number = cJSON_IsNumber(first_elem);
    cJSON_AddBoolToObject(root, "first_is_number", first_is_number);
    cJSON_AddTrueToObject(root, "always_true");
    cJSON *maybe_null = NULL;
    cJSON_bool null_is_invalid = cJSON_IsInvalid(maybe_null);
    cJSON_AddBoolToObject(root, "null_is_invalid", null_is_invalid);

    // step 3: Operate
    char *out_unformatted = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(512);
    memset(print_buf, 0, 512);
    cJSON_PrintPreallocated(root, print_buf, 512, 0);

    // step 4: Validate & Cleanup
    cJSON_free(out_unformatted);
    cJSON_free(print_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}