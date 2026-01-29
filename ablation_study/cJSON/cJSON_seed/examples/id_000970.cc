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
//<ID> 970
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateArray();
    cJSON *s0 = cJSON_CreateString("sensorA");
    cJSON *n1 = cJSON_CreateNumber(12.0);
    cJSON *n2 = cJSON_CreateNumber(3.5);
    cJSON *metrics = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToArray(data, s0);
    cJSON_AddItemToArray(data, n1);
    cJSON_AddItemToArray(data, n2);
    cJSON_AddItemToObject(root, "readings", data);
    cJSON_AddItemToObject(root, "metrics", metrics);

    // step 3: Operate & Validate
    cJSON *got_data = cJSON_GetObjectItem(root, "readings");
    cJSON *first = cJSON_GetArrayItem(got_data, 0);
    char *name = cJSON_GetStringValue(first);
    cJSON *alias = cJSON_CreateString(name);
    cJSON_AddItemToObject(metrics, "name", alias);
    cJSON *second = cJSON_GetArrayItem(got_data, 1);
    double val = cJSON_GetNumberValue(second);
    cJSON *double_first = cJSON_CreateNumber(val * 2.0);
    cJSON_AddItemToObject(metrics, "double_first", double_first);
    cJSON *third = cJSON_GetArrayItem(got_data, 2);
    double set_result = cJSON_SetNumberHelper(third, 7.0);
    double newthird = cJSON_GetNumberValue(third);
    cJSON_AddItemToObject(metrics, "modified_third", cJSON_CreateNumber(newthird));
    (void)set_result;

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}