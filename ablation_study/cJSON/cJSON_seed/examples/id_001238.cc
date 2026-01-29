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
//<ID> 1238
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double nums1[3] = {1.5, 2.5, 3.5};
    double nums2[3] = {4.5, 5.5, 6.5};
    cJSON *root = cJSON_CreateObject();
    cJSON *dbl_array1 = cJSON_CreateDoubleArray(nums1, 3);
    cJSON *dbl_array2 = cJSON_CreateDoubleArray(nums2, 3);
    cJSON *container = cJSON_CreateArray();
    cJSON *null_item = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToObject(root, "numbers_a", dbl_array1);
    cJSON_AddItemToArray(container, dbl_array2);
    cJSON_AddItemToObject(root, "container", container);
    cJSON_AddItemToObject(root, "maybe_null", null_item);
    cJSON_bool is_null = cJSON_IsNull(null_item);
    cJSON *is_null_flag = cJSON_CreateBool(is_null);
    cJSON_AddItemToObject(root, "is_null_flag", is_null_flag);

    // step 3: Operate & Validate
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, dup, 1);
    cJSON_AddItemToObject(root, "equal", cJSON_CreateBool(equal));
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}