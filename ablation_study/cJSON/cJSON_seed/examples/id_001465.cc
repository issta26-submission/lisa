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
//<ID> 1465
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    const char *json_text = "{\"num\":123.45,\"str\":\"hello\"}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *added_str = cJSON_CreateString("added_field");
    cJSON *version_str = cJSON_CreateString(version);

    // step 2: Configure
    cJSON_AddItemToObject(root, "added", added_str);
    cJSON_AddItemToObject(root, "version", version_str);

    // step 3: Operate & Validate
    cJSON *num_item = cJSON_GetObjectItem(root, "num");
    cJSON_bool num_is_num = cJSON_IsNumber(num_item);
    double num_value = cJSON_GetNumberValue(num_item);
    cJSON *orig_str_item = cJSON_GetObjectItem(root, "str");
    const char *orig_str = cJSON_GetStringValue(orig_str_item);
    char *printed = cJSON_PrintUnformatted(root);
    (void)num_is_num;
    (void)num_value;
    (void)orig_str;
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}