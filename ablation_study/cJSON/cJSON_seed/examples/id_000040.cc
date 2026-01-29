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
//<ID> 40
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"info\":{\"version\":1},\"meta\":\"example\"}";
    cJSON *parsed = cJSON_Parse(json);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "source", parsed);

    // step 2: Configure
    double values[4] = {0.5, 1.5, 2.5, 3.5};
    cJSON *double_array = cJSON_CreateDoubleArray(values, 4);
    cJSON_AddItemToObjectCS(root, "numbers", double_array);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObjectCS(root, "enabled", false_item);

    // step 3: Operate & Validate
    cJSON_bool has_numbers = cJSON_HasObjectItem(root, "numbers");
    cJSON_bool has_enabled = cJSON_HasObjectItem(root, "enabled");
    (void)has_numbers;
    (void)has_enabled;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}