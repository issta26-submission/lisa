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
//<ID> 309
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int numbers[] = {1, 2, 3, 4};
    cJSON *int_array = cJSON_CreateIntArray(numbers, 4);
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON_AddNullToObject(root, "maybe_null");
    cJSON_AddFalseToObject(root, "was_success");

    // step 2: Configure
    const char json_text[] = "{\"external\":10,\"flag\":false}";
    cJSON *external = cJSON_ParseWithLength(json_text, sizeof(json_text) - 1);
    cJSON_AddItemToObject(root, "external_obj", external);

    // step 3: Operate and Validate
    cJSON_bool has_ints = cJSON_HasObjectItem(root, "ints");
    cJSON_AddBoolToObject(root, "has_ints", has_ints);
    cJSON *first_item = cJSON_GetArrayItem(int_array, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON_AddNumberToObject(root, "first_val", first_val);
    cJSON_bool has_external = cJSON_HasObjectItem(root, "external_obj");
    cJSON_AddBoolToObject(root, "has_external", has_external);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}