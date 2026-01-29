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
//<ID> 219
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *maybe_null = cJSON_CreateNull();

    // step 2: Configure
    double numbers[3] = {1.1, 2.2, 3.3};
    cJSON *darray = cJSON_CreateDoubleArray(numbers, 3);
    const char *strings_arr[3] = {"alpha", "beta", "gamma"};
    cJSON *sarray = cJSON_CreateStringArray(strings_arr, 3);
    cJSON_AddItemToObjectCS(root, "doubles", darray);
    cJSON_AddItemToObjectCS(root, "strings", sarray);
    cJSON_AddItemToObjectCS(root, "maybe", maybe_null);

    // step 3: Operate & Validate
    cJSON_bool null_flag = cJSON_IsNull(maybe_null);
    cJSON *null_flag_num = cJSON_CreateNumber((double)null_flag);
    cJSON_AddItemToObjectCS(root, "is_null", null_flag_num);
    int dcount = cJSON_GetArraySize(darray);
    int scount = cJSON_GetArraySize(sarray);
    cJSON *dcount_num = cJSON_CreateNumber((double)dcount);
    cJSON *scount_num = cJSON_CreateNumber((double)scount);
    cJSON_AddItemToObjectCS(root, "dcount", dcount_num);
    cJSON_AddItemToObjectCS(root, "scount", scount_num);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}