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
//<ID> 1496
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "name", "example");
    cJSON_AddNumberToObject(child, "id", 42);
    cJSON_AddItemToObject(root, "child", child);
    cJSON *child_ref = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "child_ref", child_ref);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_bool root_is_object = cJSON_IsObject(root);
    cJSON *child_name_item = cJSON_GetObjectItem(child, "name");
    char *name_val = cJSON_GetStringValue(child_name_item);
    cJSON *duplicate = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, duplicate, 1);
    char *dup_printed = cJSON_PrintUnformatted(duplicate);
    cJSON *dup_child = cJSON_GetObjectItem(duplicate, "child");
    cJSON *dup_child_name_item = cJSON_GetObjectItem(dup_child, "name");
    char *dup_name_val = cJSON_GetStringValue(dup_child_name_item);
    (void)root_is_object;
    (void)name_val;
    (void)equal;
    (void)dup_name_val;
    cJSON_free(printed);
    cJSON_free(dup_printed);
    cJSON_Delete(duplicate);
    cJSON_Delete(root);
    return 66;
}