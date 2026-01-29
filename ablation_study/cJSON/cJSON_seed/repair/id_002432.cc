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
//<ID> 2432
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num1 = cJSON_CreateNumber(1.0);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON *label = cJSON_CreateString("alpha");
    cJSON_AddItemToObjectCS(root, "num1", num1);
    cJSON_AddItemToObjectCS(root, "num2", num2);
    cJSON_AddItemToObjectCS(root, "label", label);

    // step 2: Configure
    char *printed_before = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed_before && printed_before[0] ? printed_before[0] : '\0';

    // step 3: Operate
    cJSON *old_num2 = cJSON_GetObjectItem(root, "num2");
    cJSON *replacement = cJSON_CreateNumber(9.0);
    cJSON_ReplaceItemViaPointer(root, old_num2, replacement);
    cJSON *detached_label = cJSON_DetachItemFromObject(root, "label");
    const char *label_str = cJSON_GetStringValue(detached_label);
    buffer[1] = label_str && label_str[0] ? label_str[0] : '\0';

    // step 4: Validate and Cleanup
    char *printed_after = cJSON_PrintUnformatted(root);
    buffer[2] = printed_after && printed_after[0] ? printed_after[0] : '\0';
    cJSON_free(printed_before);
    cJSON_free(printed_after);
    cJSON_free(buffer);
    cJSON_Delete(detached_label);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}