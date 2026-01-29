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
//<ID> 366
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double nums[3] = {1.1, 2.2, 3.3};
    cJSON *root = cJSON_CreateObject();
    cJSON *dbl_arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "dbls", dbl_arr);
    cJSON *extra_num = cJSON_CreateNumber(4.4);
    cJSON_AddItemToArray(dbl_arr, extra_num);

    // step 2: Configure
    int count = cJSON_GetArraySize(dbl_arr);
    cJSON *second_item = cJSON_GetArrayItem(dbl_arr, 1);
    cJSON *stats = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "stats", stats);
    cJSON_AddNumberToObject(stats, "count", (double)count);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    int buf_len = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 0);
    double second_val = cJSON_GetNumberValue(second_item);
    cJSON_AddNumberToObject(stats, "second_val", second_val);

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}