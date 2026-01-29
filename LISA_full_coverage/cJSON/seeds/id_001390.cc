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
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float fnums[3] = {1.5f, 2.5f, 3.5f};
    double dnums[4] = {4.4, 5.5, 6.6, 7.7};
    cJSON *farr = cJSON_CreateFloatArray(fnums, 3);
    cJSON_AddItemToObject(root, "values", farr);
    cJSON_AddNumberToObject(root, "count", 3.0);

    // step 2: Configure
    cJSON *darr = cJSON_CreateDoubleArray(dnums, 4);
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "values", darr);
    (void)replaced;
    cJSON_AddStringToObject(root, "status", "configured");

    // step 3: Operate
    cJSON *got_values = cJSON_GetObjectItemCaseSensitive(root, "values");
    cJSON *got_count = cJSON_GetObjectItemCaseSensitive(root, "count");
    int reported_count = (int)cJSON_GetNumberValue(got_count);
    int arr_size = cJSON_GetArraySize(got_values);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);

    // step 4: Validate & Cleanup
    cJSON_AddNumberToObject(root, "size_reported", (double)arr_size);
    cJSON_AddNumberToObject(root, "original_count", (double)reported_count);
    cJSON *dup_values = cJSON_Duplicate(got_values, 1);
    cJSON_AddItemToObject(root, "values_dup", dup_values);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}