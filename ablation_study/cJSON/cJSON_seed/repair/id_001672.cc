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
//<ID> 1672
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON_AddNumberToObject(config, "version", 3.14);
    cJSON *name_str = cJSON_CreateString("MyApp");
    cJSON_AddItemToObject(config, "name", name_str);
    cJSON *enabled_flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", enabled_flag);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "count", 7.0);
    cJSON_AddNumberToObject(config, "max_items", 42.0);

    // step 3: Operate and Validate
    cJSON_bool is_disabled = cJSON_IsFalse(enabled_flag);
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    cJSON *name_item = cJSON_GetObjectItem(config, "name");
    const char *name_val = cJSON_GetStringValue(name_item);
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)is_disabled);
    buffer[2] = name_val ? name_val[0] : 'Y';
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}