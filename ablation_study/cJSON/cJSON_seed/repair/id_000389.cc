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
//<ID> 389
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"config\":{\"threshold\":5.5},\"items\":[1,2,3]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);

    // step 2: Configure
    cJSON *new_array = cJSON_CreateArray();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(new_array, num_item);
    cJSON_AddItemToObject(root, "newArray", new_array);
    cJSON_bool has_config = cJSON_HasObjectItem(root, "config");
    cJSON_AddBoolToObject(root, "has_config", has_config);
    const char *ver = cJSON_Version();
    cJSON_AddStringToObject(root, "lib_version", ver);

    // step 3: Operate
    cJSON *config = cJSON_GetObjectItem(root, "config");
    cJSON *threshold = cJSON_GetObjectItem(config, "threshold");
    double thresh_val = cJSON_GetNumberValue(threshold);
    double computed = thresh_val * 2.0 + (double)has_config;
    cJSON_AddNumberToObject(root, "computed", computed);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Validate and Cleanup
    const char *ver_from_item = cJSON_GetStringValue(cJSON_GetObjectItem(root, "lib_version"));
    (void)ver_from_item;
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}