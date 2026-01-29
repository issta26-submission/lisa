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
//<ID> 553
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
    cJSON_AddNumberToObject(root, "initial", 21.0);
    cJSON *pi_item = cJSON_CreateNumber(3.1415926535);
    cJSON_AddItemToObject(nested, "pi", pi_item);

    // step 2: Configure
    cJSON *init_item = cJSON_GetObjectItem(root, "initial");
    double updated_initial = cJSON_SetNumberHelper(init_item, 42.0);
    cJSON *root_dup = cJSON_Duplicate(root, 1);
    cJSON *dup_nested = cJSON_GetObjectItem(root_dup, "nested");
    cJSON_AddNumberToObject(dup_nested, "added_in_dup", 7.5);

    // step 3: Operate and Validate
    cJSON *pi_from_nested = cJSON_GetObjectItem(nested, "pi");
    double pi_value = cJSON_GetNumberValue(pi_from_nested);
    cJSON *product = cJSON_CreateNumber(pi_value * updated_initial);
    cJSON_AddItemToObject(root, "product", product);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root_dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}