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
//<ID> 568
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
    cJSON *container = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "container", container);

    // step 2: Configure
    cJSON *raw = cJSON_CreateRaw("{\"inner\":123}");
    cJSON *raw_ref = cJSON_CreateObjectReference(raw);
    cJSON_AddItemReferenceToObject(container, "raw_ref", raw_ref);

    // step 3: Operate & Validate
    cJSON *retrieved_container = cJSON_GetObjectItem(root, "container");
    cJSON *retrieved_raw_ref = cJSON_GetObjectItem(retrieved_container, "raw_ref");
    cJSON_bool cmp = cJSON_Compare(retrieved_raw_ref, raw_ref, 1);
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snap_len = snapshot ? (size_t)strlen(snapshot) : 0;

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;cmp=%d;snaplen=%zu",
            version ? version : "null",
            (int)cmp,
            snap_len);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    cJSON_Delete(raw);
    // API sequence test completed successfully
    return 66;
}