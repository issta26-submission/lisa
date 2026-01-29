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
//<ID> 1561
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *a_item = cJSON_AddNumberToObject(root, "a", 3.5);
    cJSON *b_item = cJSON_CreateNumber(2.25);
    cJSON_AddItemToObject(root, "b", b_item);
    cJSON *flag_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag_item);

    // step 2: Configure
    char *printed = cJSON_Print(root);

    // step 3: Operate & Validate
    cJSON *got_a = cJSON_GetObjectItem(root, "a");
    cJSON *got_b = cJSON_GetObjectItem(root, "b");
    cJSON *got_flag = cJSON_GetObjectItem(root, "flag");
    double a_val = cJSON_GetNumberValue(got_a);
    double b_val = cJSON_GetNumberValue(got_b);
    double sum = a_val + b_val;
    cJSON *sum_item = cJSON_AddNumberToObject(root, "sum", sum);
    cJSON_bool flag_true = cJSON_IsTrue(got_flag);
    double final_value = sum + (double)flag_true;
    cJSON *final_item = cJSON_CreateNumber(final_value);
    cJSON_AddItemToObject(root, "final", final_item);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}