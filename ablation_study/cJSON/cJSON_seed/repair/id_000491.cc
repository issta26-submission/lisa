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
//<ID> 491
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"base\":true,\"list\":[10.5]}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *added_array = cJSON_CreateArray();
    cJSON *numA = cJSON_CreateNumber(100.0);
    cJSON *numB = cJSON_CreateNumber(200.25);
    cJSON_AddItemToArray(added_array, numA);
    cJSON_AddItemToArray(added_array, numB);
    cJSON_AddItemToObject(root, "added_array", added_array);

    // step 2: Configure
    cJSON *nested = cJSON_AddObjectToObject(root, "nested");
    cJSON_AddItemToObject(nested, "note", cJSON_CreateString("configured"));
    cJSON_AddItemToObject(nested, "enabled", cJSON_CreateTrue());

    // step 3: Operate
    cJSON *orig_list = cJSON_GetObjectItem(root, "list");
    cJSON *first = cJSON_GetArrayItem(orig_list, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *doubled = cJSON_CreateNumber(first_val * 2.0);
    cJSON_AddItemToArray(added_array, doubled);
    cJSON *parsed_fragment = cJSON_Parse("{\"extra\":123}");
    cJSON_AddItemToObject(root, "fragment", parsed_fragment);

    // step 4: Validate and Cleanup
    cJSON *got = cJSON_GetObjectItem(root, "added_array");
    int size = cJSON_GetArraySize(got);
    cJSON *last = cJSON_GetArrayItem(got, size - 1);
    double last_val = cJSON_GetNumberValue(last);
    (void)last_val;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}