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
//<ID> 1405
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int ints[] = {1, 2, 3, 4};
    cJSON *int_arr = cJSON_CreateIntArray(ints, 4);
    cJSON *num_item = cJSON_CreateNumber(42.5);
    cJSON *str_item = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "ints", int_arr);
    cJSON_AddItemToObject(root, "answer", num_item);
    cJSON_AddItemToObject(root, "greeting", str_item);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    cJSON *got_answer = cJSON_GetObjectItem(root, "answer");
    double answer_val = cJSON_GetNumberValue(got_answer);
    cJSON_SetNumberHelper(got_answer, answer_val + 7.5);
    cJSON *appended = cJSON_CreateNumber(99.0);
    cJSON_AddItemToArray(int_arr, appended);
    int arr_size = cJSON_GetArraySize(int_arr);
    cJSON *last_elem = cJSON_GetArrayItem(int_arr, arr_size - 1);
    double last_value = cJSON_GetNumberValue(last_elem);
    (void)last_value;

    // step 3: Operate & Validate
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, dup, 1);
    (void)equal;
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(dup);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}