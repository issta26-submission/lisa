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
//<ID> 365
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double numbers[3] = {1.1, 2.2, 3.3};
    cJSON *dbl_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObject(root, "values", dbl_array);

    // step 2: Configure
    cJSON *extra = cJSON_CreateNumber(4.4);
    cJSON_AddItemToArray(dbl_array, extra);
    int array_size = cJSON_GetArraySize(dbl_array);
    cJSON *second_item = cJSON_GetArrayItem(dbl_array, 1);
    cJSON *stats = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "stats", stats);
    cJSON *count_num = cJSON_CreateNumber((double)array_size);
    cJSON_AddItemToObject(stats, "count", count_num);
    cJSON *second_val_ref = cJSON_CreateNumber(cJSON_GetNumberValue(second_item));
    cJSON_AddItemToObject(stats, "second_value", second_val_ref);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, buf_len);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 0);
    (void)prealloc_ok;
    (void)printed;
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}