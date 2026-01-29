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
//<ID> 490
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_Parse("{\"name\":\"root\",\"count\":2}");
    cJSON *array = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(100.0);
    cJSON *n1 = cJSON_CreateNumber(200.5);
    cJSON_AddItemToArray(array, n0);
    cJSON_AddItemToArray(array, n1);

    // step 2: Configure
    cJSON *container = cJSON_AddObjectToObject(root, "container");
    cJSON_AddItemToObject(container, "numbers", array);
    cJSON *label = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON *parsed_again = cJSON_Parse("{\"reparsed\":true}");
    cJSON_AddItemToObject(root, "reparsed", parsed_again);

    // step 4: Validate and Cleanup
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    (void)count_val;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}