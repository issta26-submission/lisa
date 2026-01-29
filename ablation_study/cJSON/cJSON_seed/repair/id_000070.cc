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
//<ID> 70
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
    cJSON *name = cJSON_CreateString("original");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "values", values);
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(values, n1);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(values, n2);

    // step 2: Configure
    char *newstr = cJSON_SetValuestring(name, "updated");
    cJSON *dup_config = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_copy", dup_config);
    cJSON_bool has_config = cJSON_HasObjectItem(root, "config");
    cJSON_AddNumberToObject(root, "has_config", (double)has_config);

    // step 3: Operate and Validate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_name = cJSON_GetObjectItem(got_config, "name");
    char *got_name_cstr = cJSON_GetStringValue(got_name);
    double first_char_code = (double)(unsigned char)newstr[0];
    double sum_codes = (double)(unsigned char)newstr[0] + (double)(unsigned char)got_name_cstr[0];
    cJSON_AddNumberToObject(root, "first_char_code", first_char_code);
    cJSON_AddNumberToObject(root, "sum_codes", sum_codes);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}