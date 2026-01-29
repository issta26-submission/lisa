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
//<ID> 468
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", num1);
    cJSON *null_marker = cJSON_AddNullToObject(root, "unset");

    // step 2: Configure
    cJSON *retrieved = cJSON_GetObjectItem(root, "answer");
    double base = cJSON_GetNumberValue(retrieved);
    cJSON *num2 = cJSON_CreateNumber(base + 1.0);
    cJSON_AddItemToObject(root, "answer_plus_one", num2);

    // step 3: Operate
    cJSON *got_null = cJSON_GetObjectItem(root, "unset");
    int null_flag = (int)cJSON_IsNull(got_null);
    cJSON_SetNumberHelper(num2, cJSON_GetNumberValue(num2) + (double)null_flag);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    double final_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "answer_plus_one"));
    (void)final_val;
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}