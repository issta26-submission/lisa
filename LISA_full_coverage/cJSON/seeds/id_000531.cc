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
//<ID> 531
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *names[] = { "alpha", "beta", "gamma" };
    cJSON *name_array = cJSON_CreateStringArray(names, 3);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "names", name_array);

    // step 2: Configure
    cJSON *numbers = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON *n3 = cJSON_CreateNumber(3.75);
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToArray(numbers, n2);
    cJSON_AddItemToArray(numbers, n3);
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON *meta = cJSON_CreateObject();
    double sum = cJSON_GetNumberValue(n1) + cJSON_GetNumberValue(n2) + cJSON_GetNumberValue(n3);
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(meta, "sum", sum_item);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate
    double extracted_sum = cJSON_GetNumberValue(cJSON_GetObjectItem(cJSON_GetObjectItem(root, "meta"), "sum"));
    cJSON *detached = cJSON_DetachItemFromArray(name_array, 1);
    const char *detached_str = cJSON_GetStringValue(detached);
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    size_t buf_len = snaplen + 64;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    const char *version = cJSON_Version();

    // step 4: Validate & Cleanup
    double first_number = cJSON_GetNumberValue(cJSON_GetArrayItem(numbers, 0));
    (void)detached_str;
    (void)extracted_sum;
    (void)version;
    (void)first_number;
    if (detached) { cJSON_Delete(detached); }
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}