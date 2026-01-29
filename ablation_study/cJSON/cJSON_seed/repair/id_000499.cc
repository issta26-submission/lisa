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
//<ID> 499
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *initial_json = "{\"title\":\"sequence_test\",\"count\":3}";
    cJSON *root = cJSON_Parse(initial_json);
    const cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_value = cJSON_GetNumberValue(count_item);

    // step 2: Configure
    cJSON *array = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *n1 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(array, n0);
    cJSON_AddItemToArray(array, n1);
    cJSON *subobj = cJSON_AddObjectToObject(root, "generated");
    cJSON_AddItemToObject(subobj, "values", array);
    cJSON *note = cJSON_CreateString("configured");
    cJSON_AddItemToObject(root, "note", note);

    // step 3: Operate
    double scaled = count_value * 4.0;
    cJSON *scaled_num = cJSON_CreateNumber(scaled);
    cJSON_AddItemToArray(array, scaled_num);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate and Cleanup
    cJSON *found = cJSON_GetObjectItem(root, "generated");
    (void)found;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}