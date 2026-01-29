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
//<ID> 1407
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *arr = NULL;
    cJSON *str_item = NULL;
    cJSON *num_item = NULL;
    cJSON *retrieved = NULL;
    char *buffer = NULL;
    int buf_len = 256;
    cJSON_bool printed_ok = 0;
    cJSON_bool add_ref_ok = 0;
    int arr_size = 0;
    cJSON_bool is_string = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    str_item = cJSON_AddStringToObject(child, "greeting", "hello");
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "list", arr);
    num_item = cJSON_CreateNumber(123.0);
    cJSON_AddItemToArray(arr, num_item);
    add_ref_ok = cJSON_AddItemReferenceToObject(root, "greeting_ref", str_item);

    // step 3: Operate / Validate
    buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    retrieved = cJSON_GetObjectItem(root, "list");
    arr_size = cJSON_GetArraySize(retrieved);
    is_string = cJSON_IsString(cJSON_GetObjectItem(root, "greeting_ref"));
    (void)printed_ok;
    (void)add_ref_ok;
    (void)arr_size;
    (void)is_string;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}