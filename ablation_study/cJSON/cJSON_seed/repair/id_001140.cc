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
//<ID> 1140
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_AddObjectToObject(root, "data");
    cJSON *items = cJSON_AddArrayToObject(data, "items");
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);

    // step 2: Configure
    cJSON_AddStringToObject(root, "note", "generated");
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    cJSON *data_ref = cJSON_GetObjectItemCaseSensitive(root, "data");
    cJSON *items_ref = cJSON_GetObjectItemCaseSensitive(data_ref, "items");
    cJSON *first = cJSON_GetArrayItem(items_ref, 0);
    double first_val = cJSON_GetNumberValue(first);
    double computed = first_val * 2.5;
    cJSON_AddNumberToObject(root, "computed", computed);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}