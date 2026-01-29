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
//<ID> 1677
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_AddObjectToObject(root, "child");
    cJSON *name_str = cJSON_CreateString("example");
    cJSON_AddItemToObject(child, "name", name_str);
    cJSON *num_item = cJSON_AddNumberToObject(child, "value", 3.14159);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    cJSON *note = cJSON_CreateString("initialized");
    cJSON_AddItemToObject(root, "note", note);
    cJSON_bool flag_is_false = cJSON_IsFalse(flag);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)flag_is_false);
    buffer[2] = num_item && num_item->valuestring ? num_item->valuestring[0] : (printed ? printed[1] : 'Y');
    (void)buffer;
    (void)num_item;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}