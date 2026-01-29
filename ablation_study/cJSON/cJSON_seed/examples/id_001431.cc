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
//<ID> 1431
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "nested", nested);
    cJSON *pi_item = cJSON_AddNumberToObject(root, "pi", 3.14159265);

    // step 2: Configure
    cJSON_AddNumberToObject(nested, "val", 42.0);
    cJSON *dup_nested = cJSON_Duplicate(nested, 1);

    // step 3: Operate & Validate
    cJSON *dup_val = cJSON_GetObjectItem(dup_nested, "val");
    cJSON_bool is_pi_number = cJSON_IsNumber(pi_item);
    cJSON_bool is_dup_val_number = cJSON_IsNumber(dup_val);
    char buffer[256];
    memset(buffer, 0, 256);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, 256, 1);
    (void)is_pi_number;
    (void)is_dup_val_number;
    (void)printed;

    // step 4: Cleanup
    cJSON_Delete(dup_nested);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}