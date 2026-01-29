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
//<ID> 99
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
    cJSON_AddStringToObject(meta, "name", "synth");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *num = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToArray(arr, num);
    cJSON *flag = cJSON_AddTrueToObject(root, "enabled");

    // step 2: Configure
    cJSON *elem = cJSON_CreateString("element");
    cJSON_AddItemToArray(arr, elem);
    cJSON *meta_dup = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToArray(arr, meta_dup);

    // step 3: Operate and Validate
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);
    cJSON_bool flag_is_true = cJSON_IsTrue(flag);
    char *printed_heap = cJSON_Print(root);
    char printed_buf[256];
    memset(printed_buf, 0, sizeof(printed_buf));
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, printed_buf, (int)sizeof(printed_buf), 1);
    char *printed_unfmt = cJSON_PrintUnformatted(root);
    cJSON_free(printed_heap);
    cJSON_free(printed_unfmt);
    int summary = (int)meta_is_obj + (int)flag_is_true + (int)prealloc_ok + (int)printed_buf[0];
    (void)summary;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}