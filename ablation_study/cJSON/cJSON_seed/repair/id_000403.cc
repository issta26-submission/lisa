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
//<ID> 403
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
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n1 = cJSON_CreateNumber(20.5);
    cJSON *n2 = cJSON_CreateNumber(30.25);
    cJSON_AddItemToArray(items, n0);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    cJSON_AddItemToObject(root, "items", items);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "owner", "tester");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *items_copy = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", items_copy);
    cJSON_AddStringToObject(root, "dataset", "example_set");

    // step 3: Operate
    cJSON *fetched = cJSON_GetObjectItem(root, "items");
    int count = cJSON_GetArraySize(fetched);
    cJSON *middle = cJSON_GetArrayItem(fetched, 1);
    double mid_val = cJSON_GetNumberValue(middle);
    double computed = mid_val * (double)count;
    cJSON_AddNumberToObject(root, "computed", computed);

    // step 4: Validate and Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}