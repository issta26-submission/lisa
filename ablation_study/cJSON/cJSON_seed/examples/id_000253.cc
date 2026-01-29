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
//<ID> 253
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON *n1 = cJSON_CreateNumber(7.0);
    cJSON *n2 = cJSON_CreateNumber(14.0);
    const char *tags_values[] = { "alpha", "beta", "gamma" };
    cJSON *tags = cJSON_CreateStringArray(tags_values, 3);

    // step 2: Configure
    cJSON_AddItemToObjectCS(settings, "n1", n1);
    cJSON_AddItemToObject(settings, "n2", n2);
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON_AddItemToObjectCS(root, "tags", tags);

    // step 3: Operate & Validate
    cJSON *replacement = cJSON_CreateNumber(21.0);
    cJSON_ReplaceItemViaPointer(settings, n1, replacement);
    cJSON *n1_ptr = cJSON_GetObjectItem(settings, "n1");
    double n1_value = cJSON_GetNumberValue(n1_ptr);
    cJSON *n1_copy = cJSON_CreateNumber(n1_value);
    cJSON_AddItemToObject(root, "n1_value_copy", n1_copy);
    cJSON_bool has_tags = cJSON_HasObjectItem(root, "tags");
    cJSON *has_tags_num = cJSON_CreateNumber((double)has_tags);
    cJSON_AddItemToObjectCS(root, "has_tags", has_tags_num);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}