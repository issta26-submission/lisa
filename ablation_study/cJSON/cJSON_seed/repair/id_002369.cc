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
//<ID> 2369
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int numbers[3] = {10, 20, 30};
    cJSON *int_array = cJSON_CreateIntArray(numbers, 3);
    cJSON *extra_num = cJSON_CreateNumber(7.25);
    cJSON_AddItemToArray(int_array, extra_num);

    // step 2: Configure
    cJSON_AddItemToObject(root, "int_array", int_array);
    cJSON_AddBoolToObject(root, "active", 1);
    cJSON_AddNumberToObject(root, "threshold", 42.0);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON *thr_item = cJSON_GetObjectItem(root, "threshold");
    double thr_val = cJSON_GetNumberValue(thr_item);
    cJSON *arr_mid = cJSON_GetArrayItem(int_array, 1);
    double mid_val = cJSON_GetNumberValue(arr_mid);
    buffer[0] = (printed && printed[0]) ? printed[0] : '\0';
    buffer[1] = (char)(int)thr_val;
    buffer[2] = (char)(int)mid_val;

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}