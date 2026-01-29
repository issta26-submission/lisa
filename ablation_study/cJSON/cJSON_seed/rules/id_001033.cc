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
//<ID> 1033
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
    cJSON *got_array = NULL;
    cJSON *got_flag = NULL;
    char *printed = NULL;
    const double nums[4] = {0.1, -2.5, 3.1415, 42.0};

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    darray = cJSON_CreateDoubleArray(nums, 4);
    cJSON_AddItemToObject(root, "measurements", darray);
    flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "active", flag);

    // step 3: Operate / Validate
    got_array = cJSON_GetObjectItemCaseSensitive(root, "measurements");
    got_flag = cJSON_GetObjectItemCaseSensitive(root, "active");
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}