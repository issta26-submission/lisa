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
//<ID> 95
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "name", "example");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *flag = cJSON_AddTrueToObject(root, "active");

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON_AddItemToArray(arr, num);
    cJSON *str = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str);

    // step 3: Operate and Validate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    cJSON_bool flag_true = cJSON_IsTrue(flag);
    cJSON_bool meta_obj = cJSON_IsObject(meta);
    char *scratch = (char *)cJSON_malloc(64);
    memset(scratch, 0, 64);
    /* copy a small portion of the printed output into scratch to exercise cJSON_malloc/cJSON_free */
    memcpy(scratch, buffer, 63);
    int summary = (int)printed_ok + (int)flag_true + (int)meta_obj + (int)(scratch[0]);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}