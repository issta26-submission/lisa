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
//<ID> 147
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
    cJSON *num_id = cJSON_CreateNumber(42.0);
    cJSON *null_val = cJSON_CreateNull();
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToArray(list, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(list, cJSON_CreateString("beta"));

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON_AddItemToObjectCS(meta, "id", num_id);
    cJSON_AddItemToObjectCS(root, "nullvalue", null_val);
    cJSON_AddItemToObjectCS(root, "list", list);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    cJSON *got_null = cJSON_GetObjectItemCaseSensitive(root, "nullvalue");
    cJSON_bool null_check = cJSON_IsNull(got_null);
    cJSON *got_meta = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON *got_id = cJSON_GetObjectItemCaseSensitive(got_meta, "id");
    cJSON_bool id_is_null = cJSON_IsNull(got_id);
    int summary = (int)null_check + (int)id_is_null + root->type + meta->type + got_id->type + (int)(printed ? printed[0] : 0);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}