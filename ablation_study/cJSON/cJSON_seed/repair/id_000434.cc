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
//<ID> 434
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"config\":{\"name\":\"testdoc\",\"value\":42},\"nested\":{\"inner\":{\"x\":3.14,\"y\":2}},\"flag\":true}";
    cJSON *root = cJSON_Parse(json);
    cJSON *config = cJSON_GetObjectItem(root, "config");
    cJSON *nested = cJSON_GetObjectItem(root, "nested");
    cJSON *inner = cJSON_GetObjectItem(nested, "inner");
    cJSON *name_item = cJSON_GetObjectItem(config, "name");
    char *name_str = cJSON_GetStringValue(name_item);

    // step 2: Configure
    cJSON *generated = cJSON_AddObjectToObject(root, "generated");
    cJSON_bool has_cfg = cJSON_HasObjectItem(root, "config");
    cJSON_AddBoolToObject(generated, "config_present", has_cfg);
    cJSON *inner_ref = cJSON_CreateObjectReference(inner);
    cJSON_AddItemReferenceToObject(generated, "inner_ref", inner_ref);
    cJSON_AddStringToObject(generated, "copied_name", name_str);

    // step 3: Operate
    cJSON *x_item = cJSON_GetObjectItem(inner, "x");
    double x_val = cJSON_GetNumberValue(x_item);
    cJSON_AddNumberToObject(generated, "inner_x", x_val);
    char *repr = cJSON_PrintUnformatted(root);

    // step 4: Validate and Cleanup
    cJSON *check = cJSON_GetObjectItem(generated, "inner_x");
    double retrieved_x = cJSON_GetNumberValue(check);
    (void)retrieved_x;
    cJSON_free(repr);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}