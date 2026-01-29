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
//<ID> 2430
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *label = cJSON_CreateString("initial");
    cJSON_AddItemToObjectCS(root, "label", label);
    cJSON *value = cJSON_CreateNumber(100.0);
    cJSON_AddItemToObjectCS(root, "value", value);

    // step 2: Configure
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);

    // step 3: Operate
    cJSON *detached_value = cJSON_DetachItemFromObject(root, "value");
    cJSON *new_label = cJSON_CreateString("replaced");
    cJSON *label_ptr = cJSON_GetObjectItem(root, "label");
    cJSON_bool replaced_ok = cJSON_ReplaceItemViaPointer(root, label_ptr, new_label);
    const char *label_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "label"));
    buffer[0] = label_str && label_str[0] ? label_str[0] : '\0';
    buffer[1] = (char)replaced_ok;
    char *printed = cJSON_PrintUnformatted(root);
    buffer[2] = printed && printed[0] ? printed[0] : '\0';

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(detached_value);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}