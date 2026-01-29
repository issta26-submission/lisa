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
//<ID> 1533
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
    cJSON *name_item = cJSON_AddStringToObject(meta, "name", "fuzz_tester");
    cJSON *enabled = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", enabled);

    // step 2: Configure
    cJSON *count = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(root, "count", count);
    double count_val = cJSON_GetNumberValue(count);
    cJSON *double_count = cJSON_CreateNumber(count_val * 2.0);
    cJSON_AddItemToObject(root, "double_count", double_count);

    // step 3: Operate and Validate
    cJSON *retrieved = cJSON_GetObjectItem(root, "double_count");
    double retrieved_val = cJSON_GetNumberValue(retrieved);
    char *scratch = (char *)cJSON_malloc(64);
    memset(scratch, 0, 64);
    scratch[0] = cJSON_GetStringValue(name_item)[0];
    scratch[1] = (char)('0' + (int)retrieved_val % 10);
    scratch[2] = (char)('#');
    cJSON_free(scratch);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}