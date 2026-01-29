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
//<ID> 632
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *container = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "container", container);
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(container, "initial", num);
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    double prev_set = cJSON_SetNumberHelper(num, 84.0);
    cJSON_AddNumberToObject(root, "previous_set_value", prev_set);
    const char *version_str = cJSON_Version();
    cJSON *ver_item = cJSON_CreateString(version_str);
    cJSON_AddItemToObject(root, "version", ver_item);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 3: Operate and Validate
    cJSON *existing = cJSON_GetObjectItem(container, "initial");
    cJSON *replacement = cJSON_CreateNumber(21.0);
    cJSON_ReplaceItemViaPointer(container, existing, replacement);
    double rep_val = cJSON_GetNumberValue(replacement);
    double updated = cJSON_SetNumberHelper(replacement, rep_val * 2.0);
    cJSON_AddNumberToObject(root, "replacement_updated", updated);
    cJSON *flag_ptr = cJSON_GetObjectItem(root, "flag");
    cJSON *new_flag = cJSON_CreateBool(0);
    cJSON_ReplaceItemViaPointer(root, flag_ptr, new_flag);
    cJSON *ver_retrieved = cJSON_GetObjectItem(root, "version");
    const char *ver_text = cJSON_GetStringValue(ver_retrieved);
    cJSON_AddStringToObject(root, "reported_version", ver_text);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}