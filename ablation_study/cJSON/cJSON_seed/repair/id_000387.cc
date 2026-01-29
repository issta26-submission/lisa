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
//<ID> 387
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"alpha\":10,\"beta\":[1,2,3],\"gamma\":\"ok\"}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON_bool has_alpha = cJSON_HasObjectItem(parsed, "alpha");
    const char *version = cJSON_Version();
    cJSON *ver_item = cJSON_CreateString(version);
    cJSON_AddItemToObject(root, "version", ver_item);
    cJSON *beta = cJSON_GetObjectItem(parsed, "beta");
    cJSON *beta_copy = cJSON_Duplicate(beta, 1);
    cJSON_AddItemToObject(root, "numbers", beta_copy);
    cJSON *extra_array = cJSON_CreateArray();
    cJSON *e1 = cJSON_CreateNumber(7.0);
    cJSON *e2 = cJSON_CreateNumber(8.0);
    cJSON_AddItemToArray(extra_array, e1);
    cJSON_AddItemToArray(extra_array, e2);
    cJSON_AddItemToObject(root, "extra", extra_array);
    cJSON *alpha_item = cJSON_GetObjectItem(parsed, "alpha");
    double alpha_val = cJSON_GetNumberValue(alpha_item);
    double adjust = has_alpha ? 5.0 : 0.0;
    cJSON *computed = cJSON_CreateNumber(alpha_val + adjust);
    cJSON_AddItemToObject(root, "computed", computed);

    // step 3: Operate and Validate
    cJSON *numbers = cJSON_GetObjectItem(root, "numbers");
    int numbers_size = cJSON_GetArraySize(numbers);
    cJSON *first_num = cJSON_GetArrayItem(numbers, 0);
    double first_val = cJSON_GetNumberValue(first_num);
    double final_value = first_val + (double)numbers_size;
    cJSON *final_item = cJSON_CreateNumber(final_value);
    cJSON_AddItemToObject(root, "final", final_item);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}