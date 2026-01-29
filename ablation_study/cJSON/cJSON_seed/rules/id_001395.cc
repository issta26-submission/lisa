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
//<ID> 1395
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *darr = NULL;
    cJSON *dup_darr = NULL;
    double *nums = NULL;
    char *printed = NULL;
    int size_orig = 0;
    int size_dup = 0;
    const int count = 4;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    nums = (double *)cJSON_malloc(sizeof(double) * count);
    nums[0] = 1.25;
    nums[1] = 2.5;
    nums[2] = 3.75;
    nums[3] = 4.0;
    darr = cJSON_CreateDoubleArray(nums, count);
    cJSON_AddItemToObject(root, "values", darr);
    dup_darr = cJSON_Duplicate(darr, 1);

    // step 3: Operate / Validate
    size_orig = cJSON_GetArraySize(darr);
    size_dup = cJSON_GetArraySize(dup_darr);
    printed = cJSON_PrintBuffered(root, 256, 1);
    (void)size_orig;
    (void)size_dup;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(nums);
    cJSON_Delete(dup_darr);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}