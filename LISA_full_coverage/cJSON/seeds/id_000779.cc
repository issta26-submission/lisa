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
//<ID> 779
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{ \"arr\": [10, 20, 30], \"meta\": \"begin\" }";
    size_t json_len = (sizeof(json_text) - 1);
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json_text, json_len, &parse_end, 1);
    cJSON *aux = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddRawToObject(root, "raw_insert", "{\"inserted\":true}");
    cJSON_AddNullToObject(root, "maybe_null");
    cJSON *array = cJSON_GetObjectItemCaseSensitive(root, "arr");

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromArray(array, 1);
    cJSON_AddItemToObject(aux, "moved", detached);
    cJSON_AddItemToObject(aux, "source_meta", cJSON_Duplicate(cJSON_GetObjectItemCaseSensitive(root, "meta"), 0));

    // step 4: Validate & Cleanup
    char *out_root = cJSON_PrintUnformatted(root);
    char *out_aux = cJSON_PrintUnformatted(aux);
    cJSON_free(out_root);
    cJSON_free(out_aux);
    cJSON_Delete(root);
    cJSON_Delete(aux);
    // API sequence test completed successfully
    return 66;
}