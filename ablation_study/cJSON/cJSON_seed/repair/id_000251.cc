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
//<ID> 251
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *it1 = cJSON_CreateString("alpha");
    cJSON *it2 = cJSON_CreateString("beta");
    cJSON *it3 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(items, it1);
    cJSON_AddItemToArray(items, it2);
    cJSON_AddItemToArray(items, it3);

    // step 3: Operate and Validate
    cJSON *second = cJSON_GetArrayItem(items, 1);
    const char *second_text = cJSON_GetStringValue(second);
    cJSON *second_dup = cJSON_CreateString(second_text);
    cJSON_AddItemToObjectCS(root, "second_dup", second_dup);
    cJSON_bool second_is_str = cJSON_IsString(second);
    cJSON_AddBoolToObject(root, "second_is_string", second_is_str);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}