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
//<ID> 538
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *words[] = { "alpha", "beta", "gamma" };
    cJSON *arr = cJSON_CreateStringArray(words, 3);
    cJSON *num = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, num);

    // step 2: Configure
    const char *ver = cJSON_Version();
    cJSON *ver_item = cJSON_CreateString(ver);
    cJSON_AddItemToArray(arr, ver_item);
    double initial_num = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 3));

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromArray(arr, 1);
    char *snapshot = cJSON_PrintUnformatted(arr);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    size_t buf_len = snaplen + 32;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);

    // step 4: Validate & Cleanup
    double extracted = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 2));
    (void)initial_num;
    (void)extracted;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(detached);
    cJSON_Delete(arr);
    // API sequence test completed successfully
    return 66;
}