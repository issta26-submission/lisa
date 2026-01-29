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
//<ID> 1028
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"name\":\"test\",\"values\":[10,20,30],\"meta\":{\"desc\":\"sample\",\"id\":7}}";
    size_t json_len = sizeof(initial_json) - 1;
    cJSON *root = cJSON_ParseWithLength(initial_json, json_len);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *meta_dup = cJSON_Duplicate(meta, 1);
    int numbers[] = {4, 5, 6, 7};
    cJSON *ints = cJSON_CreateIntArray(numbers, 4);
    cJSON *out = cJSON_CreateObject();
    cJSON_AddItemToObject(out, "copied_meta", meta_dup);
    cJSON_AddItemToObject(out, "embedded_ints", ints);
    cJSON_AddItemToObject(out, "original_name", cJSON_CreateString(name_str));

    // step 3: Operate
    char *out_unformatted = cJSON_PrintUnformatted(out);
    char *print_buf = (char *)cJSON_malloc(512);
    memset(print_buf, 0, 512);
    cJSON_PrintPreallocated(out, print_buf, 512, 1);

    // step 4: Validate & Cleanup
    cJSON_free(out_unformatted);
    cJSON_free(print_buf);
    cJSON_Delete(root);
    cJSON_Delete(out);

    return 66;
    // API sequence test completed successfully
}