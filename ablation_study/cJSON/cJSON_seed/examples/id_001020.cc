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
//<ID> 1020
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
    cJSON *n10 = cJSON_CreateNumber(10.0);
    cJSON *n30 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(values, n10);
    cJSON_AddItemToArray(values, n30);
    cJSON *n20 = cJSON_CreateNumber(20.0);
    cJSON_bool inserted = cJSON_InsertItemInArray(values, 1, n20);

    // step 2: Configure
    cJSON_AddItemToObject(root, "values", values);
    cJSON_bool is_obj = cJSON_IsObject(root);
    cJSON_AddNumberToObject(root, "inserted_flag", (double)inserted);
    cJSON_AddNumberToObject(root, "is_object_flag", (double)is_obj);

    // step 3: Operate & Validate
    cJSON *mid_item = cJSON_GetArrayItem(values, 1);
    double mid_val = cJSON_GetNumberValue(mid_item);
    cJSON_AddNumberToObject(root, "middle", mid_val);
    char *out = cJSON_Print(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}