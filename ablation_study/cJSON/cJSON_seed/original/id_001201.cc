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
//<ID> 1201
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);

    // step 2: Configure
    cJSON *ref1 = cJSON_CreateStringReference("alpha");
    cJSON_InsertItemInArray(arr, 0, ref1);
    cJSON *owned = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, owned);
    cJSON *ref2 = cJSON_CreateStringReference("gamma");
    cJSON_InsertItemInArray(arr, 1, ref2);

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);

    // step 4: Validate & Cleanup
    const char *first = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 0));
    (void)first;
    cJSON_free(pretty);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}