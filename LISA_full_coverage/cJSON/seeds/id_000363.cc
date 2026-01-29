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
//<ID> 363
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double initial_nums[3] = {1.1, 2.2, 3.3};
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateDoubleArray(initial_nums, 3);
    cJSON_AddItemToObject(root, "numbers", numbers);

    // step 2: Configure
    cJSON *extra = cJSON_CreateNumber(4.4);
    cJSON_AddItemToArray(numbers, extra);
    cJSON *stats = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "stats", stats);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(numbers);
    cJSON *first = cJSON_GetArrayItem(numbers, 0);
    cJSON *fourth = cJSON_GetArrayItem(numbers, 3);
    double v_first = cJSON_GetNumberValue(first);
    double v_fourth = cJSON_GetNumberValue(fourth);
    double sum = v_first + v_fourth;
    cJSON_AddNumberToObject(stats, "count", (double)count);
    cJSON_AddNumberToObject(stats, "sum", sum);
    char *printed = cJSON_PrintUnformatted(root);
    int buf_len = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 0);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}