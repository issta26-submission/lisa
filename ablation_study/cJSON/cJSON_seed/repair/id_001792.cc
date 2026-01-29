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
//<ID> 1792
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);
    cJSON *item1 = cJSON_CreateObject();
    cJSON_AddItemToObject(item1, "name", cJSON_CreateString("alpha"));
    cJSON_AddItemToObject(item1, "value", cJSON_CreateNumber(1.0));
    cJSON *initial_status = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "status", initial_status);

    // step 2: Configure
    cJSON_AddItemToArray(array, item1);
    cJSON *dup_item = cJSON_Duplicate(item1, 1);
    cJSON_AddItemToArray(array, dup_item);
    cJSON_ReplaceItemInObject(root, "status", cJSON_CreateTrue());

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    int arr_size = cJSON_GetArraySize(array);
    cJSON *first = cJSON_GetArrayItem(array, 0);
    cJSON *second = cJSON_GetArrayItem(array, 1);
    const char *first_name = cJSON_GetStringValue(cJSON_GetObjectItem(first, "name"));
    const char *second_name = cJSON_GetStringValue(cJSON_GetObjectItem(second, "name"));
    cJSON_bool has_status = cJSON_HasObjectItem(root, "status");
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = first_name ? first_name[0] : 'Y';
    buffer[2] = second_name ? second_name[0] : 'Z';
    buffer[3] = (char)('0' + (arr_size % 10));
    buffer[4] = has_status ? '1' : '0';
    (void)buffer;
    (void)first_name;
    (void)second_name;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}