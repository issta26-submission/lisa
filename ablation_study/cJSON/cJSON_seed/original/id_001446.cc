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
//<ID> 1446
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *parent = cJSON_AddObjectToObject(root, "parent");
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON *arr_ref = cJSON_CreateArrayReference(arr);
    cJSON_AddItemToObject(parent, "numbers_ref", arr_ref);

    // step 2: Configure
    cJSON *new_elem = cJSON_CreateNumber(15.0);
    cJSON *first_elem = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_invalid = cJSON_IsInvalid(first_elem);
    int insert_index = 1 + (int)first_is_invalid;
    cJSON_InsertItemInArray(arr, insert_index, new_elem);

    // step 3: Operate & Validate
    cJSON_bool arr_is_array = cJSON_IsArray(arr);
    cJSON_bool arrref_is_array = cJSON_IsArray(arr_ref);
    (void)arr_is_array;
    (void)arrref_is_array;
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    memcpy(buffer, printed, printed_len + 1);
    cJSON_Minify(buffer);
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(arr);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}