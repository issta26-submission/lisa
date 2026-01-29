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
//<ID> 532
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *strings[] = { "alpha", "beta", "gamma" };
    cJSON *root = cJSON_CreateObject();
    cJSON *mixed_array = cJSON_CreateStringArray(strings, 3);
    cJSON *num_item = cJSON_CreateNumber(123.456);
    cJSON_AddItemToArray(mixed_array, num_item);
    cJSON_AddItemToObject(root, "mixed", mixed_array);

    // step 2: Configure
    const char *version = cJSON_Version();
    (void)version;
    int size_before = cJSON_GetArraySize(mixed_array);
    double extracted_number = cJSON_GetNumberValue(cJSON_GetArrayItem(mixed_array, 3));

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromArray(mixed_array, 1);
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    int buf_len = (int)snaplen + 64;
    void *buffer = cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);

    // step 4: Validate & Cleanup
    double after_number = cJSON_GetNumberValue(cJSON_GetArrayItem(mixed_array, size_before - 2));
    (void)extracted_number;
    (void)after_number;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}