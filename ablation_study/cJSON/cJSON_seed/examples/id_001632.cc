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
//<ID> 1632
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *s2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);
    int items_count = cJSON_GetArraySize(items);
    cJSON *count_num = cJSON_CreateNumber((double)items_count);
    cJSON_AddItemToObject(root, "count", count_num);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nullable", maybe_null);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    double first_is_str_flag = (double)cJSON_IsString(first);
    cJSON_AddNumberToObject(root, "first_is_string", first_is_str_flag);
    char *first_val = cJSON_GetStringValue(first);
    cJSON *first_copy = cJSON_CreateString(first_val);
    cJSON_AddItemToObject(root, "first_copy", first_copy);
    cJSON *items_dup = cJSON_Duplicate(items, 1);
    cJSON *items_ref = cJSON_CreateArrayReference(items_dup);
    cJSON_AddItemToObject(root, "items_ref", items_ref);
    char *unfmt = cJSON_PrintUnformatted(root);
    cJSON_free(unfmt);
    char *full = cJSON_Print(root);
    cJSON_free(full);
    cJSON_Delete(root);
    return 66;
}