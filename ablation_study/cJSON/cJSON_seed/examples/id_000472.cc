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
//<ID> 472
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", array);
    cJSON_AddStringToObject(root, "name", "fuzzer_v1");
    (void)version;

    // step 2: Configure
    cJSON *raw = cJSON_CreateRaw("{\"raw\":123}");
    cJSON_AddItemToArray(array, raw);
    cJSON *t = cJSON_CreateTrue();
    cJSON_AddItemToArray(array, t);
    cJSON *f = cJSON_CreateFalse();
    cJSON_AddItemToArray(array, f);
    cJSON_AddStringToObject(root, "status", "configured");

    // step 3: Operate & Validate
    int sz = cJSON_GetArraySize(array);
    cJSON *it0 = cJSON_GetArrayItem(array, 0);
    cJSON *it1 = cJSON_GetArrayItem(array, 1);
    cJSON *it2 = cJSON_GetArrayItem(array, 2);
    cJSON_bool is_t = cJSON_IsTrue(it1);
    cJSON_bool is_f = cJSON_IsFalse(it2);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "size=%d;true=%d;false=%d;equal=%d", sz, (int)is_t, (int)is_f, (int)equal);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_DeleteItemFromArray(array, 0);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}