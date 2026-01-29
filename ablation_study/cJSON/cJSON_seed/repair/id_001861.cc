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
//<ID> 1861
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(2.0));

    // step 2: Configure
    char *prev_name = cJSON_SetValuestring(name, "updated");
    (void)prev_name;
    cJSON_bool is_flag_true = cJSON_IsTrue(flag);
    cJSON_AddItemToObject(meta, "status", cJSON_CreateString("ok"));

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    const char *name_val = cJSON_GetStringValue(name);
    int size_before = cJSON_GetArraySize(items);
    cJSON_AddItemToArray(items, cJSON_CreateString("extra"));
    int size_after = cJSON_GetArraySize(items);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_val ? name_val[0] : 'N';
    buffer[2] = (char)('0' + (size_before % 10));
    buffer[3] = (char)('0' + (size_after % 10));
    buffer[4] = is_flag_true ? '1' : '0';
    buffer[5] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}