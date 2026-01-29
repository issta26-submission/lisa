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
//<ID> 1966
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
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(meta, "ok", flag);
    cJSON *label = cJSON_CreateString("init_label");
    cJSON_AddItemToObject(meta, "label", label);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *first = cJSON_CreateObject();
    cJSON_AddItemToArray(items, first);
    cJSON *fname = cJSON_CreateString("first");
    cJSON_AddItemToObject(first, "name", fname);

    // step 2: Configure
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_label = cJSON_GetObjectItem(got_meta, "label");
    cJSON *second = cJSON_CreateObject();
    cJSON_AddItemToArray(items, second);
    cJSON *sname = cJSON_CreateString("second");
    cJSON_AddItemToObject(second, "name", sname);

    // step 3: Operate
    const int buflen = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 0);
    char *label_json = cJSON_PrintUnformatted(got_label);
    cJSON_free(label_json);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}