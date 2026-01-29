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
//<ID> 381
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"alpha\": 1, \"beta\": [2,3]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    const char *version = cJSON_Version();
    cJSON *added_array = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON *n2 = cJSON_CreateNumber(3.14);

    // step 2: Configure
    cJSON_AddItemToArray(added_array, n1);
    cJSON_AddItemToArray(added_array, n2);
    cJSON_AddItemToObject(root, "added", added_array);
    cJSON_bool has_alpha = cJSON_HasObjectItem(root, "alpha");
    cJSON_bool has_gamma = cJSON_HasObjectItem(root, "gamma");

    // step 3: Operate and Validate
    cJSON *alpha_item = cJSON_GetObjectItem(root, "alpha");
    double alpha_val = cJSON_GetNumberValue(alpha_item);
    double computed_val = alpha_val + (double)has_alpha * 10.0 + (double)has_gamma * -1.0;
    cJSON *computed = cJSON_CreateNumber(computed_val);
    cJSON_AddItemToObject(root, "computed", computed);
    cJSON_AddStringToObject(root, "lib_version", version);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}