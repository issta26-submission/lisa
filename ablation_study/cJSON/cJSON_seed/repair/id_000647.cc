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
//<ID> 647
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
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name_item = cJSON_CreateString("fuzz-test");
    cJSON_AddItemToObject(meta, "name", name_item);
    cJSON_AddStringToObject(meta, "category", "demo");
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag);

    // step 2: Configure
    const char *name_val = cJSON_GetStringValue(name_item);
    cJSON *copied_name = cJSON_AddStringToObject(root, "name_copy", name_val);
    cJSON_bool flag_is_false = cJSON_IsFalse(flag);
    char flag_buf[16];
    memset(flag_buf, 0, sizeof(flag_buf));
    sprintf(flag_buf, "%d", (int)flag_is_false);
    cJSON_AddStringToObject(root, "enabled_as_int", flag_buf);

    // step 3: Operate and Validate
    const char *copied_val = cJSON_GetStringValue(copied_name);
    cJSON_AddStringToObject(root, "copied_snapshot", copied_val);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}