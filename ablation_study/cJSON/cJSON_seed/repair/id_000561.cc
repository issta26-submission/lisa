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
//<ID> 561
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    cJSON_AddItemToArray(items, n3);
    double arrvals[3] = {1.1, 2.2, 3.3};
    cJSON *dbls = cJSON_CreateDoubleArray(arrvals, 3);
    cJSON_AddItemToObject(root, "dbls", dbls);

    // step 2: Configure
    cJSON_DeleteItemFromArray(items, 0);
    cJSON *found_dbls = cJSON_GetObjectItemCaseSensitive(root, "dbls");
    cJSON *first_dbl = cJSON_GetArrayItem(found_dbls, 0);
    double v = cJSON_GetNumberValue(first_dbl);
    cJSON_AddNumberToObject(root, "first_double_times_two", v * 2.0);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON *dup = cJSON_Duplicate(root, 1);
    char *out_dup = cJSON_PrintUnformatted(dup);
    cJSON_free(out_dup);
    cJSON_Delete(dup);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}