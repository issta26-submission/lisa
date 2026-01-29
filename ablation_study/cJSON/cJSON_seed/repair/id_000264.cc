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
//<ID> 264
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"a\": 3.5, \"b\": 2}";
    size_t json_len = (size_t)(sizeof("{\"a\": 3.5, \"b\": 2}") - 1);
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *item_a = cJSON_GetObjectItem(root, "a");
    double a_val = cJSON_GetNumberValue(item_a);
    cJSON *double_a = cJSON_CreateNumber(a_val * 2.0);
    cJSON_AddItemToObject(root, "double_a", double_a);

    // step 3: Operate and Validate
    cJSON *item_b = cJSON_GetObjectItem(root, "b");
    double b_val = cJSON_GetNumberValue(item_b);
    cJSON *replacement_b = cJSON_CreateNumber(b_val + a_val);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "b", replacement_b);
    char *out = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}