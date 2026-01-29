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
//<ID> 1393
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double init_doubles[3] = {1.1, 2.2, 3.3};
    cJSON *doubles = cJSON_CreateDoubleArray(init_doubles, 3);
    cJSON_AddItemToObject(root, "doubles", doubles);
    float init_floats[2] = {10.0f, 20.0f};
    cJSON *floats = cJSON_CreateFloatArray(init_floats, 2);
    cJSON_AddItemToObject(root, "floats", floats);
    cJSON_AddStringToObject(root, "name", "example");

    // step 2: Configure
    cJSON *doubles_ptr = cJSON_GetObjectItemCaseSensitive(root, "doubles");
    cJSON *first_double_item = cJSON_GetArrayItem(doubles_ptr, 0);
    double first_val = cJSON_GetNumberValue(first_double_item);
    float replacement_floats[3];
    replacement_floats[0] = (float)(first_val + 1.0);
    replacement_floats[1] = 15.5f;
    replacement_floats[2] = 25.5f;
    cJSON *new_floats = cJSON_CreateFloatArray(replacement_floats, 3);
    cJSON_ReplaceItemInObject(root, "floats", new_floats);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}