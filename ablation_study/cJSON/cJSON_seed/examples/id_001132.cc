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
//<ID> 1132
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double numbers[4] = {4.0, 5.5, -1.25, 0.0};
    cJSON *darr = cJSON_CreateDoubleArray(numbers, 4);
    cJSON *flag = cJSON_CreateTrue();
    cJSON *name = cJSON_CreateString("example");
    cJSON *meta = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToObject(root, "values", darr);
    cJSON_AddItemToObjectCS(root, "flag", flag);
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "count", (double)cJSON_GetArraySize(darr));

    // step 3: Operate & Validate
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    cJSON_bool is_flag_true = cJSON_IsTrue(flag_item);
    cJSON *first_item = cJSON_GetArrayItem(darr, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    double prev_first = cJSON_SetNumberHelper(first_item, first_val + 1.0);
    double computed = prev_first + first_val + (double)is_flag_true;
    cJSON_AddNumberToObject(root, "computed", computed);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}