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
//<ID> 590
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double initial_nums[] = {1.0, 2.0, 3.0};
    double replacement_nums[] = {9.9, 8.8};
    cJSON *root = cJSON_CreateObject();
    cJSON *pi_item = cJSON_CreateNumber(3.14159265);
    cJSON_AddItemToObject(root, "pi", pi_item);
    cJSON *values_array = cJSON_CreateDoubleArray(initial_nums, 3);
    cJSON_AddItemToObject(root, "values", values_array);

    // step 2: Configure
    cJSON *fetched_pi = cJSON_GetObjectItem(root, "pi");
    double pi_val = cJSON_GetNumberValue(fetched_pi);
    cJSON *replacement_array = cJSON_CreateDoubleArray(replacement_nums, 2);
    cJSON_ReplaceItemViaPointer(root, fetched_pi, replacement_array);

    // step 3: Operate
    cJSON *pi_after = cJSON_GetObjectItem(root, "pi");
    int replaced_size = cJSON_GetArraySize(pi_after);
    cJSON *dup = cJSON_Duplicate(root, 1);
    char *printed = cJSON_PrintUnformatted(dup);
    size_t printed_len = printed ? strlen(printed) : 0;
    char *scratch = (char *)cJSON_malloc(printed_len + 1);
    memset(scratch, 0, printed_len + 1);
    if (printed && scratch) strcpy(scratch, printed);

    // step 4: Validate & Cleanup
    (void)pi_val;
    (void)replaced_size;
    (void)scratch;
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}