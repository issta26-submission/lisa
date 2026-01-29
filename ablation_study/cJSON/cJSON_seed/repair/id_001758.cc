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
//<ID> 1758
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON_AddItemToObject(info, "name", cJSON_CreateString("alpha"));
    cJSON_AddItemToObject(info, "value", cJSON_CreateNumber(42.0));
    cJSON_AddItemToArray(list, cJSON_CreateString("one"));
    cJSON_AddItemToArray(list, cJSON_CreateString("two"));

    // step 2: Configure
    cJSON *mirror = cJSON_CreateObject();
    cJSON *mirror_info = cJSON_CreateObject();
    cJSON_AddItemToObject(mirror, "info", mirror_info);
    cJSON_AddItemToObject(mirror_info, "name", cJSON_CreateString("alpha"));
    cJSON_AddItemToObject(mirror_info, "value", cJSON_CreateNumber(42.0));
    cJSON *mirror_list = cJSON_CreateArray();
    cJSON_AddItemToObject(mirror, "list", mirror_list);
    cJSON_AddItemToArray(mirror_list, cJSON_CreateString("one"));
    cJSON_AddItemToArray(mirror_list, cJSON_CreateString("two"));

    // step 3: Operate and Validate
    cJSON_bool are_equal = cJSON_Compare(root, mirror, 1);
    char *printed = cJSON_Print(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    const char *name_val = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetObjectItem(root, "info"), "name"));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_val ? name_val[0] : 'Y';
    buffer[2] = (char)('0' + (int)are_equal);
    (void)buffer;
    (void)name_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(mirror);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}