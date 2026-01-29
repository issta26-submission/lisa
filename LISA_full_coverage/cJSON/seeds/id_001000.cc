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
//<ID> 1000
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"numbers\": [1, 2.5, \"three\"], \"flags\": [false, true], \"meta\": {\"valid\": false}}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *numbers = cJSON_GetObjectItem(root, "numbers");
    cJSON *second = cJSON_GetArrayItem(numbers, 1);
    cJSON_bool second_is_num = cJSON_IsNumber(second);
    cJSON_AddBoolToObject(root, "second_is_number", second_is_num);
    cJSON_AddTrueToObject(root, "always_true");
    cJSON *third = cJSON_GetArrayItem(numbers, 2);
    cJSON_bool third_invalid = cJSON_IsInvalid(third);
    cJSON_AddBoolToObject(root, "third_is_invalid", third_invalid);
    cJSON *summary = cJSON_AddObjectToObject(root, "summary");
    int count = cJSON_GetArraySize(numbers);
    cJSON_AddNumberToObject(summary, "count", count);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(512);
    memset(print_buf, 0, 512);
    cJSON_PrintPreallocated(root, print_buf, 512, 0);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "summary");
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}