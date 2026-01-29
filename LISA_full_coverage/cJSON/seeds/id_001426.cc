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
//<ID> 1426
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"items\": [10, 20, 30], \"name\": \"example\"}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *second_elem = cJSON_GetArrayItem(items, 1);
    cJSON_bool was_invalid = cJSON_IsInvalid(second_elem);
    (void)was_invalid;

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemInArray(items, 1, replacement);

    // step 3: Operate & Validate
    cJSON *new_second = cJSON_GetArrayItem(items, 1);
    double new_value = cJSON_GetNumberValue(new_second);
    (void)new_value;
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}