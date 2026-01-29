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
//<ID> 1019
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *placeholder = NULL;
    cJSON *darray = NULL;
    cJSON *raw1 = NULL;
    cJSON *raw2 = NULL;
    cJSON *flag = NULL;
    cJSON *got_flag = NULL;
    cJSON *got_measurements = NULL;
    char *printed = NULL;
    const double nums[3] = {1.1, 2.2, 3.3};

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    placeholder = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "measurements", placeholder);
    flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", flag);
    raw1 = cJSON_CreateRaw("\"initial-meta\"");
    cJSON_AddItemToObject(root, "meta", raw1);

    // step 3: Operate / Validate
    darray = cJSON_CreateDoubleArray(nums, 3);
    cJSON_ReplaceItemViaPointer(root, cJSON_GetObjectItem(root, "measurements"), darray);
    got_measurements = cJSON_GetObjectItem(root, "measurements");
    printed = cJSON_PrintUnformatted(root);
    raw2 = cJSON_CreateRaw("{\"updated\":true}");
    cJSON_ReplaceItemViaPointer(root, cJSON_GetObjectItem(root, "meta"), raw2);
    got_flag = cJSON_GetObjectItem(root, "active");
    cJSON_bool is_flag_true = cJSON_IsTrue(got_flag);
    (void)is_flag_true;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}