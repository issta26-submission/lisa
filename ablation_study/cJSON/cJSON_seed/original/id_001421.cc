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
//<ID> 1421
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"numbers\": [1, 2, 3], \"name\": \"test\"}";
    cJSON *root = cJSON_Parse(json_text);
    // step 2: Configure
    cJSON *numbers = cJSON_GetObjectItem(root, "numbers");
    cJSON *orig_second = cJSON_GetArrayItem(numbers, 1);
    cJSON_bool orig_invalid = cJSON_IsInvalid(orig_second);
    cJSON *replacement = cJSON_CreateNumber(99.5);
    cJSON_ReplaceItemInArray(numbers, 1, replacement);
    // step 3: Operate & Validate
    cJSON *got_second = cJSON_GetArrayItem(numbers, 1);
    double got_val = cJSON_GetNumberValue(got_second);
    cJSON *append_num = cJSON_CreateNumber(got_val + 1.0);
    cJSON_AddItemToArray(numbers, append_num);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    (void)orig_invalid;
    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}