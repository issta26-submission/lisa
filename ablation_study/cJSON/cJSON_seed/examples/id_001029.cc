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
//<ID> 1029
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n3);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_InsertItemInArray(values, 1, n2);
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddNumberToObject(root, "total", 60.0);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "count", (double)cJSON_GetArraySize(values));
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON_AddNumberToObject(root, "is_object_flag", (double)root_is_obj);

    // step 3: Operate & Validate
    cJSON *total_item = cJSON_GetObjectItem(root, "total");
    double total_val = cJSON_GetNumberValue(total_item);
    double average = total_val / (double)cJSON_GetArraySize(values);
    cJSON_AddNumberToObject(root, "average", average);
    char *out = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}