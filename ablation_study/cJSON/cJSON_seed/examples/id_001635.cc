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
//<ID> 1635
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("foo");
    cJSON *s2 = cJSON_CreateString("bar");
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);
    cJSON_AddItemToObject(root, "items", items);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe_null", maybe_null);

    cJSON *items_dup = cJSON_Duplicate(items, 1);
    cJSON *items_ref = cJSON_CreateArrayReference(items_dup);
    cJSON_AddItemToObject(root, "items_ref", items_ref);
    char *first_value = (char *)cJSON_GetStringValue(s1);
    cJSON_AddStringToObject(root, "first_value_copy", first_value);
    cJSON_AddNumberToObject(root, "first_is_string", (double)cJSON_IsString(s1));

    int count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "items_count", (double)count);
    cJSON_AddItemToArray(items, cJSON_Duplicate(s1, 1));
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_free(unformatted);
    char *formatted = cJSON_Print(root);
    cJSON_free(formatted);

    cJSON_Delete(root);
    return 66;
}