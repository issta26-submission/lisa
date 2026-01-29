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
//<ID> 1397
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
    int arr_size = 0;
    double values[4] = {1.1, 2.2, 3.3, 4.4};
    int prebuffer = 256;
    cJSON_bool fmt = 1;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateDoubleArray(values, 4);
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Operate / Validate
    dup_arr = cJSON_Duplicate(arr, 1);
    retrieved = cJSON_GetObjectItem(root, "values");
    arr_size = cJSON_GetArraySize(retrieved);
    printed = cJSON_PrintBuffered(root, prebuffer, fmt);
    (void)arr_size;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_arr);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}