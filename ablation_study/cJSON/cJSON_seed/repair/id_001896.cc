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
//<ID> 1896
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "name", cJSON_CreateString("original"));

    // step 2: Configure
    cJSON *num_item = cJSON_CreateNumber(1.0);
    cJSON *str_item = cJSON_CreateString("two");
    cJSON *bool_item = cJSON_CreateTrue();
    cJSON_InsertItemInArray(arr, 0, num_item);
    cJSON_InsertItemInArray(arr, 1, str_item);
    cJSON_InsertItemInArray(arr, 1, bool_item); // inserts bool_item at index 1, pushes str_item to index 2
    cJSON_ReplaceItemInObject(meta, "name", cJSON_CreateString("replaced"));

    // step 3: Operate and Validate
    cJSON *got_item_index1 = cJSON_GetArrayItem(arr, 1);
    cJSON *got_item_index2 = cJSON_GetArrayItem(arr, 2);
    cJSON *got_meta_name = cJSON_GetObjectItem(meta, "name");
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (cJSON_IsTrue(got_item_index1) % 10));
    buffer[2] = (char)('0' + (cJSON_IsString(got_item_index2) % 10));
    buffer[3] = (char)('0' + (cJSON_IsString(got_meta_name) % 10));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}