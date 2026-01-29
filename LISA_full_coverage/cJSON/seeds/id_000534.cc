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
//<ID> 534
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *strings[] = { "alpha", "beta", "gamma" };
    cJSON *array = cJSON_CreateStringArray(strings, 3);
    cJSON *num_a = cJSON_CreateNumber(3.5);
    cJSON *num_b = cJSON_CreateNumber(7.25);
    cJSON_AddItemToArray(array, num_a);
    cJSON_AddItemToArray(array, num_b);
    const char *version = cJSON_Version();

    // step 2: Configure
    int size_before = cJSON_GetArraySize(array);
    double last_value_before = cJSON_GetNumberValue(cJSON_GetArrayItem(array, size_before - 1));
    cJSON *detached_item = cJSON_DetachItemFromArray(array, size_before - 1);
    double detached_value = cJSON_GetNumberValue(detached_item);
    double combined_value = last_value_before + detached_value + (double)strlen(version);
    cJSON *combined_num = cJSON_CreateNumber(combined_value);
    cJSON_AddItemToArray(array, combined_num);

    // step 3: Operate
    char *snapshot = cJSON_PrintUnformatted(array);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    size_t buf_len = snaplen + 32;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);

    // step 4: Validate & Cleanup
    double extracted = cJSON_GetNumberValue(cJSON_GetArrayItem(array, cJSON_GetArraySize(array) - 1));
    (void)extracted;
    (void)version;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(detached_item);
    cJSON_Delete(array);
    // API sequence test completed successfully
    return 66;
}