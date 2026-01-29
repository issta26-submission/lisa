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
//<ID> 565
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double nums[4] = {1.1, 2.2, 3.3, 4.4};
    cJSON *dbls = cJSON_CreateDoubleArray(nums, 4);
    cJSON_AddItemToObject(root, "dbls", dbls);
    cJSON_AddNumberToObject(root, "initial", 5.5);

    // step 2: Configure
    cJSON *found_dbls = cJSON_GetObjectItemCaseSensitive(root, "dbls");
    cJSON *elem1 = cJSON_GetArrayItem(found_dbls, 1);
    cJSON *elem2 = cJSON_GetArrayItem(found_dbls, 2);
    double v1 = cJSON_GetNumberValue(elem1);
    double v2 = cJSON_GetNumberValue(elem2);
    double sum = v1 + v2;
    cJSON_AddNumberToObject(root, "sum", sum);

    // step 3: Operate and Validate
    double newval = cJSON_SetNumberHelper(elem2, v2 * 2.0);
    cJSON_AddNumberToObject(root, "doubled", newval);
    char *out = cJSON_Print(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_DeleteItemFromArray(dbls, 0);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}