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
//<ID> 1390
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *dup_arr = NULL;
    cJSON *retrieved = NULL;
    char *printed = NULL;
    int size_dup = 0;
    int size_orig = 0;
    int size_diff = 0;
    double values[4] = {3.14, 2.718, 1.618, 0.5772};

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateDoubleArray(values, 4);
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Operate / Validate
    dup_arr = cJSON_Duplicate(arr, 1);
    size_dup = cJSON_GetArraySize(dup_arr);
    retrieved = cJSON_GetObjectItem(root, "values");
    size_orig = cJSON_GetArraySize(retrieved);
    size_diff = size_dup - size_orig;
    printed = cJSON_PrintBuffered(root, 128, 1);
    (void)size_diff;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_arr);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}