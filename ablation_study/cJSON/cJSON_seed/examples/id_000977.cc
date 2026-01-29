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
//<ID> 977
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *number_item = cJSON_CreateNumber(3.1415);
    cJSON *string_item = cJSON_CreateString("sensorA");
    cJSON *meta = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToArray(array, number_item);
    cJSON_AddItemToArray(array, string_item);
    cJSON_AddItemToObject(root, "items", array);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate & Validate
    double prev = cJSON_SetNumberHelper(number_item, 42.0);
    double extracted = cJSON_GetNumberValue(number_item);
    double doubled = extracted * 2.0;
    cJSON_AddItemToObject(meta, "doubled_value", cJSON_CreateNumber(doubled));
    char *name_val = cJSON_GetStringValue(string_item);
    cJSON_AddItemToObject(meta, "name_copy", cJSON_CreateString(name_val));
    (void)prev;
    (void)name_val;

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}