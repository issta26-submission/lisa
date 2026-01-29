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
//<ID> 895
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
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *flags = cJSON_AddArrayToObject(root, "flags");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(flags, true_item);

    // step 2: Configure
    cJSON *n = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(flags, n);
    cJSON_AddStringToObject(root, "status", "ok");
    cJSON *details = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "details", details);
    cJSON_AddNumberToObject(details, "count", 2.0);

    // step 3: Operate and Validate
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool ok = cJSON_PrintPreallocated(root, buffer, buf_len, (cJSON_bool)1);
    cJSON_AddNumberToObject(root, "print_ok", (double)ok);
    cJSON *snapshot = cJSON_CreateString(buffer);
    cJSON_AddItemToObject(root, "snapshot", snapshot);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}