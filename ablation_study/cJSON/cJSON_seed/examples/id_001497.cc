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
//<ID> 1497
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("hello");
    cJSON *s2 = cJSON_CreateString("world");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "arr", arr);

    cJSON *arr_dup_for_ref = cJSON_Duplicate(arr, 1);
    cJSON *ref = cJSON_CreateArrayReference(arr_dup_for_ref);
    cJSON_AddItemToObject(root, "ref", ref);
    char *printed = cJSON_PrintUnformatted(root);
    char *first_str = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 0));
    cJSON *copied_str = cJSON_CreateString(first_str);
    cJSON_AddItemToObject(root, "copied", copied_str);

    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON *dup_ref = cJSON_GetObjectItem(dup, "ref");
    cJSON *dup_first = cJSON_GetArrayItem(dup_ref, 0);
    char *dup_first_str = cJSON_GetStringValue(dup_first);
    (void)dup_first_str;

    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    return 66;
}