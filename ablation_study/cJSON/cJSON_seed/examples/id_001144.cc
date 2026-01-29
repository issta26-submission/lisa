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
//<ID> 1144
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char jsonA[] = "{\"arr\": [10, 20, 30], \"meta\": {\"id\":1}}";
    size_t lenA = sizeof(jsonA) - 1;
    cJSON *parsedA = cJSON_ParseWithLength(jsonA, lenA);
    const char jsonB[] = "{\"tag\":\"example\"}";
    cJSON *parsedB = cJSON_Parse(jsonB);
    cJSON *root = cJSON_CreateObject();
    cJSON *null_item = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToObject(root, "sourceA", parsedA);
    cJSON_AddItemToObject(root, "sourceB", parsedB);
    cJSON_AddItemToObject(root, "nullable", null_item);

    // step 3: Operate & Validate
    cJSON *arr = cJSON_GetObjectItem(parsedA, "arr");
    cJSON *second_elem = cJSON_GetArrayItem(arr, 1);
    double second_value = cJSON_GetNumberValue(second_elem);
    cJSON_AddNumberToObject(root, "second_value", second_value);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}