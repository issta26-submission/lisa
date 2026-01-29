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
//<ID> 1394
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
    char *printed = NULL;
    double values[5] = { 1.1, 2.2, 3.3, 4.4, 5.5 };
    int orig_size = 0;
    int dup_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateDoubleArray(values, 5);
    cJSON_AddItemToObject(root, "values", arr);
    dup_arr = cJSON_Duplicate(arr, 1);

    // step 3: Operate / Validate
    orig_size = cJSON_GetArraySize(arr);
    dup_size = cJSON_GetArraySize(dup_arr);
    printed = cJSON_PrintBuffered(root, 256, 1);
    (void)orig_size;
    (void)dup_size;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_arr);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}