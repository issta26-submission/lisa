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
//<ID> 624
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    int numbers_arr[] = { 10, 20, 30, 40 };
    cJSON *int_array = cJSON_CreateIntArray(numbers_arr, 4);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "numbers", int_array);
    cJSON_AddNullToObject(root, "nullable");
    cJSON *id_item = cJSON_CreateNumber(2026.0);
    cJSON_AddItemToObject(root, "id", id_item);

    // step 2: Configure
    size_t bufsize = 512;
    char *buffer = (char *)cJSON_malloc(bufsize);
    memset(buffer, 0, bufsize);
    cJSON_PrintPreallocated(root, buffer, (int)bufsize, 0);

    // step 3: Operate & Validate
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_numbers = cJSON_GetObjectItem(parsed, "numbers");
    int parsed_count = cJSON_GetArraySize(parsed_numbers);
    cJSON *parsed_nullable = cJSON_GetObjectItem(parsed, "nullable");
    cJSON_bool nullable_is_null = cJSON_IsNull(parsed_nullable);
    cJSON *size_item = cJSON_CreateNumber((double)parsed_count);
    cJSON_AddItemToObject(parsed, "arr_size", size_item);

    // step 4: Cleanup
    cJSON_DeleteItemFromObject(root, "id");
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_free(buffer);
    // API sequence test completed successfully
    (void)version;
    (void)nullable_is_null;
    return 66;
}