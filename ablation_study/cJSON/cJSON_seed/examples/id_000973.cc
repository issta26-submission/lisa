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
//<ID> 973
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
    cJSON *meta = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(12.34);
    cJSON *str_item = cJSON_CreateString("sensorA");
    cJSON *flag = cJSON_CreateBool(1);

    // step 2: Configure
    cJSON_AddItemToArray(array, num_item);
    cJSON_AddItemToArray(array, str_item);
    cJSON_AddItemToObject(root, "data", array);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "active", flag);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(array, 0);
    cJSON_SetNumberHelper(first, 99.9);
    double extracted = cJSON_GetNumberValue(first);
    cJSON_AddItemToObject(root, "extracted_value", cJSON_CreateNumber(extracted));
    cJSON *second = cJSON_GetArrayItem(array, 1);
    char *current_name = cJSON_GetStringValue(second);
    cJSON_AddItemToObject(root, "name_copy", cJSON_CreateString(current_name));

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}