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
//<ID> 686
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", cfg);
    cJSON *pi_item = cJSON_AddNumberToObject(cfg, "pi", 3.1415);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(cfg, "maybe", maybe_null);

    // step 2: Configure
    double new_pi = cJSON_SetNumberHelper(pi_item, 2.718281828);
    cJSON *e_copy = cJSON_AddNumberToObject(cfg, "e_value", new_pi);
    (void)e_copy;

    // step 3: Operate and Validate
    cJSON *detached_pi = cJSON_DetachItemViaPointer(cfg, pi_item);
    cJSON_AddItemToObject(root, "moved_pi", detached_pi);
    char *snapshot = cJSON_PrintUnformatted(root);
    int buflen = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 0);
    (void)printed;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}