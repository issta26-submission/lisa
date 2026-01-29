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
//<ID> 1449
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    const char json_text[] = "{\"items\":[1,2],\"name\":\"orig\"}";
    cJSON *root = cJSON_ParseWithLength(json_text, sizeof(json_text) - 1);
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON_bool items_was_array = cJSON_IsArray(items);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON_bool first_is_invalid = cJSON_IsInvalid(first_item);
    (void)items_was_array;
    (void)first_is_invalid;

    cJSON *container = cJSON_AddObjectToObject(root, "container");
    cJSON *items_dup = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(container, "ref", items_dup);
    cJSON *new_number = cJSON_CreateNumber(42.5);
    cJSON_InsertItemInArray(items, 1, new_number);

    cJSON_bool still_array = cJSON_IsArray(items);
    cJSON *second = cJSON_GetArrayItem(items, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *ref_node = cJSON_GetObjectItem(container, "ref");
    cJSON_bool ref_invalid = cJSON_IsInvalid(ref_node);
    (void)still_array;
    (void)second_val;
    (void)ref_invalid;
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    memcpy(buffer, printed, printed_len + 1);
    cJSON_Minify(buffer);
    cJSON_free(printed);
    cJSON_free(buffer);

    cJSON_Delete(root);
    return 66;
}