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
//<ID> 1409
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int ints[] = {1, 2, 3};
    cJSON *num_item = cJSON_CreateNumber(42.5);
    cJSON *int_array = cJSON_CreateIntArray(ints, 3);
    cJSON *str_item = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "number", num_item);
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON_AddItemToObject(root, "message", str_item);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    cJSON *replacement_num = cJSON_CreateNumber(100.0);
    cJSON_ReplaceItemInObject(root, "number", replacement_num);

    // step 3: Operate & Validate
    cJSON *got_num = cJSON_GetObjectItem(root, "number");
    double got_num_val = cJSON_GetNumberValue(got_num);
    cJSON *got_msg = cJSON_GetObjectItem(root, "message");
    const char *got_str = cJSON_GetStringValue(got_msg);
    cJSON *got_arr = cJSON_GetObjectItem(root, "ints");
    int arr_size = cJSON_GetArraySize(got_arr);
    cJSON *arr_elem = cJSON_GetArrayItem(got_arr, 1);
    double arr_elem_val = cJSON_GetNumberValue(arr_elem);
    (void)got_num_val;
    (void)got_str;
    (void)arr_size;
    (void)arr_elem_val;
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}