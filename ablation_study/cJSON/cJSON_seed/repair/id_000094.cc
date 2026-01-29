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
//<ID> 94
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON *name = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(settings, "name", name);
    cJSON *values = cJSON_CreateArray();
    cJSON *v1 = cJSON_CreateNumber(10.0);
    cJSON *v2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(values, v1);
    cJSON_AddItemToArray(values, v2);
    cJSON_AddItemToObject(settings, "values", values);

    // step 2: Configure
    char *set_ret = cJSON_SetValuestring(name, "beta");
    cJSON *dup_settings = cJSON_Duplicate(settings, 1);
    cJSON_AddItemToObject(root, "backup", dup_settings);

    // step 3: Operate and Validate
    cJSON *got_values = cJSON_GetObjectItem(settings, "values");
    int count = cJSON_GetArraySize(got_values);
    cJSON_AddNumberToObject(root, "values_count", (double)count);
    cJSON *first = cJSON_GetArrayItem(got_values, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_AddNumberToObject(root, "first_value_times_two", first_val * 2.0);
    cJSON *got_name = cJSON_GetObjectItem(settings, "name");
    const char *name_str = cJSON_GetStringValue(got_name);
    cJSON_AddNumberToObject(root, "first_char_code", (double)(unsigned char)name_str[0]);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    (void)set_ret;

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}