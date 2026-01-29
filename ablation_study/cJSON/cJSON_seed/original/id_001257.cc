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
//<ID> 1257
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(10.5);
    cJSON_AddItemToArray(list, num1);
    cJSON *num2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(list, num2);
    cJSON_AddStringToObject(config, "name", "example");
    cJSON_AddBoolToObject(config, "enabled", 1);

    // step 3: Operate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_name = cJSON_GetObjectItem(got_config, "name");
    const char *name_str = cJSON_GetStringValue(got_name);
    cJSON_bool name_invalid = cJSON_IsInvalid(got_name);
    cJSON_AddBoolToObject(root, "name_invalid", name_invalid);
    cJSON *detached = cJSON_DetachItemViaPointer(list, num1);
    cJSON_AddItemToObject(config, "first", detached);
    char *buf = (char *)cJSON_malloc(512);
    memset(buf, 0, 512);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buf, 512, 1);

    // step 4: Validate & Cleanup
    cJSON *first_item = cJSON_GetObjectItem(config, "first");
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON_SetNumberHelper(first_item, first_val + 5.0);
    cJSON_AddStringToObject(root, "copied_name", name_str);
    cJSON *dup = cJSON_Duplicate(root, 1);
    (void)printed_ok;
    cJSON_free(buf);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}