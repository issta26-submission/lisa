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
//<ID> 1152
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"alpha\",\"enabled\":false,\"count\":3}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *dup = cJSON_Duplicate(parsed, 1);
    cJSON *root = cJSON_CreateObject();
    cJSON *injected_label = cJSON_CreateString("injected");
    cJSON *injected_flag = cJSON_CreateBool(1);

    // step 2: Configure
    cJSON_AddItemToObject(root, "label", injected_label);
    cJSON_AddItemToObject(root, "injected_flag", injected_flag);
    cJSON_AddItemToObject(root, "parsed", dup);

    // step 3: Operate & Validate
    cJSON *parsed_obj = cJSON_GetObjectItem(root, "parsed");
    cJSON *name_item = cJSON_GetObjectItem(parsed_obj, "name");
    char *name_value = cJSON_GetStringValue(name_item);
    cJSON *name_copy = cJSON_CreateString(name_value);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    cJSON_bool are_equal = cJSON_Compare(dup, parsed, 1);
    cJSON *equal_node = cJSON_CreateBool(are_equal);
    cJSON_AddItemToObject(root, "equal", equal_node);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}