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
//<ID> 384
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    const char *json_text = "{\"config\": {\"enabled\": true, \"threshold\": 3.14}}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON_bool has_config = cJSON_HasObjectItem(parsed, "config");
    const char *ver = cJSON_Version();
    cJSON *ver_item = cJSON_CreateString(ver);
    cJSON_AddItemToObject(root, "version", ver_item);
    cJSON *flag_item = cJSON_CreateNumber((double)has_config);
    cJSON_AddItemToObject(root, "has_config_flag", flag_item);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(values, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *computed = cJSON_CreateNumber(first_val + 42.0 * (double)has_config);
    cJSON_AddItemToObject(root, "computed", computed);
    cJSON *config_obj = cJSON_GetObjectItem(parsed, "config");
    cJSON *config_copy = cJSON_Duplicate(config_obj, 1);
    cJSON_AddItemToObject(root, "config_copy", config_copy);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}