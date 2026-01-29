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
//<ID> 1432
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *val_item = cJSON_CreateNumber(10.5);
    cJSON_AddItemToObject(root, "value", val_item);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "added_direct", 2.25);
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddNumberToObject(nested, "inner", 100.0);
    cJSON_AddItemToObject(root, "nested", nested);

    // step 3: Operate & Validate
    cJSON *retrieved = cJSON_GetObjectItem(root, "value");
    cJSON_bool is_num_original = cJSON_IsNumber(retrieved);
    double original_val = cJSON_GetNumberValue(retrieved);
    double doubled = original_val * 2.0;
    cJSON_AddNumberToObject(root, "double_of_value", doubled);
    cJSON *dup = cJSON_Duplicate(retrieved, 1);
    cJSON_AddItemToObject(root, "dup_value", dup);
    cJSON_bool is_num_dup = cJSON_IsNumber(dup);
    int buf_len = 256;
    char buffer[256];
    memset(buffer, 0, buf_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    (void)is_num_original;
    (void)is_num_dup;
    (void)printed;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}