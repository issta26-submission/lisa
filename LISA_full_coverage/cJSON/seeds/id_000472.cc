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
//<ID> 472
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"title\":\"example\",\"values\":[10,20,30]}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *values = cJSON_GetObjectItem(root, "values");
    cJSON *first = cJSON_GetArrayItem(values, 0);
    double first_val = cJSON_GetNumberValue(first);

    // step 2: Configure
    cJSON *extras = cJSON_AddArrayToObject(root, "extras");
    cJSON *num = cJSON_CreateNumber(0.0);
    double updated_num = cJSON_SetNumberHelper(num, 99.5);
    cJSON_AddItemReferenceToArray(extras, num);
    cJSON_bool root_is_object = cJSON_IsObject(root);
    cJSON_AddItemReferenceToArray(extras, first);
    cJSON *flag = cJSON_AddTrueToObject(root, "active");
    (void)updated_num;
    (void)root_is_object;
    (void)flag;

    // step 3: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    int values_count = cJSON_GetArraySize(values);
    int extras_count = cJSON_GetArraySize(extras);
    double moved_val = cJSON_GetNumberValue(cJSON_GetArrayItem(extras, 0));
    (void)first_val;
    (void)values_count;
    (void)extras_count;
    (void)moved_val;

    // step 4: Validate & Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}