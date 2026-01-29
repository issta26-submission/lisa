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
//<ID> 1398
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float fnums[3] = {1.5f, 2.5f, 3.5f};
    cJSON *farray = cJSON_CreateFloatArray(fnums, 3);
    cJSON_AddItemToObject(root, "float_values", farray);
    double dnums[4] = {10.1, 20.2, 30.3, 40.4};
    cJSON *darray = cJSON_CreateDoubleArray(dnums, 4);
    cJSON_AddItemToObject(root, "double_values", darray);

    // step 2: Configure
    double new_dnums[2] = {99.9, 100.1};
    cJSON *new_darray = cJSON_CreateDoubleArray(new_dnums, 2);
    cJSON_ReplaceItemInObject(root, "double_values", new_darray);

    // step 3: Operate
    cJSON *retrieved = cJSON_GetObjectItemCaseSensitive(root, "double_values");
    int arr_size = cJSON_GetArraySize(retrieved);
    cJSON *first_elem = cJSON_GetArrayItem(retrieved, 0);
    double first_val = cJSON_GetNumberValue(first_elem);
    cJSON *first_num = cJSON_CreateNumber(first_val);
    cJSON_AddItemToObject(root, "first_double_value", first_num);
    cJSON *count_item = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "double_count", count_item);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}