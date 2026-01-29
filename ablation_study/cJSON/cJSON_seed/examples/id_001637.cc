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
//<ID> 1637
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
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe_null", maybe_null);

    cJSON *s1_ref = cJSON_CreateStringReference(cJSON_GetStringValue(s1));
    cJSON_AddItemReferenceToArray(arr, s1_ref);
    int arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "items_count", (double)arr_size);

    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    const char *first_str = cJSON_GetStringValue(first_item);
    cJSON_bool is_str = cJSON_IsString(first_item);
    cJSON *first_copy = cJSON_CreateString(first_str);
    cJSON_AddItemToObject(root, "first_copy", first_copy);
    cJSON_AddNumberToObject(root, "is_first_string", (double)is_str);
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_free(unformatted);

    char *full = cJSON_Print(root);
    cJSON_free(full);
    cJSON_Delete(root);

    return 66;
}