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
//<ID> 1121
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
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToObject(root, "values", values);
    cJSON *alpha = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "alpha", alpha);

    // step 2: Configure
    cJSON *note = cJSON_CreateString("metadata");
    cJSON_AddItemToObject(root, "note", note);

    // step 3: Operate and Validate
    cJSON *alpha_item = cJSON_GetObjectItemCaseSensitive(root, "alpha");
    double alpha_val = cJSON_GetNumberValue(alpha_item);
    cJSON *values_item = cJSON_GetObjectItemCaseSensitive(root, "values");
    cJSON *first_val = cJSON_GetArrayItem(values_item, 0);
    double first_num = cJSON_GetNumberValue(first_val);
    double computed = alpha_val + first_num * 2.0;
    cJSON *computed_item = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "computed", computed_item);
    char *out = cJSON_PrintUnformatted(root);
    char *scratch = (char *)cJSON_malloc(64);
    memset(scratch, 0, 64);
    scratch[0] = out[0];
    scratch[1] = out[1];
    cJSON_free(out);
    cJSON_free(scratch);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}