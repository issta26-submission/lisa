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
//<ID> 1039
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *darray = NULL;
    cJSON *flag = NULL;
    cJSON *got_darray = NULL;
    cJSON *first_elem = NULL;
    cJSON *last_elem = NULL;
    char *printed = NULL;
    const double nums[4] = {0.5, 1.5, 2.5, 3.5};

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag);
    darray = cJSON_CreateDoubleArray(nums, 4);
    cJSON_AddItemToObject(root, "measurements", darray);
    cJSON_AddItemToObject(root, "status", cJSON_CreateString("ok"));

    // step 3: Operate / Validate
    got_darray = cJSON_GetObjectItemCaseSensitive(root, "measurements");
    int count = cJSON_GetArraySize(got_darray);
    first_elem = cJSON_GetArrayItem(got_darray, 0);
    last_elem = cJSON_GetArrayItem(got_darray, count - 1);
    double first_val = cJSON_GetNumberValue(first_elem);
    double last_val = cJSON_GetNumberValue(last_elem);
    double avg_edge = (first_val + last_val) / 2.0;
    cJSON_AddItemToObject(root, "avg_edge", cJSON_CreateNumber(avg_edge));
    cJSON_bool was_disabled = cJSON_IsFalse(cJSON_GetObjectItemCaseSensitive(root, "enabled"));
    (void)was_disabled;
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}