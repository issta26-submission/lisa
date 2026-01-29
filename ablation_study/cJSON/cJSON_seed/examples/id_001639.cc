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
//<ID> 1639
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *s1 = cJSON_CreateString("first");
    cJSON *s2 = cJSON_CreateString("second");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe_null", maybe_null);

    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_string = cJSON_IsString(first_item);
    const char *first_str = cJSON_GetStringValue(first_item);
    cJSON *first_copy = cJSON_CreateString(first_str);
    cJSON_AddItemToObject(root, "first_copy", first_copy);
    cJSON_AddNumberToObject(root, "first_is_string_flag", (double)first_is_string);

    /* create a duplicate to use as the target of a reference to avoid
       potential aliasing/double-free issues when the same array is
       referenced and owned by the root object */
    cJSON *dup_for_ref = cJSON_Duplicate(arr, 1);
    cJSON *arr_ref = cJSON_CreateArrayReference(dup_for_ref);
    cJSON_AddItemToObject(root, "items_ref", arr_ref);

    int size = cJSON_GetArraySize(arr);
    cJSON *size_num = cJSON_CreateNumber((double)size);
    cJSON_AddItemToObject(root, "size", size_num);
    cJSON *dup = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "items_dup", dup);
    cJSON *dup_first_item = cJSON_GetArrayItem(dup, 0);
    const char *dup_first = cJSON_GetStringValue(dup_first_item);
    cJSON *dup_annotation = cJSON_CreateString(dup_first);
    cJSON_AddItemToObject(root, "dup_first", dup_annotation);

    char *unfmt = cJSON_PrintUnformatted(root);
    cJSON_free(unfmt);
    char *full = cJSON_Print(root);
    cJSON_free(full);
    cJSON_Delete(root);

    return 66;
}