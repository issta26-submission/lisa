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
//<ID> 1139
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double values[3];
    values[0] = 1.5;
    values[1] = 2.5;
    values[2] = 3.5;
    cJSON *dbl_array = cJSON_CreateDoubleArray(values, 3);
    cJSON *true_flag = cJSON_CreateTrue();
    cJSON *label = cJSON_CreateString("demo_label");

    // step 2: Configure
    cJSON_AddItemToObject(root, "numbers", dbl_array);
    cJSON_AddItemToObjectCS(root, "enabled", true_flag);
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate & Validate
    cJSON *first_item = cJSON_GetArrayItem(dbl_array, 0);
    double original_first = cJSON_GetNumberValue(first_item);
    double prev_value = cJSON_SetNumberHelper(first_item, original_first + 4.0);
    cJSON_bool flag_state = cJSON_IsTrue(true_flag);
    double computed = prev_value + cJSON_GetNumberValue(first_item) + (double)flag_state;
    cJSON_AddNumberToObject(root, "computed", computed);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}