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
//<ID> 1353
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"example\",\"meta\":{\"count\":3}}";
    cJSON *parsed = cJSON_Parse(json_text);
    float numbers[3] = {3.14f, 1.5f, -2.0f};
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *child = cJSON_AddObjectToObject(root, "child");
    cJSON_AddTrueToObject(root, "active");
    cJSON *float_array = cJSON_CreateFloatArray(numbers, 3);
    cJSON_AddItemToObject(child, "floats", float_array);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(child, "nothing", null_item);
    cJSON_AddItemToObject(root, "parsed", parsed);

    // step 3: Operate
    cJSON *retrieved_child = cJSON_GetObjectItem(root, "child");
    int float_count = cJSON_GetArraySize(cJSON_GetObjectItem(retrieved_child, "floats"));
    (void)float_count;
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