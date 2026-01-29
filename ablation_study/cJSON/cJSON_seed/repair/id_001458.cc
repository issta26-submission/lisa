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
//<ID> 1458
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON_AddItemToObject(config, "name", cJSON_CreateString("example-name"));
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(items, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(items, cJSON_CreateString("beta"));

    // step 2: Configure
    cJSON_AddStringToObject(root, "note", "api-sequence-test");
    cJSON_AddNumberToObject(root, "expected_count", (double)cJSON_GetArraySize(items));

    // step 3: Operate and Validate
    cJSON_bool has_cfg = cJSON_HasObjectItem(root, "config");
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(config, "name");
    cJSON *items_item = cJSON_GetObjectItemCaseSensitive(root, "items");
    cJSON_bool items_are_array = cJSON_IsArray(items_item);
    int count = cJSON_GetArraySize(items_item);
    char *out = cJSON_PrintUnformatted(root);
    size_t buf_len = 128;
    char *scratch = (char *)cJSON_malloc(buf_len);
    memset(scratch, 0, buf_len);
    scratch[0] = has_cfg ? '1' : '0';
    scratch[1] = items_are_array ? '1' : '0';
    scratch[2] = (char)('0' + (count % 10));
    scratch[3] = out ? out[0] : ' ';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}