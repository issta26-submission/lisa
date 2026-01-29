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
//<ID> 2431
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(1.5);
    cJSON *label = cJSON_CreateString("hello");
    cJSON_AddItemToObjectCS(root, "num", num);
    cJSON_AddItemToObjectCS(root, "label", label);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';

    // step 3: Operate
    cJSON *label_item = cJSON_GetObjectItem(root, "label");
    const char *label_before = cJSON_GetStringValue(label_item);
    cJSON *detached_num = cJSON_DetachItemFromObject(root, "num");
    cJSON *replacement_label = cJSON_CreateString("world");
    cJSON_ReplaceItemViaPointer(root, label_item, replacement_label);
    cJSON_AddItemToObjectCS(root, "restored", detached_num);
    buffer[1] = label_before && label_before[0] ? label_before[0] : '\0';
    const char *label_after = cJSON_GetStringValue(cJSON_GetObjectItem(root, "label"));
    buffer[2] = label_after && label_after[0] ? label_after[0] : '\0';

    // step 4: Validate and Cleanup
    char *printed2 = cJSON_PrintUnformatted(root);
    buffer[3] = printed2 && printed2[0] ? printed2[0] : '\0';
    cJSON_free(printed);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}