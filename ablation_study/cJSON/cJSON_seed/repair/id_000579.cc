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
//<ID> 579
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *name_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(config, "name", name_item);
    cJSON *pi_item = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(config, "pi", pi_item);
    cJSON *flag_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(config, "flag", flag_item);

    // step 2: Configure
    char *setstr = cJSON_SetValuestring(name_item, "updated");
    cJSON_AddNumberToObject(config, "extra", 7.0);

    // step 3: Operate and Validate
    char *got = cJSON_GetStringValue(name_item);
    double num = cJSON_GetNumberValue(pi_item);
    cJSON_bool is_false = cJSON_IsFalse(flag_item);
    double multiplier = 1.0 + (double)is_false;
    double derived_value = num * multiplier;
    cJSON_AddNumberToObject(root, "derived", derived_value);
    cJSON_AddStringToObject(root, "name_updated", setstr);
    cJSON_AddStringToObject(root, "name_copy", got);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}