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
//<ID> 1153
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"alpha\",\"enabled\":false}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *created_str = cJSON_CreateString("alpha");
    cJSON *created_flag = cJSON_CreateBool(0);

    // step 2: Configure
    cJSON_AddItemToObject(root, "name", created_str);
    cJSON_AddItemToObject(root, "enabled", created_flag);
    cJSON *dup_parsed = cJSON_Duplicate(parsed, 1);

    // step 3: Operate & Validate
    cJSON *parsed_name = cJSON_GetObjectItem(parsed, "name");
    const char *name_value = cJSON_GetStringValue(parsed_name);
    cJSON *copied_name = cJSON_CreateString(name_value);
    cJSON_AddItemToObject(root, "copied_name", copied_name);
    cJSON_bool equal_dup = cJSON_Compare(parsed, dup_parsed, 1);
    cJSON *cmp_flag = cJSON_CreateBool(equal_dup);
    cJSON_AddItemToObject(root, "parsed_equals_dup", cmp_flag);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    cJSON_Delete(dup_parsed);
    // API sequence test completed successfully
    return 66;
}