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
//<ID> 1461
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version_str = cJSON_Version();
    const char *input_json = "{\"num\": 3.14, \"name\": \"original\"}";
    cJSON *root = cJSON_Parse(input_json);
    cJSON *injected = cJSON_CreateString("injected");
    cJSON *version_item = cJSON_CreateString(version_str);

    // step 2: Configure
    cJSON_AddItemToObject(root, "added", injected);
    cJSON_AddItemToObject(root, "version", version_item);

    // step 3: Operate & Validate
    cJSON *num_item = cJSON_GetObjectItem(root, "num");
    cJSON_bool num_is_num = cJSON_IsNumber(num_item);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *printed = cJSON_PrintUnformatted(root);
    (void)num_is_num;
    (void)name_item;
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}