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
//<ID> 2455
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *obj1 = cJSON_CreateObject();
    cJSON *name_str = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(obj1, "name", name_str);
    cJSON_AddNumberToObject(obj1, "value", 42.0);
    cJSON_AddItemToArray(items, obj1);
    cJSON_AddItemToArray(items, cJSON_CreateString("beta"));
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *items_ref = cJSON_CreateArrayReference(items);
    cJSON_AddNumberToObject(root, "count", 2.0);
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';

    // step 3: Operate
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON *first_name_item = cJSON_GetObjectItem(first, "name");
    char *first_name = cJSON_GetStringValue(first_name_item);
    cJSON_bool has_items = cJSON_HasObjectItem(root, "items");
    cJSON_AddNumberToObject(first, "extra", 99.0);
    cJSON *ref_second = cJSON_GetArrayItem(items_ref, 1);
    char *second_name = cJSON_GetStringValue(ref_second);

    // step 4: Validate and Cleanup
    buffer[1] = first_name && first_name[0] ? first_name[0] : '\0';
    buffer[2] = second_name && second_name[0] ? second_name[0] : '\0';
    buffer[3] = (char)(int)has_items;
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(items_ref);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}