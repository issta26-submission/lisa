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
//<ID> 1641
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *active_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", active_true);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "name", name);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("temp");
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "name", replacement);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *set_result = cJSON_SetValuestring(name_item, "final-value");

    // step 3: Operate and Validate
    const char *name_val = cJSON_GetStringValue(name_item);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[8];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = name_val ? name_val[0] : 'X';
    buffer[1] = (char)('0' + (int)replaced);
    buffer[2] = set_result ? set_result[0] : 'Y';
    buffer[3] = printed ? printed[0] : 'Z';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}