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
//<ID> 19
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"a\": 42, \"b\": \"hello\"}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);

    // step 2: Configure
    cJSON *root_dup = cJSON_Duplicate(root, 1);
    cJSON_AddTrueToObject(root_dup, "is_valid");
    cJSON_AddFalseToObject(root_dup, "is_temp");

    // step 3: Operate and Validate
    cJSON *item_a = cJSON_GetObjectItem(root_dup, "a");
    double val_a = cJSON_GetNumberValue(item_a);
    cJSON *item_b = cJSON_GetObjectItem(root_dup, "b");
    char *str_b = cJSON_GetStringValue(item_b);
    cJSON_bool same_structure = cJSON_Compare(root, root_dup, 0);

    // step 4: Cleanup
    (void)val_a;
    (void)str_b;
    (void)same_structure;
    cJSON_Delete(root_dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}