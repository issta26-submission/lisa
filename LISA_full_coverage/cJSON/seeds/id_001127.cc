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
//<ID> 1127
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double values[] = {1.5, 2.5, 3.5};
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(root, "value", num_item);
    cJSON *dbl_array = cJSON_CreateDoubleArray(values, 3);
    cJSON_AddItemToObject(root, "doubles", dbl_array);
    cJSON *name_item = cJSON_CreateString("original");
    cJSON_AddItemToObject(root, "name", name_item);

    // step 2: Configure
    char *old_name = cJSON_SetValuestring(name_item, "updated");
    (void)old_name;

    // step 3: Operate
    cJSON_bool is_num = cJSON_IsNumber(num_item);
    double num_val = cJSON_GetNumberValue(num_item);
    double computed = num_val + (double)is_num;
    cJSON_AddNumberToObject(root, "computed", computed);

    // step 4: Validate & Cleanup
    char *unformatted = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *prealloc = (char *)cJSON_malloc((size_t)buf_len);
    memset(prealloc, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, prealloc, buf_len, 1);
    cJSON_free(unformatted);
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}