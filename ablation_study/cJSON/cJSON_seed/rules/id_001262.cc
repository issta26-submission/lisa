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
//<ID> 1262
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = NULL;
    cJSON *raw = NULL;
    cJSON *ok = NULL;
    cJSON *retrieved_meta = NULL;
    cJSON *retrieved_raw = NULL;
    cJSON *dup = NULL;
    char *printed = NULL;
    cJSON_bool add_raw_result = 0;
    cJSON_bool is_raw_flag = 0;
    cJSON_bool is_true_flag = 0;
    cJSON_bool compare_result = 0;
    const char raw_json[] = "{\"raw_key\":123}";

    // step 2: Setup / Configure
    meta = cJSON_AddObjectToObject(root, "meta");
    raw = cJSON_CreateRaw(raw_json);
    add_raw_result = cJSON_AddItemToObjectCS(meta, "raw", raw);
    ok = cJSON_AddTrueToObject(root, "ok");

    // step 3: Operate / Validate
    retrieved_meta = cJSON_GetObjectItem(root, "meta");
    retrieved_raw = cJSON_GetObjectItem(retrieved_meta, "raw");
    is_raw_flag = cJSON_IsRaw(retrieved_raw);
    is_true_flag = cJSON_IsTrue(cJSON_GetObjectItem(root, "ok"));
    dup = cJSON_Duplicate(root, 1);
    compare_result = cJSON_Compare(root, dup, 1);
    printed = cJSON_PrintUnformatted(root);
    (void)add_raw_result;
    (void)is_raw_flag;
    (void)is_true_flag;
    (void)compare_result;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}