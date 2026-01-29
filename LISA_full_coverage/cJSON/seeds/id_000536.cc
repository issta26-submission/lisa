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
//<ID> 536
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *labels[] = { "10", "20", "30" };
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateStringArray(labels, 3);
    cJSON_AddItemToObject(root, "labels", arr);
    cJSON *base_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "base", base_num);

    // step 2: Configure
    const char *version = cJSON_Version();
    double base_value = cJSON_GetNumberValue(base_num);
    double computed = base_value + 8.0;
    cJSON *computed_num = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "computed", computed_num);

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromArray(arr, 1);
    cJSON_AddItemToObject(root, "detached_label", detached);
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    size_t buf_len = snaplen + 32;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);

    // step 4: Validate & Cleanup
    double extracted_base = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "base"));
    double extracted_computed = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "computed"));
    (void)version;
    (void)extracted_base;
    (void)extracted_computed;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}