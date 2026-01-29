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
//<ID> 241
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(3.5);
    cJSON *n2 = cJSON_CreateNumber(4.5);

    // step 2: Configure
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToObject(root, "values", values);
    cJSON *sum = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "sum", sum);

    // step 3: Operate & Validate
    double v1 = cJSON_GetNumberValue(n1);
    double v2 = cJSON_GetNumberValue(n2);
    double newsum = v1 + v2;
    cJSON_SetNumberHelper(sum, newsum);
    cJSON *sum_ptr = cJSON_GetObjectItem(root, "sum");
    double verified = cJSON_GetNumberValue(sum_ptr);
    cJSON *verified_num = cJSON_CreateNumber(verified);
    cJSON_AddItemToObject(root, "verified_sum", verified_num);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}