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
//<ID> 2410
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
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(items, num_item);
    cJSON_AddItemToArray(items, str_item);
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *dup_items = cJSON_Duplicate(items, 1);
    cJSON *orig_first = cJSON_GetArrayItem(items, 0);
    cJSON *dup_second = cJSON_GetArrayItem(dup_items, 1);
    double orig_num = cJSON_GetNumberValue(orig_first);
    char *dup_str = cJSON_GetStringValue(dup_second);
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = (char)(int)orig_num;
    buffer[2] = dup_str[0];

    // step 3: Operate
    char *updated = cJSON_SetValuestring(dup_second, "world");
    buffer[3] = updated[0];
    char *printed_dup = cJSON_PrintUnformatted(dup_items);
    buffer[4] = printed_dup[0];

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(printed_dup);
    cJSON_free(buffer);
    cJSON_Delete(dup_items);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}