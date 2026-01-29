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
//<ID> 2147
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
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *elem1 = cJSON_CreateString("first_entry");
    cJSON *elem2 = cJSON_CreateString("second_entry");
    cJSON_AddItemToArray(items, elem1);
    cJSON_AddItemToArray(items, elem2);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "inner", "inner_value");
    cJSON_AddItemToObject(root, "child", child);
    cJSON *dup_child = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "dup_child", dup_child);
    cJSON_AddStringToObject(root, "greeting", "hello_world");

    // step 3: Operate and Validate
    cJSON *fetched_items = cJSON_GetObjectItem(root, "items");
    cJSON_bool is_array = cJSON_IsArray(fetched_items);
    int arr_size = cJSON_GetArraySize(fetched_items);
    cJSON *first_item = cJSON_GetArrayItem(fetched_items, 0);
    const char *first_str = cJSON_GetStringValue(first_item);
    const int buflen = 16;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    char ch0 = '\0';
    const char *tmp = first_str;
    tmp && (ch0 = tmp[0]);
    buffer[0] = ch0;
    buffer[1] = (char)('N' + (is_array * ('A' - 'N')));
    buffer[2] = (char)('0' + (arr_size % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}