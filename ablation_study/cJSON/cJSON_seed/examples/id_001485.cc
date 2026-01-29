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
//<ID> 1485
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
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "arr", arr);

    // step 2: Configure
    cJSON *got_arr = cJSON_GetObjectItem(root, "arr");
    cJSON_bool arr_is_array = cJSON_IsArray(got_arr);
    cJSON *first = cJSON_GetArrayItem(got_arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *info = cJSON_CreateObject();
    cJSON_AddNumberToObject(info, "first_value", first_val);
    cJSON_AddItemToObject(root, "info", info);

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    void *buffer = cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    memcpy(buffer, printed, printed_len + 1);
    char *buffer_str = (char *)buffer;
    (void)arr_is_array;
    (void)buffer_str;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}