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
//<ID> 1487
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
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON *lbl = cJSON_CreateString("demo_label");
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "label", lbl);
    size_t initial_buf_size = 16;
    char *temp_buf = (char *)cJSON_malloc(initial_buf_size);
    memset(temp_buf, 0, initial_buf_size);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    cJSON_free(temp_buf);
    char *copy_buf = (char *)cJSON_malloc(printed_len + 1);
    if (printed_len) memcpy(copy_buf, printed, printed_len + 1);
    cJSON_bool is_array = cJSON_IsArray(arr);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    double v1 = cJSON_GetNumberValue(first);
    double v2 = cJSON_GetNumberValue(second);
    double combined = v1 + v2 + (double)is_array;
    (void)combined;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(copy_buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}