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
//<ID> 1432
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"alpha\":1,\"beta\":2}";
    size_t json_len = strlen(json_text);
    cJSON *root = cJSON_ParseWithLength(json_text, json_len);

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_AddRawToObject(dup, "injected_raw", "{\"nested\":true}");
    cJSON *beta_item = cJSON_GetObjectItem(root, "beta");
    cJSON *detached_beta = cJSON_DetachItemViaPointer(root, beta_item);
    cJSON_AddItemToObject(dup, "moved_beta", detached_beta);

    // step 3: Operate & Validate
    const int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(dup, buffer, buf_len, 1);
    (void)printed_ok;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(dup);

    // API sequence test completed successfully
    return 66;
}