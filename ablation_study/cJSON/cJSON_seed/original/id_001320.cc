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
//<ID> 1320
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
    cJSON *arr2 = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddItemToObject(root, "arr2", arr2);
    cJSON *num1 = cJSON_CreateNumber(1.0);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON *shared = cJSON_CreateString("shared_string");
    cJSON_AddItemToArray(arr, shared);

    // step 2: Configure
    cJSON_AddItemReferenceToArray(arr2, shared);
    cJSON *inserted = cJSON_CreateString("inserted_at_one");

    // step 3: Operate
    cJSON_InsertItemInArray(arr, 1, inserted);
    double new_val = cJSON_SetNumberHelper(num1, 42.0);
    (void)new_val;
    int buffer_len = 1024;
    char *buffer = (char *)cJSON_malloc(buffer_len);
    memset(buffer, 0, buffer_len);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buffer_len, 1);
    (void)printed_ok;

    // step 4: Validate & Cleanup
    size_t printed_len = strlen(buffer);
    char *copy_buf = (char *)cJSON_malloc(printed_len + 1);
    memset(copy_buf, 0, printed_len + 1);
    memcpy(copy_buf, buffer, printed_len + 1);
    cJSON_Minify(copy_buf);
    cJSON_free(buffer);
    cJSON_free(copy_buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}