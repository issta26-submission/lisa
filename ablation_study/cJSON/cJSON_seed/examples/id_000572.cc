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
//<ID> 572
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *parent_array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "parent", parent_array);

    // step 2: Configure
    const char *strings[] = { "one", "two", "three" };
    cJSON *str_array = cJSON_CreateStringArray(strings, 3);
    cJSON_AddItemToArray(parent_array, str_array);
    cJSON *flag = cJSON_AddFalseToObject(root, "flag");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_parent = cJSON_GetObjectItem(parsed, "parent");
    cJSON *parsed_first = cJSON_GetArrayItem(parsed_parent, 0);
    cJSON *parsed_flag = cJSON_GetObjectItem(parsed, "flag");
    cJSON_bool flag_is_false = cJSON_IsFalse(parsed_flag);
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;flag_false=%d;first_str=%s",
            version ? version : "null",
            (int)flag_is_false,
            cJSON_GetStringValue(cJSON_GetArrayItem(parsed_first, 0)));

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}