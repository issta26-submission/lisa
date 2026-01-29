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
//<ID> 1643
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "name", name);

    // step 2: Configure
    char *sv = cJSON_SetValuestring(name, "updated");
    cJSON *replacement_flag = cJSON_CreateTrue();
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "flag", replacement_flag);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *val = cJSON_GetStringValue(name);
    char buffer[16];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = val ? val[0] : 'X';
    buffer[1] = (char)('0' + (int)replaced);
    buffer[2] = printed ? printed[0] : 'Y';
    buffer[3] = sv ? sv[0] : 'Z';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}