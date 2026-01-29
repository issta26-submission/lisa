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
//<ID> 2436
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(10.5);
    cJSON *label = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "value", num);
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromObject(root, "label");
    cJSON *val_item = cJSON_GetObjectItem(root, "value");
    cJSON *new_num = cJSON_CreateNumber(99.0);
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, val_item, new_num);
    cJSON_AddItemToObjectCS(root, "constant", cJSON_CreateString("csval"));
    char *label_val = cJSON_GetStringValue(detached);
    buffer[1] = label_val && label_val[0] ? label_val[0] : '\0';
    cJSON *cs_item = cJSON_GetObjectItem(root, "constant");
    char *cs_val = cJSON_GetStringValue(cs_item);
    buffer[2] = cs_val && cs_val[0] ? cs_val[0] : '\0';
    buffer[4] = (char)(int)replaced;

    // step 4: Validate and Cleanup
    char *printed2 = cJSON_PrintUnformatted(root);
    buffer[3] = printed2 && printed2[0] ? printed2[0] : '\0';
    cJSON_free(printed);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}