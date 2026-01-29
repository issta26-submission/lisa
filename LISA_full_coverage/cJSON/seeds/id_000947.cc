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
//<ID> 947
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
    cJSON_AddItemToObject(root, "values", array);

    // step 2: Configure
    cJSON *num0 = cJSON_CreateNumber(10.0);
    cJSON *num1 = cJSON_CreateNumber(20.5);
    cJSON *num2 = cJSON_CreateNumber(30.25);
    cJSON_InsertItemInArray(array, 0, num0);
    cJSON_InsertItemInArray(array, 1, num1);
    cJSON_InsertItemInArray(array, 2, num2);

    // step 3: Operate
    cJSON *middle_item = cJSON_GetArrayItem(array, 1);
    double middle_value = cJSON_GetNumberValue(middle_item);
    cJSON_AddNumberToObject(root, "middle_value", middle_value);

    // step 4: Validate & Cleanup
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}