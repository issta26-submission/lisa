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
//<ID> 563
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double nums[] = {1.1, 2.2, 3.3};
    cJSON *values = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "values", values);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "label", "dataset");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *fetched_values = cJSON_GetObjectItemCaseSensitive(root, "values");
    cJSON *first_item = cJSON_GetArrayItem(fetched_values, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *derived = cJSON_CreateNumber(first_val + 10.0);
    cJSON_AddItemToObject(meta, "first_plus_ten", derived);
    cJSON_DeleteItemFromArray(fetched_values, 1);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON *got_meta = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON *got_first_plus = cJSON_GetObjectItemCaseSensitive(got_meta, "first_plus_ten");
    double fetched = cJSON_GetNumberValue(got_first_plus);
    double updated = cJSON_SetNumberHelper(got_first_plus, fetched * 2.0);
    (void)updated;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}