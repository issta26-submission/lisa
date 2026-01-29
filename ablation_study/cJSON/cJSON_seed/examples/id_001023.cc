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
//<ID> 1023
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
    cJSON *n1 = cJSON_CreateNumber(10.5);
    cJSON *n2 = cJSON_CreateNumber(20.25);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON *n3 = cJSON_CreateNumber(15.75);

    // step 2: Configure
    cJSON_InsertItemInArray(values, 1, n3);
    cJSON_AddItemToObject(root, "values", values);
    double sum_val = cJSON_GetNumberValue(n1) + cJSON_GetNumberValue(n2) + cJSON_GetNumberValue(n3);
    cJSON_AddNumberToObject(root, "sum", sum_val);

    // step 3: Operate & Validate
    cJSON_bool is_obj = cJSON_IsObject(root);
    (void)is_obj;
    double retrieved_sum = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "sum"));
    cJSON_AddNumberToObject(root, "computed", retrieved_sum);
    char *out = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}