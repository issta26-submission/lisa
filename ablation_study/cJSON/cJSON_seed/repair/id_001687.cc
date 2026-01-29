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
//<ID> 1687
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("tester");
    cJSON *active_item = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_bool added_name = cJSON_AddItemToObject(root, "name", name_item);
    cJSON_bool added_active = cJSON_AddItemToObject(root, "active", active_item);

    // step 3: Operate and Validate
    cJSON *retrieved_name = cJSON_GetObjectItem(root, "name");
    cJSON *retrieved_active = cJSON_GetObjectItem(root, "active");
    cJSON_bool name_invalid = cJSON_IsInvalid(retrieved_name);
    cJSON_bool active_is_true = cJSON_IsTrue(retrieved_active);
    char *printed = cJSON_Print(root);
    char *name_str = cJSON_GetStringValue(retrieved_name);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_str ? name_str[0] : 'Y';
    buffer[2] = (char)('0' + (int)name_invalid);
    buffer[3] = (char)('0' + (int)active_is_true);
    (void)added_name;
    (void)added_active;
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}