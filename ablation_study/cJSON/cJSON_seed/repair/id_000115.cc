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
//<ID> 115
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
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "values", values);
    cJSON_AddItemToArray(values, cJSON_CreateNumber(3.14));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(2.71));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(1.62));

    // step 2: Configure
    cJSON_AddStringToObject(config, "name", "alpha");
    cJSON *dup_config = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "backup", dup_config);

    // step 3: Operate and Validate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_values = cJSON_GetObjectItem(got_config, "values");
    cJSON *first_item = cJSON_GetArrayItem(got_values, 0);
    cJSON *dup_item = cJSON_Duplicate(first_item, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON_AddNumberToObject(root, "first_value", first_val);
    cJSON *dup_name = cJSON_GetObjectItem(dup_config, "name");
    char *name_str = cJSON_GetStringValue(dup_name);
    cJSON_AddNumberToObject(root, "name_first_char_code", (double)(unsigned char)name_str[0]);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(dup_item);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}