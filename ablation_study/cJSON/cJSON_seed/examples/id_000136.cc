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
//<ID> 136
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", data);

    // step 2: Configure
    double nums_main[4] = {1.1, 2.2, 3.3, 4.4};
    cJSON *dbl_arr = cJSON_CreateDoubleArray(nums_main, 4);
    cJSON_AddItemToObject(root, "doubles", dbl_arr);
    double small_nums[2] = {9.9, 8.8};
    cJSON *small_arr = cJSON_CreateDoubleArray(small_nums, 2);
    cJSON_AddItemToArray(data, small_arr);
    char *buf = (char *)cJSON_malloc(64);
    memset(buf, 0, 64);
    strcpy(buf, "  { \"raw\": 12345 }  ");
    cJSON_Minify(buf);
    cJSON *raw = cJSON_CreateRaw(buf);
    cJSON_AddItemToObject(root, "raw_minified", raw);

    // step 3: Operate & Validate
    int dbl_size = cJSON_GetArraySize(dbl_arr);
    cJSON *second = cJSON_GetArrayItem(dbl_arr, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON_AddNumberToObject(root, "doubles_size", (double)dbl_size);
    cJSON_AddNumberToObject(root, "picked", second_val);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}