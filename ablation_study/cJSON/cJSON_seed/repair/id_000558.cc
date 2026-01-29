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
//<ID> 558
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_initial = cJSON_AddNumberToObject(root, "initial", 1.5);
    cJSON *created_num = cJSON_CreateNumber(2.5);
    cJSON_AddItemToObject(root, "created_num", created_num);

    // step 2: Configure
    cJSON *root_dup = cJSON_Duplicate(root, 1);
    cJSON *dup_initial = cJSON_GetObjectItem(root_dup, "initial");
    double dup_initial_val = cJSON_GetNumberValue(dup_initial);
    double updated_val = dup_initial_val + 7.5;
    cJSON_SetNumberHelper(num_initial, updated_val);
    cJSON_AddNumberToObject(root, "adjusted", cJSON_GetNumberValue(num_initial));

    // step 3: Operate and Validate
    cJSON *dup_created = cJSON_GetObjectItem(root_dup, "created_num");
    double sum_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root_dup, "initial")) + cJSON_GetNumberValue(dup_created);
    cJSON_AddNumberToObject(root_dup, "sum", sum_val);
    char *out = cJSON_PrintUnformatted(root_dup);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root_dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}