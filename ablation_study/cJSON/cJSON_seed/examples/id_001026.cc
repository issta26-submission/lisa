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
//<ID> 1026
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
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(values, n0);
    cJSON_AddItemToArray(values, n1);

    // step 2: Configure
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddNumberToObject(root, "initial_count", 2.0);

    // step 3: Operate & Validate
    cJSON *n_insert = cJSON_CreateNumber(15.0);
    cJSON_bool insert_ok = cJSON_InsertItemInArray(values, 1, n_insert);
    cJSON *first = cJSON_GetArrayItem(values, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    double computed = first_val + (double)insert_ok + (double)root_is_obj;
    cJSON_AddNumberToObject(root, "computed_count", computed);

    // step 4: Cleanup
    char *out = cJSON_Print(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}