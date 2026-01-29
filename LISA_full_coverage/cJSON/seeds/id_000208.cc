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
//<ID> 208
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *b1 = cJSON_CreateTrue();
    cJSON_AddItemToArray(array, n1);
    cJSON_AddItemToArray(array, s1);
    cJSON_AddItemToArray(array, b1);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "initial_count", (double)cJSON_GetArraySize(array));
    char *printed = cJSON_PrintBuffered(root, 128, 1);
    void *tmp_buf = cJSON_malloc(64);
    memset(tmp_buf, 0, 64);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(array, 0);
    double first_val = cJSON_GetNumberValue(first);
    int size_before = cJSON_GetArraySize(array);
    cJSON *detached = cJSON_DetachItemViaPointer(array, first);
    int size_after = cJSON_GetArraySize(array);
    int summary = (int)first_val + size_before + size_after;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp_buf);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}